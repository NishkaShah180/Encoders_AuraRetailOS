#pragma once
#include <string>

class IInventory {
public:
    virtual bool checkStock(const std::string& itemName) = 0;
    virtual void updateStock(const std::string& itemName, int quantity) = 0;
    virtual int getStock(const std::string& itemName) = 0;
    virtual ~IInventory() = default;
};