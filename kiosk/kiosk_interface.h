#pragma once
#include <string>
#include <memory>
#include "../payment/payment_interface.h"
#include "../inventory/inventory_proxy.h"
#include "../inventory/hardware/idispenser.h"

class IKiosk {
protected:
    std::string location;
    InventoryProxy inventory;
    std::shared_ptr<IDispenser> dispenser;

public:
    IKiosk(const std::string& loc, const std::string& user, std::shared_ptr<IDispenser> disp, std::shared_ptr<RealInventory> inv)
        : location(loc), inventory(user, inv), dispenser(disp) {}

    void swapDispenser(std::shared_ptr<IDispenser> newDispenser) {
        std::cout << "[HardwareManager] Swapping dispenser: "
                  << dispenser->getDispenserType()
                  << " -> " << newDispenser->getDispenserType() << "\n";
        dispenser = newDispenser;
        std::cout << "[HardwareManager] Swap complete. Active: "
                  << dispenser->getDispenserType() << "\n";
    }

    std::shared_ptr<IDispenser> getDispenser() const { return dispenser; }
    virtual void purchaseItem(const std::string& itemName, double amount, IPayment& payment) = 0;
    virtual void runDiagnostics() = 0;
    virtual ~IKiosk() = default;
};