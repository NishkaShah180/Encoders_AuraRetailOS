#pragma once
#include <iostream>
#include "idispenser.h"

class ConveyorDispenser : public IDispenser {
public:
    void dispense(const std::string& itemName) override {
        std::cout << "[ConveyorDispenser] Starting belt mechanism...\n";
        std::cout << "[ConveyorDispenser] Dispensing: " << itemName << "\n";
    }

    std::string getDispenserType() override {
        return "Conveyor Dispenser";
    }
};