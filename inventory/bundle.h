#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include "inventory_item.h"

class Bundle : public IInventoryItem {
    std::string name;
    std::vector<std::shared_ptr<IInventoryItem>> items;
    double discount;

public:
    Bundle(const std::string& name, double discountPercent)
        : name(name), discount(discountPercent) {}

    void add(std::shared_ptr<IInventoryItem> item) {
        items.push_back(item);
    }

    std::string getName() override { return name; }

    double getPrice() override {
        double total = 0;
        for (auto& item : items)
            total += item->getPrice();
        return total * (1.0 - discount / 100.0);
    }

    int getQuantity() override { return 1; }

    void display() override {
        std::cout << "    [Bundle] " << name
                  << " | Price: Rs." << getPrice()
                  << " (" << discount << "% discount)\n";
        for (auto& item : items)
            item->display();
    }
};