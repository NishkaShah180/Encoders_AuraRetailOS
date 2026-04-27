#pragma once
#include <iostream>
#include <string>
#include "inventory_interface.h"
#include "real_inventory.h"

class InventoryProxy : public IInventory {
    RealInventory realInventory;
    std::string currentUser;
    const std::string correctPin = "1234";
    bool sessionAuthorized = false;

    bool authorize() {
        if (sessionAuthorized) {
            std::cout << "[InventoryProxy] Session already authorized. "
                    << "Access GRANTED.\n";   // ← ADD THIS LINE
            return true;
        }
        std::string enteredPin;
        std::cout << "[InventoryProxy] Authorization required for: "
                  << currentUser << "\n";
        std::cout << "[InventoryProxy] Enter access PIN: ";
        std::cin >> enteredPin;

        if (enteredPin != correctPin) {
            std::cout << "[InventoryProxy] Incorrect PIN. "
                      << "Access DENIED.\n";
            return false;
        }

        std::cout << "[InventoryProxy] PIN verified. "
                  << "Access GRANTED for this session.\n";
        sessionAuthorized = true;
        return true;
    }

    void logAccess(const std::string& itemName) {
        std::cout << "[InventoryProxy] Access logged: "
                  << itemName << " by " << currentUser << "\n";
    }

public:
    InventoryProxy(const std::string& user) : currentUser(user) {}

    void resetSession() {
        sessionAuthorized = false;
        std::cout << "[InventoryProxy] Session reset. "
                  << "Re-authorization required.\n";
    }

    bool checkStock(const std::string& itemName) override {
        std::cout << "[InventoryProxy] Access request for: "
                  << itemName << "\n";
        if (!authorize()) return false;
        logAccess(itemName);
        return realInventory.checkStock(itemName);
    }

    void updateStock(const std::string& itemName, int quantity) override {
        realInventory.updateStock(itemName, quantity);
        std::cout << "[InventoryProxy] Stock updated: "
                  << itemName << " => "
                  << realInventory.getStock(itemName)
                  << " remaining\n";
    }

    int getStock(const std::string& itemName) override {
        return realInventory.getStock(itemName);
    }
};