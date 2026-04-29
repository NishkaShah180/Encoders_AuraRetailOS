#pragma once
#include <memory>
#include "../kiosk/kiosk_interface.h"
#include "../inventory/hardware/spiral_dispenser.h"
#include "../inventory/hardware/conveyor_dispenser.h"
#include "../inventory/real_inventory.h"

class SimulationRunner {
public:
    std::shared_ptr<IKiosk>            decoratedFood;
    std::shared_ptr<IKiosk>            pharmacyKiosk;
    std::shared_ptr<IKiosk>            decoratedEmergency;
    std::shared_ptr<SpiralDispenser>   spiral;
    std::shared_ptr<ConveyorDispenser> conveyor;

    // Single shared inventory — all InventoryProxy instances reference this
    // Stock changes persist across every customer and admin session
    std::shared_ptr<RealInventory>     sharedInventory;

    void initialize();
};
