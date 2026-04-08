#pragma once
#include <iostream>
#include <string>
#include <map>
#include "inventory_interface.h"

class RealInventory : public IInventory {
    std::map<std::string, int> stock;
public:
    RealInventory() {
        stock["Energy Bar"]    = 10;
        stock["Water Bottle"]  = 15;
        stock["Paracetamol"]   = 8;
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
};