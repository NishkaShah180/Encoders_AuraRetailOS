#pragma once
#include <iostream>
#include <string>
#include "icommand.h"
#include "../inventory/inventory_proxy.h"

class RefundCommand : public ICommand {
    std::string itemName;
    double amount;
    InventoryProxy& inventory;
    bool executed = false;

public:
    RefundCommand(const std::string& itemName, double amount,
                  InventoryProxy& inventory)
        : itemName(itemName), amount(amount), inventory(inventory) {}

    void execute() override {
        std::cout << "\n[RefundCommand] Processing refund for: "
                  << itemName << "\n";
        inventory.updateStock(itemName, +1);
        std::cout << "[RefundCommand] Refund of Rs." << amount
                  << " processed. Stock restored.\n";
        executed = true;
    }

    void undo() override {
        if (!executed) {
            std::cout << "[RefundCommand] Nothing to undo.\n";
            return;
        }
        inventory.updateStock(itemName, -1);
        std::cout << "[RefundCommand] Undo refund: "
                  << itemName << " removed from stock again.\n";
        executed = false;
    }
};