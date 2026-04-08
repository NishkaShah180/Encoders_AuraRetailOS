#pragma once
#include <iostream>
#include <string>
#include "inventory_interface.h"
#include "real_inventory.h"

class InventoryProxy : public IInventory {
    RealInventory realInventory;
    std::string currentUser;

    bool authorize() {
        std::cout << "[InventoryProxy] Validating authorization for: " << currentUser << "... OK\n";
        return true;
    }

    void logAccess(const std::string& itemName) {
        std::cout << "[InventoryProxy] Logging access to: " << itemName << " by: " << currentUser << "\n";
    }

public:
    InventoryProxy(const std::string& user) : currentUser(user) {}

    bool checkStock(const std::string& itemName) override {
        std::cout << "[InventoryProxy] Access request for: " << itemName << "\n";
        if (!authorize()) {
            std::cout << "[InventoryProxy] Access DENIED.\n";
            return false;
        }
        logAccess(itemName);
        return realInventory.checkStock(itemName);
    }

    void updateStock(const std::string& itemName, int quantity) override {
        authorize();
        realInventory.updateStock(itemName, quantity);
        std::cout << "[InventoryProxy] Stock updated: " << itemName
                  << " => " << realInventory.getStock(itemName) << " remaining\n";
    }

    int getStock(const std::string& itemName) override {
        return realInventory.getStock(itemName);
    }
};