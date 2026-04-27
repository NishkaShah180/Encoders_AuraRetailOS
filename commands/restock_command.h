#pragma once
#include <iostream>
#include <string>
#include "icommand.h"
#include "../inventory/inventory_proxy.h"

class RestockCommand : public ICommand {
    std::string itemName;
    int quantity;
    InventoryProxy& inventory;
    bool executed = false;

public:
    RestockCommand(const std::string& itemName, int quantity,
                   InventoryProxy& inventory)
        : itemName(itemName), quantity(quantity), inventory(inventory) {}

    void execute() override {
        std::cout << "\n[RestockCommand] Restocking "
                  << itemName << " by " << quantity << " units...\n";
        inventory.updateStock(itemName, quantity);
        std::cout << "[RestockCommand] Restock complete.\n";
        executed = true;
    }

    void undo() override {
        if (!executed) {
            std::cout << "[RestockCommand] Nothing to undo.\n";
            return;
        }
        inventory.updateStock(itemName, -quantity);
        std::cout << "[RestockCommand] Undo restock: "
                  << itemName << " reduced by "
                  << quantity << " units.\n";
        executed = false;
    }
};