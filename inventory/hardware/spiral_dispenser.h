#pragma once
#include <iostream>
#include "idispenser.h"

class SpiralDispenser : public IDispenser {
public:
    void dispense(const std::string& itemName) override {
        std::cout << "[SpiralDispenser] Rotating coil mechanism...\n";
        std::cout << "[SpiralDispenser] Dispensing: " << itemName << "\n";
    }

    std::string getDispenserType() override {
        return "Spiral Dispenser";
    }
};