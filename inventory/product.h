#pragma once
#include <iostream>
#include "inventory_item.h"

class Product : public IInventoryItem {
    std::string name;
    double price;
    int quantity;

public:
    Product(const std::string& name, double price, int quantity)
        : name(name), price(price), quantity(quantity) {}

    std::string getName() override { return name; }
    double getPrice() override { return price; }
    int getQuantity() override { return quantity; }
    std::string getItemType() const override { return "Product"; }

    void display() override {
        std::cout << "    [Product] " << name
                  << " | Price: Rs." << price
                  << " | Qty: " << quantity << "\n";
    }
};