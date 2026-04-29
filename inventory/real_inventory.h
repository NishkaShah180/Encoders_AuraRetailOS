#pragma once
#include <iostream>
#include <string>
#include <map>
#include <vector>
#include "inventory_interface.h"

class RealInventory : public IInventory {
    std::map<std::string, int> stock;
public:
    RealInventory() {
        // Food items
        stock["Water Bottle"]  = 15;
        stock["Energy Bar"]    = 10;
        stock["Sandwich"]      = 12;
        stock["Fruit Juice"]   = 15;
        stock["Chips"]         = 20;
        stock["Meal Kit"]      = 6;

        // Pharmacy items
        stock["Paracetamol"]   = 8;
        stock["Bandage"]       = 20;
        stock["Antiseptic"]    = 10;
        stock["Cough Syrup"]   = 5;
        stock["Face Mask"]     = 50;

        // Emergency items
        stock["Flashlight"]    = 8;
        stock["Power Bank"]    = 4;
        stock["First Aid Kit"] = 6;
        stock["Emergency Kit"] = 5;
    }

    bool checkStock(const std::string& itemName) override {
        return stock.count(itemName) && stock[itemName] > 0;
    }
    void updateStock(const std::string& itemName, int quantity) override {
        stock[itemName] += quantity;
    }
    int getStock(const std::string& itemName) override {
        return stock.count(itemName) ? stock[itemName] : 0;
    }
    void addItem(const std::string& name, int qty) {
        stock[name] = qty;
    }
    void removeItem(const std::string& name) {
        stock.erase(name);
    }
    std::vector<std::string> getAllItems() {
        std::vector<std::string> names;
        for (std::map<std::string, int>::iterator it = stock.begin(); it != stock.end(); ++it) {
            names.push_back(it->first);
        }
        return names;
    }
};