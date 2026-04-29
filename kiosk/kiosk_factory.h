#pragma once
#include <iostream>
#include <memory>
#include "food_kiosk.h"
#include "pharmacy_kiosk.h"
#include "emergency_kiosk.h"

class KioskFactory {
public:
    static std::shared_ptr<IKiosk> createKiosk(const std::string& type,
                                                const std::string& location,
                                                std::shared_ptr<IDispenser> dispenser,
                                                std::shared_ptr<RealInventory> inv) {
        std::cout << "[KioskFactory] Creating kiosk of type: " << type << "\n";
        if (type == "food")
            return std::make_shared<FoodKiosk>(location, dispenser, inv);
        else if (type == "pharmacy")
            return std::make_shared<PharmacyKiosk>(location, dispenser, inv);
        else if (type == "emergency")
            return std::make_shared<EmergencyKiosk>(location, dispenser, inv);
        else {
            std::cout << "[KioskFactory] Unknown kiosk type: " << type << "\n";
            return nullptr;
        }
    }
};