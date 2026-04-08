#pragma once
#include <iostream>
#include <memory>
#include "food_kiosk.h"
#include "pharmacy_kiosk.h"
#include "emergency_kiosk.h"

class KioskFactory {
public:
    static std::unique_ptr<IKiosk> createKiosk(const std::string& type,
                                                const std::string& location,
                                                std::shared_ptr<IDispenser> dispenser) {
        std::cout << "[KioskFactory] Creating kiosk of type: " << type << "\n";
        if (type == "food")
            return std::make_unique<FoodKiosk>(location, dispenser);
        else if (type == "pharmacy")
            return std::make_unique<PharmacyKiosk>(location, dispenser);
        else if (type == "emergency")
            return std::make_unique<EmergencyKiosk>(location, dispenser);
        else {
            std::cout << "[KioskFactory] Unknown kiosk type: " << type << "\n";
            return nullptr;
        }
    }
};