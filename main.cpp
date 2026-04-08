#include <iostream>
#include <memory>
#include "kiosk/kiosk_factory.h"
#include "payment/upi_adapter.h"
#include "payment/card_adapter.h"
#include "payment/wallet_adapter.h"
#include "inventory/hardware/spiral_dispenser.h"
#include "inventory/hardware/conveyor_dispenser.h"

void printSeparator(const std::string& title) {
    std::cout << "\n------------------------------------------------------------\n";
    std::cout << " " << title << "\n";
    std::cout << "------------------------------------------------------------\n";
}

int main() {
    std::cout << "============================================================\n";
    std::cout << "        AURA RETAIL OS  --  PREVIEW 1 SIMULATION\n";
    std::cout << "============================================================\n";

    // PHASE 1: Factory Pattern
    printSeparator("PHASE 1: Kiosk Creation via Factory Pattern");

    auto spiral   = std::make_shared<SpiralDispenser>();
    auto conveyor = std::make_shared<ConveyorDispenser>();

    auto foodKiosk      = KioskFactory::createKiosk("food",      "Metro Station - Gate 2",   spiral);
    auto pharmacyKiosk  = KioskFactory::createKiosk("pharmacy",  "City Hospital - Wing B",   conveyor);
    auto emergencyKiosk = KioskFactory::createKiosk("emergency", "Disaster Zone - Sector 4", spiral);

    // PHASE 2: Adapter Pattern
    printSeparator("PHASE 2: Payment Providers via Adapter Pattern");

    UPIAdapter    upiPayment("nishka@upi");
    CardAdapter   cardPayment("1234567890124242");
    WalletAdapter walletPayment("PaytmWallet");

    std::cout << "[Adapters] UPI, Card, and Wallet adapters initialized.\n";
    std::cout << "[Adapters] All three expose the same IPayment interface.\n";

    // PHASE 3: Proxy Pattern
    printSeparator("PHASE 3: Secure Inventory Access via Proxy Pattern");

    std::cout << "[Proxy] All inventory access goes through InventoryProxy.\n";
    std::cout << "[Proxy] Proxy handles authorization, validation, and logging.\n";

    // PHASE 4: Simulation
    printSeparator("PHASE 4: Basic Working Simulation");

    foodKiosk->purchaseItem("Energy Bar",    30.0, upiPayment);
    pharmacyKiosk->purchaseItem("Paracetamol", 50.0, cardPayment);
    emergencyKiosk->purchaseItem("Emergency Kit", 0.0, walletPayment);

    // PHASE 5: Hardware Swap
    printSeparator("PHASE 5: Hardware Abstraction -- Runtime Dispenser Swap");

    std::cout << "[FoodKiosk] Performing another purchase before swap...\n";
    foodKiosk->purchaseItem("Water Bottle", 20.0, upiPayment);

    std::cout << "\n[HardwareManager] Initiating runtime hardware swap on FoodKiosk...\n";
    foodKiosk->swapDispenser(std::make_shared<ConveyorDispenser>());

    std::cout << "\n[FoodKiosk] Performing purchase after swap...\n";
    foodKiosk->purchaseItem("Water Bottle", 20.0, upiPayment);

    // PHASE 6: Diagnostics
    printSeparator("PHASE 6: Running Diagnostics on All Kiosks");

    foodKiosk->runDiagnostics();
    pharmacyKiosk->runDiagnostics();
    emergencyKiosk->runDiagnostics();

    return 0;
}
/* Summary
    "        PREVIEW 1 COMPLETE "
    "============================================================"
    "[OK] Factory Pattern       -- 3 kiosk types created via factory"
    "[OK] Proxy Pattern         -- Inventory access secured and logged"
    "[OK] Hardware Abstraction  -- Spiral and Conveyor dispensers, runtime swap shown\n";
    "============================================================"*/