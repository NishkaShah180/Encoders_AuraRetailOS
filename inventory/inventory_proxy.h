#pragma once
#include <iostream>
#include <memory>
#include <string>
#include <vector>
#include "inventory_interface.h"
#include "real_inventory.h"

// Proxy Pattern: controls access to shared RealInventory
// All InventoryProxy instances share the SAME RealInventory via shared_ptr
class InventoryProxy : public IInventory {
    std::shared_ptr<RealInventory> realInventory;  // shared — stock persists globally
    std::string currentUser;
    const std::string correctPin = "1234";
    bool sessionAuthorized = false;

    bool authorize() {
        if (sessionAuthorized) {
            std::cout << "[InventoryProxy] Session already authorized. Access GRANTED.\n";
            return true;
        }

        int attempts = 0;
        const int maxAttempts = 3;
        std::string enteredPin;

        while (attempts < maxAttempts) {
            std::cout << "[InventoryProxy] Authorization required for: " << currentUser << "\n";
            std::cout << "[InventoryProxy] Enter access PIN (Attempt " << attempts + 1 << "/" << maxAttempts << "): ";
            std::cin >> enteredPin;
            std::cin.ignore(1000, '\n');

            if (enteredPin == correctPin) {
                std::cout << "[InventoryProxy] PIN verified. Access GRANTED for this session.\n";
                sessionAuthorized = true;
                return true;
            } else {
                attempts++;
                std::cout << "[InventoryProxy] Incorrect PIN. ";
                if (attempts < maxAttempts) {
                    std::cout << (maxAttempts - attempts) << " attempts remaining.\n";
                } else {
                    std::cout << "Out of chances. Access DENIED.\n";
                }
            }
        }
        return false;
    }

    void logAccess(const std::string& itemName) {
        std::cout << "[InventoryProxy] Access logged: " << itemName << " by " << currentUser << "\n";
    }

public:
    // Must be constructed with the shared inventory instance
    InventoryProxy(const std::string& user, std::shared_ptr<RealInventory> inv)
        : currentUser(user), realInventory(inv) {}

    // Admin bypass — skips PIN requirement [Proxy Pattern: admin override]
    void adminOverride() {
        sessionAuthorized = true;
        std::cout << "[InventoryProxy] Admin session authorized (override).\n";
    }

    void resetSession() { sessionAuthorized = false; }

    bool checkStock(const std::string& itemName) override {
        std::cout << "[InventoryProxy] Access request for: " << itemName << "\n";
        if (!authorize()) return false;
        logAccess(itemName);
        return realInventory->checkStock(itemName);
    }

    void updateStock(const std::string& itemName, int quantity) override {
        realInventory->updateStock(itemName, quantity);
        std::cout << "[InventoryProxy] Stock updated: " << itemName
                  << " => " << realInventory->getStock(itemName) << " remaining\n";
    }

    int getStock(const std::string& itemName) override {
        return realInventory->getStock(itemName);
    }

    // Admin-only: add new item directly to stock (bypasses PIN check)
    void addNewItem(const std::string& name, int qty) {
        realInventory->addItem(name, qty);
    }

    void removeItem(const std::string& name) {
        realInventory->removeItem(name);
    }

    std::vector<std::string> getAllItems() {
        return realInventory->getAllItems();
    }
};