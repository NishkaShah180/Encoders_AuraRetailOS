#pragma once
#include <iostream>
#include <string>
#include <memory>
#include "icommand.h"
#include "../payment/payment_interface.h"
#include "../inventory/inventory_proxy.h"
#include "../inventory/hardware/idispenser.h"

class PurchaseCommand : public ICommand {
    std::string itemName;
    double amount;
    IPayment& payment;
    InventoryProxy& inventory;
    IDispenser& dispenser;
    bool executed = false;

public:
    PurchaseCommand(const std::string& itemName, double amount,
                    IPayment& payment, InventoryProxy& inventory,
                    IDispenser& dispenser)
        : itemName(itemName), amount(amount),
          payment(payment), inventory(inventory),
          dispenser(dispenser) {}

    void execute() override {
        std::cout << "\n[PurchaseCommand] Executing purchase...\n";

        if (!inventory.checkStock(itemName)) {
            std::cout << "[PurchaseCommand] Item out of stock. Aborting.\n";
            return;
        }

        bool paymentSuccess = payment.pay(amount);
        if (!paymentSuccess) {
            std::cout << "[PurchaseCommand] Payment failed. Aborting purchase.\n";
            return;
        }

        dispenser.dispense(itemName);
        inventory.updateStock(itemName, -1);
        std::cout << "[PurchaseCommand] Purchase of "
                << itemName << " complete.\n";
        executed = true;
    }
    bool isExecuted() const {
            return executed;
    }
    void undo() override {
        if (!executed) {
            std::cout << "[PurchaseCommand] Nothing to undo.\n";
            return;
        }
        inventory.updateStock(itemName, +1);
        std::cout << "[PurchaseCommand] Undo purchase: "
                  << itemName << " restocked. Refund of Rs."
                  << amount << " initiated.\n";
        executed = false;
    }

    std::string getItemName() { return itemName; }
    double getAmount() { return amount; }
};