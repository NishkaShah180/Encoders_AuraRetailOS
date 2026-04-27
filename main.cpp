#include <iostream>
#include <memory>
#include <string>
#include "kiosk/kiosk_factory.h"
#include "payment/upi_adapter.h"
#include "payment/card_adapter.h"
#include "payment/wallet_adapter.h"
#include "inventory/hardware/spiral_dispenser.h"
#include "inventory/hardware/conveyor_dispenser.h"
#include "inventory/product.h"
#include "inventory/bundle.h"
#include "decorator/refrigeration_decorator.h"
#include "decorator/network_decorator.h"
#include "decorator/solar_decorator.h"
#include "registry/central_registry.h"
#include "commands/purchase_command.h"
#include "commands/refund_command.h"
#include "commands/restock_command.h"
#include "commands/command_logger.h"
#include "persistence/persistence_manager.h"
#include "pretty_print.h"          // ← only new include

int main() {
    // ── Enable UTF-8 on Windows (for box-drawing chars) ──────────────
     system("chcp 65001 > nul");   // uncomment if boxes look broken
    system("mode con cols=70");   // ← forces terminal width to match box width
    AuraCLI::printBanner();

    // ─────────────────────────────────────────────────────────────────
    // PHASE 1: Singleton
    // ─────────────────────────────────────────────────────────────────
    AuraCLI::printPhase("PHASE 1: Central Registry  ──  Singleton Pattern");
    std::cout << "[CentralRegistry] Initializing system...\n";
    CentralRegistry* registry  = CentralRegistry::getInstance();
    CentralRegistry* registry2 = CentralRegistry::getInstance();
    if (registry == registry2)
        AuraCLI::ok("Singleton verified -- same instance returned.");

    // ─────────────────────────────────────────────────────────────────
    // PHASE 2: Hardware
    // ─────────────────────────────────────────────────────────────────
    AuraCLI::printPhase("PHASE 2: Hardware Abstraction  ──  Dispenser Setup");
    auto spiral   = std::make_shared<SpiralDispenser>();
    auto conveyor = std::make_shared<ConveyorDispenser>();
    registry->setDispenser(spiral);
    AuraCLI::info("Active dispenser: " + spiral->getDispenserType());

    // ─────────────────────────────────────────────────────────────────
    // PHASE 3: Factory
    // ─────────────────────────────────────────────────────────────────
    AuraCLI::printPhase("PHASE 3: Kiosk Creation  ──  Factory Pattern");
    auto foodKiosk      = KioskFactory::createKiosk("food",
                          "Metro Station - Gate 2",   spiral);
    auto pharmacyKiosk  = KioskFactory::createKiosk("pharmacy",
                          "City Hospital - Wing B",   spiral);
    auto emergencyKiosk = KioskFactory::createKiosk("emergency",
                          "Disaster Zone - Sector 4", conveyor);

    {
        std::vector<AuraCLI::TableCol> cols = {
            {"Kiosk",    16}, {"Location", 26}, {"Hardware", 18}
        };
        AuraCLI::printTableHeader(cols);
        AuraCLI::printTableRow(cols, {"FoodKiosk",
            "Metro Station - Gate 2",   spiral->getDispenserType()});
        AuraCLI::printTableRow(cols, {"PharmacyKiosk",
            "City Hospital - Wing B",   spiral->getDispenserType()});
        AuraCLI::printTableRow(cols, {"EmergencyKiosk",
            "Disaster Zone - Sector 4", conveyor->getDispenserType()});
        AuraCLI::printTableBot(cols);
    }

    // ─────────────────────────────────────────────────────────────────
    // PHASE 4: Decorators
    // ─────────────────────────────────────────────────────────────────
    AuraCLI::printPhase("PHASE 4: Optional Modules  ──  Decorator Pattern");
    auto decoratedFood      = std::make_shared<RefrigerationDecorator>(
                              std::make_shared<NetworkDecorator>(foodKiosk));
    auto decoratedEmergency = std::make_shared<SolarDecorator>(emergencyKiosk);

    AuraCLI::ok("NetworkDecorator       attached → Signal: 94%");
    AuraCLI::ok("RefrigerationDecorator attached → Temp: 4°C");
    AuraCLI::ok("SolarDecorator         attached → Battery: 87%");

    // ─────────────────────────────────────────────────────────────────
    // PHASE 5: Composite inventory
    // ─────────────────────────────────────────────────────────────────
    AuraCLI::printPhase("PHASE 5: Inventory Setup  ──  Composite Pattern");

    auto waterBottle = std::make_shared<Product>("Water Bottle", 20.0,  15);
    auto energyBar   = std::make_shared<Product>("Energy Bar",   30.0,  10);
    auto paracetamol = std::make_shared<Product>("Paracetamol",  50.0,  8);
    auto bandage     = std::make_shared<Product>("Bandage",      15.0,  20);

    auto mealKit = std::make_shared<Bundle>("Meal Kit", 10.0);
    mealKit->add(waterBottle);
    mealKit->add(energyBar);

    auto emergencyKit = std::make_shared<Bundle>("Emergency Kit", 15.0);
    emergencyKit->add(mealKit);
    emergencyKit->add(paracetamol);
    emergencyKit->add(bandage);

    AuraCLI::printSection("Single Products");
    AuraCLI::printInventoryTable(
        {"Water Bottle", "Energy Bar", "Paracetamol", "Bandage"},
        {"Rs.20",  "Rs.30",  "Rs.50",  "Rs.15"},
        {"15",     "10",     "8",      "20"},
        {"Food",   "Food",   "Pharmacy","Pharmacy"}
    );

    AuraCLI::printSection("Bundles");
    AuraCLI::printInventoryTable(
        {"Meal Kit (10% off)",   "Emrg. Kit (15% off)"},  // names
        {"Rs.45",   "Rs.93.5"},                            // prices
        {"N/A",     "N/A"},                                // qtys
        {"Food",    "Emergency"}                           // types
    );

    // register in catalog
    // suppress raw registry prints during catalog registration
    {
        std::streambuf* orig = std::cout.rdbuf(nullptr);  // mute cout
        registry->addToCatalog(waterBottle,  "food");
        registry->addToCatalog(energyBar,    "food");
        registry->addToCatalog(mealKit,      "food");
        registry->addToCatalog(paracetamol,  "pharmacy");
        registry->addToCatalog(bandage,      "pharmacy");
        registry->addToCatalog(emergencyKit, "emergency");
        std::cout.rdbuf(orig);                            // restore cout
    }
    AuraCLI::ok("All items registered in Central Catalog.");

    // ─────────────────────────────────────────────────────────────────
    // PHASE 6: Payment
    // ─────────────────────────────────────────────────────────────────
    AuraCLI::printPhase("PHASE 6: Payment Providers  ──  Adapter Pattern");
    AuraCLI::printPaymentMenu();

    int paymentChoice = 0;
    std::cout << "Select payment method: ";
    std::cin >> paymentChoice;

    std::shared_ptr<IPayment> selectedPayment;
    std::string paymentName;

    if (paymentChoice == 1) {
        selectedPayment = std::make_shared<UPIAdapter>();
        paymentName = "UPI";
    } else if (paymentChoice == 2) {
        selectedPayment = std::make_shared<CardAdapter>();
        paymentName = "Card";
    } else {
        selectedPayment = std::make_shared<WalletAdapter>();
        paymentName = "Wallet";
    }
    AuraCLI::ok(paymentName + " adapter selected. All use IPayment interface.");
    registry->setPayment(selectedPayment);

    // ─────────────────────────────────────────────────────────────────
    // PHASE 7: Purchase Simulation
    // ─────────────────────────────────────────────────────────────────
    AuraCLI::printPhase("PHASE 7: Purchase Simulation  ──  Proxy + Command");

    {
        std::vector<AuraCLI::TableCol> cols = {
            {"#", 2}, {"Kiosk", 14}, {"Location", 28}
        };
        AuraCLI::printTableHeader(cols);
        AuraCLI::printTableRow(cols, {"1", "FoodKiosk",
                                           "Metro Station - Gate 2"});
        AuraCLI::printTableRow(cols, {"2", "PharmacyKiosk",
                                           "City Hospital - Wing B"});
        AuraCLI::printTableRow(cols, {"3", "EmergencyKiosk",
                                           "Disaster Zone - Sector 4"});
        AuraCLI::printTableBot(cols);
    }

    int kioskChoice = 0;
    std::cout << "Select kiosk: ";
    std::cin >> kioskChoice;

    std::shared_ptr<IKiosk> activeKiosk;
    std::string kioskType;

    if (kioskChoice == 1) {
        activeKiosk = decoratedFood;
        kioskType   = "food";
    } else if (kioskChoice == 2) {
        activeKiosk = pharmacyKiosk;
        kioskType   = "pharmacy";
    } else {
        activeKiosk = decoratedEmergency;
        kioskType   = "emergency";
    }

    auto selectedItem = registry->selectItemFromCatalog(kioskType);
    if (!selectedItem) {
        AuraCLI::warn("No item selected. Exiting.");
        return 0;
    }

    CommandLogger logger;
    InventoryProxy sharedInventory("SimulationUser");

    PurchaseCommand purchase(
        selectedItem->getName(),
        selectedItem->getPrice(),
        *selectedPayment,
        sharedInventory,
        *spiral
    );

    purchase.execute();
    bool purchaseSucceeded = purchase.isExecuted();
    logger.record("PURCHASE", selectedItem->getName(),
                  selectedItem->getPrice(), paymentName,
                  purchaseSucceeded ? "Success" : "Failed");

    if (purchaseSucceeded)
        AuraCLI::ok("Purchase of " + selectedItem->getName() + " complete.");
    else
        AuraCLI::fail("Purchase failed.");

    // ─────────────────────────────────────────────────────────────────
    // PHASE 8: Refund
    // ─────────────────────────────────────────────────────────────────
    AuraCLI::printPhase("PHASE 8: Refund  ──  Command Pattern Undo");
    if (!purchaseSucceeded) {
        AuraCLI::warn("No successful purchase to refund.");
    } else {
        std::cout << "Would you like to refund? (1=Yes / 0=No): ";
        int refundChoice = 0;
        std::cin >> refundChoice;
        if (refundChoice == 1) {
            RefundCommand refund(selectedItem->getName(),
                                 selectedItem->getPrice(),
                                 sharedInventory);
            refund.execute();
            logger.record("REFUND", selectedItem->getName(),
                          selectedItem->getPrice(),
                          paymentName, "Refunded");
            AuraCLI::ok("Refund of Rs." +
                        std::to_string((int)selectedItem->getPrice()) +
                        " processed. Stock restored.");
        }
    }

    // ─────────────────────────────────────────────────────────────────
    // PHASE 9: Restock
    // ─────────────────────────────────────────────────────────────────
    AuraCLI::printPhase("PHASE 9: Restock  ──  Command Pattern");
    std::string restockItem;
    std::cout << "Restock item name (or 0 to skip): ";
    std::cin.ignore();
    std::getline(std::cin, restockItem);

    if (restockItem != "0") {
        int restockQty = 0;
        std::cout << "Enter quantity to restock: ";
        std::cin >> restockQty;
        RestockCommand restock(restockItem, restockQty, sharedInventory);
        restock.execute();
        logger.record("RESTOCK", restockItem, 0.0, "-", "Done");
        AuraCLI::ok("Restocked " + restockItem +
                    " +" + std::to_string(restockQty) + " units.");
    } else {
        AuraCLI::info("Restock skipped.");
    }

    // ─────────────────────────────────────────────────────────────────
    // PHASE 10: Hardware Swap
    // ─────────────────────────────────────────────────────────────────
    AuraCLI::printPhase("PHASE 10: Runtime Hardware Swap  ──  FoodKiosk");
    AuraCLI::info("FoodKiosk current: " + spiral->getDispenserType());
    AuraCLI::arrow("Swapping to Conveyor Dispenser...");
    foodKiosk->swapDispenser(conveyor);
    AuraCLI::ok("Swap complete → Active: " + conveyor->getDispenserType());
    AuraCLI::info("Running test purchase on FoodKiosk with new hardware...");

    auto foodItem = registry->selectItemFromCatalog("food");
    if (foodItem) {
        PurchaseCommand testPurchase(
            foodItem->getName(),
            foodItem->getPrice(),
            *selectedPayment,
            sharedInventory,
            *conveyor
        );
        testPurchase.execute();
        logger.record("PURCHASE", foodItem->getName(),
                      foodItem->getPrice(), paymentName,
                      testPurchase.isExecuted() ? "Success" : "Failed");

        if (testPurchase.isExecuted())
            AuraCLI::ok("Test purchase of " + foodItem->getName() +
                        " via Conveyor successful.");
        else
            AuraCLI::fail("Test purchase failed.");
    }

    // ─────────────────────────────────────────────────────────────────
    // PHASE 11: Diagnostics
    // ─────────────────────────────────────────────────────────────────
    AuraCLI::printPhase("PHASE 11: Diagnostics  ──  All Kiosks");

    AuraCLI::printDiagCard("FoodKiosk",
        "Metro Station - Gate 2",
        conveyor->getDispenserType(),
        {"Network   : Connected | Signal 94%",
         "Refrigeration : Active | Temp 4°C"},
        true);

    std::cout << "\n";

    AuraCLI::printDiagCard("PharmacyKiosk",
        "City Hospital - Wing B",
        spiral->getDispenserType(),
        {},
        true);

    std::cout << "\n";

    AuraCLI::printDiagCard("EmergencyKiosk",
        "Disaster Zone - Sector 4",
        conveyor->getDispenserType(),
        {"Solar Power : Active | Battery 87%"},
        true);

    // ─────────────────────────────────────────────────────────────────
    // Transaction Log
    // ─────────────────────────────────────────────────────────────────
    AuraCLI::printPhase("TRANSACTION LOG");
    auto entries = logger.getEntries();
    std::vector<AuraCLI::TxnEntry> txns;
    int idx = 1;
    for (auto& e : entries) {
        std::ostringstream idss;
        idss << "#TXN" << std::setw(3) << std::setfill('0') << idx++;
        txns.push_back({
            idss.str(),
            e.type,
            e.itemName,
            "Rs." + std::to_string((int)e.amount),
            e.paymentMethod,
            e.status
        });
    }
    AuraCLI::printTransactionTable(txns);

    // ─────────────────────────────────────────────────────────────────
    // Persistence
    // ─────────────────────────────────────────────────────────────────
    AuraCLI::printPhase("PERSISTENCE  ──  Saving System State");
    std::vector<std::string> itemNames = {
        "Water Bottle", "Energy Bar",
        "Paracetamol",  "Bandage",
        "Meal Kit",     "Emergency Kit"
    };
    PersistenceManager::saveAll(
        sharedInventory, itemNames,
        logger, logger.getEntries(),
        conveyor->getDispenserType(),
        "Metro Station - Gate 2"
    );
    AuraCLI::ok("inventory.json    saved.");
    AuraCLI::ok("transactions.json saved.");
    AuraCLI::ok("config.json       saved.");

    // ─────────────────────────────────────────────────────────────────
    // Final Summary
    // ─────────────────────────────────────────────────────────────────
    AuraCLI::printSimComplete();
   AuraCLI::printPatternSummary({
        {"Singleton",  "CentralRegistry - single instance enforced",  true},
        {"Factory",    "3 kiosk types created via KioskFactory",      true},
        {"Adapter",    "UPI / Card / Wallet unified via IPayment",    true},
        {"Proxy",      "PIN-secured inventory + session auth",        true},
        {"Decorator",  "Refrigeration, Network, Solar modules",       true},
        {"Composite",  "Products -> Bundles -> Nested Bundles",       true},  // → to ->
        {"Command",    "Purchase, Refund, Restock + undo + log",      true},
        {"Persistence","Saved to inventory/transactions/config",      true},
    });
    std::cout << "\n";

    return 0;
}