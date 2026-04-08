#pragma once
#include <iostream>
#include <string>

class IDispenser {
public:
    virtual void dispense(const std::string& itemName) = 0;
    virtual std::string getDispenserType() = 0;
    virtual ~IDispenser() = default;
};