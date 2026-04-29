#include "cli_manager.h"
#include <iostream>
#include <string>
#include <vector>
#include <iomanip>
#include <sstream>
#include <cstdlib>
#include <ctime>
#include <utility>
#include "../commands/purchase_command.h"
#include "../commands/refund_command.h"
#include "../commands/restock_command.h"
#include "../commands/command_logger.h"
#include "../persistence/persistence_manager.h"
#include "../pretty_print.h"
#include "../registry/central_registry.h"
#include "../inventory/product.h"
#include "../payment/upi_adapter.h"
#include "../payment/card_adapter.h"
#include "../payment/wallet_adapter.h"
#include "../email/email_service.h"
#include "../core/otp_manager.h"

using namespace AuraCLI;

CLIManager::CLIManager(SimulationRunner& s) : sim(s) {}

// ─── Simple helpers ────────────────────────────────────────────────────
static void hr(char c = '-', int w = 66) {
    std::cout << Color::DIM << std::string(w, c) << Color::RESET << "\n";
}
static void hdr(const std::string& t, const std::string& col = Color::BCYAN) {
    const int totalWidth = 66;
    int textLen = visibleLen(t);
    int pad = totalWidth - textLen - 6; // 6 = "  ║  " + "  ║"
    if (pad < 0) pad = 0;

    std::cout << "\n" << col << Color::BOLD;
    std::cout << "  " << tDTL << repeatStr(tDH, totalWidth - 4) << tDTR << "\n";
    std::cout << "  " << tDV << "  " << t << std::string(pad, ' ') << "  " << tDV << "\n";
    std::cout << "  " << tDBL << repeatStr(tDH, totalWidth - 4) << tDBR << "\n";
    std::cout << Color::RESET;
}
static void menuLine(const std::string& key, const std::string& label,
                     const std::string& note = "", bool isExit = false) {
    std::string kc = isExit ? Color::BRED : Color::BGREEN;
    std::string lc = isExit ? Color::DIM  : Color::WHITE;
    std::cout << "  " << kc << Color::BOLD << "[" << key << "]" << Color::RESET
              << "  " << lc << label << Color::RESET;
    if (!note.empty())
        std::cout << "  " << Color::DIM << "(" << note << ")" << Color::RESET;
    std::cout << "\n";
}
static int readInt(const std::string& prompt, int lo = 0, int hi = 99) {
    int v;
    while (true) {
        std::cout << Color::YELLOW << Color::BOLD << "  >> " << Color::RESET << prompt;
        if (std::cin >> v && v >= lo && v <= hi) { std::cin.ignore(1000,'\n'); return v; }
        std::cin.clear(); std::cin.ignore(1000,'\n');
        fail("Invalid input. Enter a number between " + std::to_string(lo) + " and " + std::to_string(hi) + ".");
    }
}
static std::string readStr(const std::string& prompt) {
    std::cout << Color::YELLOW << Color::BOLD << "  >> " << Color::RESET << prompt;
    std::string s; std::getline(std::cin, s); return s;
}
static void pause() {
    std::cout << Color::DIM << "\n  [Press ENTER to continue] " << Color::RESET;
    std::cin.get();
}

// ─── Print live inventory for a kiosk type ────────────────────────────
static void showStock(const std::string& kt, InventoryProxy& inv) {
    CentralRegistry* reg = CentralRegistry::getInstance();
    std::vector<std::string> names, prices, qtys, types;
    for (auto& e : reg->getCatalog()) {
        if (e.kioskType != kt) continue;
        std::string displayName = e.item->getName();
        if (e.item->getItemType() == "Bundle") displayName += " (Bundle)";
        names.push_back(displayName);
        prices.push_back("Rs." + std::to_string((int)e.item->getPrice()));
        int q = inv.getStock(e.item->getName());
        qtys.push_back(q < 0 ? "N/A" : std::to_string(q));
        types.push_back(kt);
    }
    if (names.empty()) { warn("No items in catalog for this kiosk."); return; }
    printInventoryTable(names, prices, qtys, types);
}

// ─── Payment details flow ──────────────────────────────────────────────
static std::pair<std::shared_ptr<IPayment>, std::string> doPayment(double /*amount*/) {
    printPaymentMenu();
    int ch = readInt("Select payment [1=UPI / 2=Card / 3=Wallet]: ", 1, 3);

    std::shared_ptr<IPayment> pay;
    std::string pname;

    if (ch == 1) {
        // ── UPI: collect ID once, store in adapter ──────────────────
        printSection("UPI Setup   [Adapter Pattern]");
        std::string uid;
        while (true) {
            uid = readStr("Enter UPI ID (e.g. name@upi): ");
            if (uid.find("@upi") != std::string::npos) break;
            fail("Invalid UPI ID! Format must be 'name@upi'.");
        }
        info("Gateway: Routing to " + uid + " ...");
        ok("UPI ID verified and saved for this session.");
        auto upi = std::make_shared<UPIAdapter>();
        upi->setupUPI(uid);        // store — pay() will NEVER prompt again
        pay   = upi;
        pname = "UPI (" + uid + ")";

    } else if (ch == 2) {
        // ── Card: collect number/expiry/CVV once, store in adapter ──
        printSection("Card Setup   [Adapter Pattern]");
        std::string cnum = readStr("Card Number (16 digits): ");
        std::string expd = readStr("Expiry (MM/YY): ");
        std::string cvv  = readStr("CVV (3 digits): ");
        std::string last4 = cnum.size() >= 4 ? cnum.substr(cnum.size()-4) : cnum;
        info("Card **** **** **** " + last4 + " registered.");
        ok("Card saved for this session. All purchases will use this card.");
        auto card = std::make_shared<CardAdapter>();
        card->setupCard(cnum, expd);   // store — pay() will NEVER prompt again
        pay   = card;
        pname = "Card (***" + last4 + ")";

    } else {
        // ── Wallet: collect provider once, store in adapter ─────────
        printSection("Wallet Setup   [Adapter Pattern]");
        std::string wapp = readStr("Wallet App (Paytm / PhonePe / GPay): ");
        info("Linking " + wapp + " wallet ...");
        ok("Wallet saved for this session. All purchases will use " + wapp);
        auto wallet = std::make_shared<WalletAdapter>();
        wallet->setupWallet(wapp);     // store — pay() will NEVER prompt again
        pay   = wallet;
        pname = "Wallet (" + wapp + ")";
    }
    return {pay, pname};
}

// ─── Print receipt ─────────────────────────────────────────────────────
static void printReceipt(const PurchaseInfo& p) {
    std::cout << "\n" << Color::BCYAN << Color::BOLD;
    std::cout << "  ╔══════════════════════════════════════════════╗\n";
    std::cout << "  ║         AURA RETAIL OS  --  RECEIPT          ║\n";
    std::cout << "  ╠══════════════════════════════════════════════╣\n";
    auto row = [](const std::string& l, const std::string& v){
        int pad = 42 - (int)l.size() - (int)v.size();
        std::cout << "  ║  " << AuraCLI::Color::RESET << l
                  << (pad > 0 ? std::string(pad,' ') : "") << v
                  << AuraCLI::Color::BCYAN << AuraCLI::Color::BOLD << "  ║\n";
    };
    row("Location:", p.location);
    row("Payment:", p.paymentMethod);
    std::cout << "  ║  " << AuraCLI::Color::RESET << "------------------------------------------" << AuraCLI::Color::BCYAN << AuraCLI::Color::BOLD << "  ║\n";
    
    for (const auto& item : p.items) {
        std::string label = item.itemName + " (x" + std::to_string(item.quantity) + ")";
        row(label, "Rs." + std::to_string((int)item.subtotal));
    }

    std::cout << "  ║  " << AuraCLI::Color::RESET << "------------------------------------------" << AuraCLI::Color::BCYAN << AuraCLI::Color::BOLD << "  ║\n";
    row("TOTAL PAID:", "Rs." + std::to_string((int)p.totalAmount));
    row("Status:", "PAID / SUCCESSFUL");
    std::cout << Color::BCYAN << Color::BOLD;
    std::cout << "  ╚══════════════════════════════════════════════╝\n";
    std::cout << Color::RESET << "\n";
}

// ─── CUSTOMER FLOW ─────────────────────────────────────────────────────
static void runCustomer(SimulationRunner& sim, const std::string& username,
                        const std::string& email, CommandLogger& logger) {
    CentralRegistry* reg = CentralRegistry::getInstance();

    // Step 1: Kiosk selection
    hdr("STEP 1 of 3  >>  SELECT YOUR KIOSK", Color::BGREEN);
    std::cout << "\n";
    menuLine("1", "Food Kiosk",      "Metro Station - Gate 2");
    menuLine("2", "Pharmacy Kiosk",  "City Hospital - Wing B");
    menuLine("3", "Emergency Kiosk", "Disaster Zone - Sector 4");
    menuLine("0", "Exit", "", true);
    int kc = readInt("Select kiosk: ", 0, 3);
    if (kc == 0) return;

    std::shared_ptr<IKiosk> kiosk;
    std::string kt, location;
    std::shared_ptr<IDispenser> disp;
    std::vector<std::string> mods;

    if      (kc==1){ kiosk=sim.decoratedFood;      kt="food";      location="Metro Station - Gate 2";   mods={"Network: 94%","Refg: 4C"}; }
    else if (kc==2){ kiosk=sim.pharmacyKiosk;       kt="pharmacy";  location="City Hospital - Wing B";   }
    else           { kiosk=sim.decoratedEmergency;  kt="emergency"; location="Disaster Zone - Sector 4"; mods={"Solar: 87%"}; }

    disp = kiosk->getDispenser();

    InventoryProxy userInv(username, sim.sharedInventory);  // shares global stock

    // Step 2: Kiosk Dashboard
    hdr("STEP 2 of 3  >>  KIOSK DASHBOARD", Color::BGREEN);
    printDiagCard(kt + " Kiosk", location, disp->getDispenserType(), mods, true);

    // Email Service & Session Tracking
    EmailService emailSvc;
    std::vector<PurchaseInfo> sessionPurchases;

    // Payment session variables — initialized lazily on first purchase
    std::shared_ptr<IPayment> sessionPayment = nullptr;
    std::string sessionPayName;
    bool paymentInitialized = false;

    // Cart for multiple items
    struct CartItem {
        std::shared_ptr<IInventoryItem> item;
        int qty;
    };
    std::vector<CartItem> sessionCart;

    // Last-purchase tracking (for refund)
    std::string lastItem; double lastAmt = 0; bool lastOk = false; std::string lastPay;

    // Step 3: Customer menu loop
    bool running = true;
    while (running) {
        hdr("STEP 3 of 3  >>  CUSTOMER TERMINAL  //  " + location, Color::BGREEN);
        hr();
        menuLine("1", "Add Item to Cart",         "Browse & Select Items");
        menuLine("2", "View Cart & Checkout",     "Process Multi-item Purchase");
        menuLine("3", "View Current Stock",       "Composite Pattern View");
        menuLine("4", "Refund Last Purchase",     "Command Undo");
        menuLine("0", "Exit Terminal", "", true);
        hr();
        int ch = readInt("Select option: ", 0, 4);

        if (ch == 1) {
            // ── Browse catalogue ──────────────────────────────────────
            hdr("PRODUCT CATALOGUE  //  " + kt, Color::BGREEN);
            showStock(kt, userInv);

            // Select item
            std::vector<std::shared_ptr<IInventoryItem>> items;
            for (auto& e : reg->getCatalog())
                if (e.kioskType == kt) items.push_back(e.item);
            if (items.empty()) { warn("No items available."); pause(); continue; }

            int ic = readInt("Select item number: ", 1, (int)items.size());
            auto selected = items[ic-1];

            // ── COMPOSITE PATTERN: Show bundle details if it is a bundle ──
            if (selected->getItemType() == "Bundle") {
                hdr("BUNDLE DETAILS: " + selected->getName(), Color::BCYAN);
                selected->display(); // Shows nested products/bundles
                hr();
            }

            // Quantity
            int available = userInv.getStock(selected->getName());
            if (available <= 0) {
                fail("Item is OUT OF STOCK.");
                pause(); continue;
            }
            int maxQ = available > 5 ? 5 : available;
            info("Available stock: " + std::to_string(available) + "  |  Max per transaction: " + std::to_string(maxQ));
            int qty = readInt("Enter quantity (1-" + std::to_string(maxQ) + "): ", 1, maxQ);
            
            sessionCart.push_back({selected, qty});
            ok(selected->getName() + " x" + std::to_string(qty) + " added to cart.");
            pause();

        } else if (ch == 2) {
            // ── VIEW CART & CHECKOUT ──────────────────────────────────
            if (sessionCart.empty()) {
                warn("Your cart is empty! Add some items first.");
                pause(); continue;
            }

            hdr("YOUR SHOPPING CART", Color::BGREEN);
            double cartTotal = 0;
            for (auto& ci : sessionCart) {
                double sub = ci.item->getPrice() * ci.qty;
                std::cout << "  - " << std::left << std::setw(20) << ci.item->getName() 
                          << " x" << ci.qty << "  Rs." << (int)sub << "\n";
                cartTotal += sub;
            }
            hr();
            info("TOTAL PAYABLE: Rs." + std::to_string((int)cartTotal));
            
            int confirm = readInt("Proceed to payment? [1=Yes / 0=Cancel]: ", 0, 1);
            if (confirm == 0) continue;

            // ── Lazy payment init: ask ONLY once per checkout ─────────
            if (!paymentInitialized) {
                hdr("PAYMENT SETUP   >>   Adapter Pattern", Color::BGREEN);
                auto payResult    = doPayment(cartTotal);
                sessionPayment    = payResult.first;
                sessionPayName    = payResult.second;
                paymentInitialized = true;
                ok("Payment locked for session: " + sessionPayName);
            }
            
            auto payment = sessionPayment;
            auto pname   = sessionPayName;

            // Execute purchase for EACH item in cart
            bool anySuccess = false;
            PurchaseInfo finalReceipt;
            finalReceipt.totalAmount = 0;
            finalReceipt.paymentMethod = pname;
            finalReceipt.location = location;

            for (auto& ci : sessionCart) {
                double itemTotal = ci.item->getPrice() * ci.qty;
                PurchaseCommand cmd(ci.item->getName(), itemTotal, *payment, userInv, *disp);
                cmd.execute();
                
                if (cmd.isExecuted()) {
                    anySuccess = true;
                    if (ci.qty > 1) userInv.updateStock(ci.item->getName(), -(ci.qty - 1));
                    
                    lastItem = ci.item->getName();
                    lastAmt  = itemTotal;
                    lastOk   = true;
                    lastPay  = pname;
                    
                    logger.record("PURCHASE", lastItem, itemTotal, pname, "Success");
                    finalReceipt.items.push_back({ci.item->getName(), ci.qty, itemTotal});
                    finalReceipt.totalAmount += itemTotal;
                } else {
                    fail("Failed to dispense: " + ci.item->getName());
                }
            }

            if (anySuccess) {
                printReceipt(finalReceipt);
                info("Sending consolidated receipt to " + email + "...");
                emailSvc.sendReceiptAsync(email, username, finalReceipt);
                ok("Checkout complete! All items dispensed.");
                sessionCart.clear(); // Empty cart after checkout
            }
            pause();

        } else if (ch == 3) {
            hdr("CURRENT STOCK  //  " + kt, Color::BGREEN);
            showStock(kt, userInv);
            pause();

        } else if (ch == 4) {
            hdr("REFUND  //  Command Pattern Undo", Color::YELLOW);
            if (!lastOk) { warn("No successful purchase to refund."); pause(); continue; }
            info("Last purchase: " + lastItem + "  Rs." + std::to_string((int)lastAmt));
            int conf = readInt("Confirm refund? [1=Yes / 0=No]: ", 0, 1);
            if (conf == 1) {
                RefundCommand ref(lastItem, lastAmt, userInv);
                ref.execute();
                logger.record("REFUND", lastItem, lastAmt, lastPay, "Refunded");
                lastOk = false;
                std::cout << "  " << tag(" REFUND PROCESSED ", Color::YELLOW)
                          << "  Rs." << (int)lastAmt << " will be returned.\n";
            }
            pause();

        } else {
            running = false;
        }
    }
}

// ─── ADMIN FLOW ────────────────────────────────────────────────────────
static void runAdmin(SimulationRunner& sim, CommandLogger& logger) {
    CentralRegistry* reg = CentralRegistry::getInstance();
    InventoryProxy adminInv("AuraOS_Admin", sim.sharedInventory); // shares global stock
    adminInv.adminOverride();   // Admin bypasses PIN [Proxy override]

    bool running = true;
    while (running) {
        hdr("ADMIN CONSOLE  //  AURA OS MANAGER", Color::YELLOW);
        hr();
        menuLine("1", "View All Kiosk Inventory",   "Composite + Proxy");
        menuLine("2", "Restock Item",                "Command Pattern");
        menuLine("3", "Add New Product to Catalog",  "Composite + Registry");
        menuLine("4", "Remove Product from Catalog", "Registry Management");
        menuLine("5", "View All Transactions",       "Command Logger");
        menuLine("6", "Run Diagnostics",             "Decorator Pattern");
        menuLine("7", "Switch Hardware Module",      "HW Abstraction");
        menuLine("0", "Logout", "", true);
        hr();
        int ch = readInt("Select option: ", 0, 7);

        if (ch == 1) {
            for (const std::string& kt : {"food","pharmacy","emergency"}) {
                hdr("INVENTORY  //  " + kt, Color::YELLOW);
                showStock(kt, adminInv);
            }
            pause();

        } else if (ch == 2) {
            hdr("RESTOCK ITEM  //  Select Kiosk & Item", Color::YELLOW);
            info("Kiosk type: 1=food  2=pharmacy  3=emergency");
            int ktChoice = readInt("Select kiosk type: ", 1, 3);
            std::string ktStr = (ktChoice==1?"food":(ktChoice==2?"pharmacy":"emergency"));
            
            hdr("ITEMS IN " + ktStr + " KIOSK", Color::YELLOW);
            showStock(ktStr, adminInv);

            // Get items for selection
            std::vector<std::string> kioskItems;
            for (auto& e : reg->getCatalog()) {
                if (e.kioskType == ktStr) kioskItems.push_back(e.item->getName());
            }

            if (kioskItems.empty()) {
                warn("No items found in this kiosk to restock.");
                pause(); continue;
            }

            int ic = readInt("Select item number (1-" + std::to_string(kioskItems.size()) + "): ", 1, (int)kioskItems.size());
            std::string iname = kioskItems[ic-1];

            int qty = readInt("Quantity to add (0-500): ", 0, 500);
            if (qty > 0) {
                RestockCommand rc(iname, qty, adminInv);
                rc.execute();
                logger.record("RESTOCK", iname, 0, "-", "Done");
                ok("Restocked " + iname + " +" + std::to_string(qty) + " units.");
            } else {
                warn("Restock cancelled (quantity 0).");
            }
            pause();

        } else if (ch == 3) {
            hdr("ADD NEW PRODUCT  //  Composite + Registry", Color::YELLOW);
            std::string iname = readStr("Product name: ");
            int price = readInt("Price (Rs.): ", 1, 9999);
            int qty   = readInt("Initial stock quantity: ", 1, 999);
            info("Kiosk type: 1=food  2=pharmacy  3=emergency");
            int kt = readInt("Assign to kiosk type: ", 1, 3);
            std::string ktStr = (kt==1?"food":(kt==2?"pharmacy":"emergency"));
            auto newProd = std::make_shared<Product>(iname, (double)price, qty);
            reg->addToCatalog(newProd, ktStr);
            adminInv.addNewItem(iname, qty);
            ok("Product '" + iname + "' added to " + ktStr + " catalog with stock " + std::to_string(qty) + ".");
            pause();

        } else if (ch == 4) {
            hdr("REMOVE PRODUCT  //  Registry Management", Color::YELLOW);
            std::string iname = readStr("Product name to remove: ");
            info("Kiosk type: 1=food  2=pharmacy  3=emergency");
            int kt = readInt("From kiosk type: ", 1, 3);
            std::string ktStr = (kt==1?"food":(kt==2?"pharmacy":"emergency"));
            reg->removeFromCatalog(iname, ktStr);
            adminInv.removeItem(iname);
            ok("Product '" + iname + "' removed from " + ktStr + " catalog.");
            pause();

        } else if (ch == 5) {
            hdr("ALL TRANSACTIONS  //  Command Logger", Color::YELLOW);
            auto& entries = logger.getEntries();
            if (entries.empty()) { warn("No transactions recorded."); }
            else {
                std::vector<TxnEntry> txns;
                int i = 1;
                for (auto& e : entries) {
                    std::ostringstream ss;
                    ss << "#TXN" << std::setw(3) << std::setfill('0') << i++;
                    txns.push_back({ss.str(), e.type, e.itemName,
                                    "Rs."+std::to_string((int)e.amount),
                                    e.paymentMethod, e.status});
                }
                printTransactionTable(txns);
            }
            pause();

        } else if (ch == 6) {
            hdr("SYSTEM DIAGNOSTICS  //  Decorator Pattern", Color::YELLOW);
            printDiagCard("Food Kiosk", "Metro Station - Gate 2",
                sim.spiral->getDispenserType(), {"Network: 94%","Refg: 4C"}, true);
            std::cout << "\n";
            printDiagCard("Pharmacy Kiosk", "City Hospital - Wing B",
                sim.spiral->getDispenserType(), {}, true);
            std::cout << "\n";
            printDiagCard("Emergency Kiosk", "Disaster Zone - Sector 4",
                sim.conveyor->getDispenserType(), {"Solar: 87%"}, true);
            pause();

        } else if (ch == 7) {
            hdr("HARDWARE SWAP  //  Runtime HW Abstraction", Color::YELLOW);
            menuLine("1", "Spiral Dispenser");
            menuLine("2", "Conveyor Dispenser");
            int hw = readInt("Select hardware: ", 1, 2);
            auto disp = (hw==1) ? std::static_pointer_cast<IDispenser>(sim.spiral)
                                : std::static_pointer_cast<IDispenser>(sim.conveyor);
            sim.decoratedFood->swapDispenser(disp);
            ok("Hardware swapped to: " + disp->getDispenserType());
            pause();

        } else {
            running = false;
        }
    }
}

// ─── MAIN ENTRY POINT ──────────────────────────────────────────────────
void CLIManager::run() {
    srand((unsigned)time(nullptr));
    CentralRegistry* reg = CentralRegistry::getInstance();
    CommandLogger logger;

    bool appRunning = true;
    while (appRunning) {
        // Role Selection
        hdr("AURA RETAIL OS  //  SELECT ACCESS MODE", Color::BCYAN);
        std::cout << "\n";
        menuLine("1", "Customer Access",  "Browse & Purchase");
        menuLine("2", "Admin Login",      "Manage Inventory & System");
        menuLine("0", "Shut Down System", "", true);
        std::cout << "\n";
        hr();
        int role = readInt("Select mode: ", 0, 2);

        if (role == 0) {
            appRunning = false;

        } else if (role == 1) {
            std::string uname = readStr("Enter your name: ");
            if (uname.empty()) uname = "Guest";
            
            EmailService emailSvc;
            std::string email;
            while (true) {
                email = readStr("Enter email address: ");
                if (emailSvc.validateEmail(email)) break;
                fail("Invalid email format! Please try again.");
            }

            // OTP Flow
            info("Generating OTP...");
            std::string otp = OTPManager::generateOTP(email);
            
            if (emailSvc.sendOTP(email, uname, otp)) {
                ok("OTP sent to " + email);
                bool verified = false;
                for (int i = 0; i < 3; i++) {
                    std::string inputOtp = readStr("Enter 6-digit OTP: ");
                    if (OTPManager::verifyOTP(email, inputOtp)) {
                        ok("OTP Verified! Starting session...");
                        verified = true;
                        break;
                    } else {
                        fail("Invalid or expired OTP. Attempts remaining: " + std::to_string(2 - i));
                    }
                }
                
                if (verified) {
                    info("Welcome, " + uname + "!");
                    runCustomer(sim, uname, email, logger);
                } else {
                    fail("OTP Verification failed. Access DENIED.");
                    pause();
                }
            } else {
                fail("Failed to send OTP. Please check your SMTP settings.");
                pause();
            }

        } else if (role == 2) {
            // Admin credentials
            std::string uname = readStr("Admin Username: ");
            std::string pwd   = readStr("Admin Password: ");

            const char* envUser = std::getenv("AURA_ADMIN_USER");
            const char* envPass = std::getenv("AURA_ADMIN_PASS");

            bool authSuccess = false;
            if (envUser && envPass) {
                authSuccess = (uname == envUser && pwd == envPass);
            }

            if (authSuccess) {
                ok("Admin access granted.");
                runAdmin(sim, logger);
            } else {
                fail("Invalid admin credentials. Access DENIED.");
                pause();
            }
        }
    }

    // Save on shutdown
    hdr("SAVING SYSTEM STATE  //  Persistence", Color::BCYAN);
    InventoryProxy saveInv("system", sim.sharedInventory); // reads final global stock
    saveInv.adminOverride();
    
    PersistenceManager::saveAll(saveInv, saveInv.getAllItems(), logger, logger.getEntries(),
                                sim.spiral->getDispenserType(), "System");
    ok("inventory.json    saved.");
    ok("transactions.json saved.");
    ok("config.json       saved.");

    // Final pattern summary
    printSimComplete();
    printPatternSummary({
        {"Singleton",  "CentralRegistry — one global instance",             true},
        {"Factory",    "KioskFactory — 3 kiosk types from one method",      true},
        {"Adapter",    "UPI/Card/Wallet via IPayment interface",             true},
        {"Proxy",      "InventoryProxy — PIN auth + admin override",        true},
        {"Decorator",  "Network, Refrigeration, Solar at runtime",          true},
        {"Composite",  "Product/Bundle — nested bundles + discounts",       true},
        {"Command",    "Purchase/Refund/Restock — undoable + logged",       true},
        {"Persistence","JSON save of inventory, transactions, config",       true},
    });
    std::cout << "\n";
}
