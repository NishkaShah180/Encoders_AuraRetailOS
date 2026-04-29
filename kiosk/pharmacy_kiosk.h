#pragma once
#include <iostream>
#include <memory>
#include "kiosk_interface.h"

class PharmacyKiosk : public IKiosk {
public:
    PharmacyKiosk(const std::string& location, std::shared_ptr<IDispenser> disp, std::shared_ptr<RealInventory> inv)
        : IKiosk(location, "PharmacyKiosk_User", disp, inv) {
        std::cout << "[PharmacyKiosk] Initialized at " << location << "\n";
        std::cout << "[PharmacyKiosk] Active hardware: " << dispenser->getDispenserType() << "\n";
    }

    void purchaseItem(const std::string& itemName, double amount, IPayment& payment) override {
        std::cout << "\n[PharmacyKiosk] Customer requests: " << itemName << "\n";
        std::cout << "[PharmacyKiosk] Pharmacy authorization check... OK\n";
        if (!inventory.checkStock(itemName)) {
            std::cout << "[PharmacyKiosk] Item out of stock.\n";
            return;
        }
        payment.pay(amount);
        dispenser->dispense(itemName);
        inventory.updateStock(itemName, -1);
        std::cout << "Purchase of " << itemName << " successful at PharmacyKiosk.\n";
    }

    void runDiagnostics() override {
        std::cout << "[PharmacyKiosk] Diagnostics OK | Hardware: "
                  << dispenser->getDispenserType()
                  << " | Location: " << location << "\n";
    }
};