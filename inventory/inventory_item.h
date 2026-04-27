#pragma once
#include <string>

class IInventoryItem {
public:
    virtual std::string getName() = 0;
    virtual double getPrice() = 0;
    virtual int getQuantity() = 0;
    virtual void display() = 0;
    virtual ~IInventoryItem() = default;
};