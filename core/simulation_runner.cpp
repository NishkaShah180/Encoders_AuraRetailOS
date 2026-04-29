#include "simulation_runner.h"
#include <iostream>
#include "../registry/central_registry.h"
#include "../kiosk/kiosk_factory.h"
#include "../decorator/refrigeration_decorator.h"
#include "../decorator/network_decorator.h"
#include "../decorator/solar_decorator.h"
#include "../inventory/product.h"
#include "../inventory/bundle.h"
#include "../persistence/persistence_manager.h"
#include "../pretty_print.h"

void SimulationRunner::initialize() {
    AuraCLI::printBootSection("Singleton Registry Init");
    std::cout.flush();
    CentralRegistry* registry = CentralRegistry::getInstance();
    AuraCLI::printBootStep("CentralRegistry instantiated           [Singleton Pattern]");
    CentralRegistry* reg2 = CentralRegistry::getInstance();
    if (registry == reg2)
        AuraCLI::printBootStep("Singleton verified — same pointer returned");

    // Shared inventory — allocated ONCE, referenced by every InventoryProxy
    sharedInventory = std::make_shared<RealInventory>();
    AuraCLI::printBootStep("Shared RealInventory  → allocated (stock persists across all sessions)");

    // Load from disk if exists
    PersistenceManager::loadInventory(*sharedInventory);
    AuraCLI::printBootStep("PersistenceManager    → inventory.json checked and loaded");

    AuraCLI::printBootSection("Hardware Interface Layer");
    spiral   = std::make_shared<SpiralDispenser>();
    conveyor = std::make_shared<ConveyorDispenser>();
    registry->setDispenser(spiral);
    AuraCLI::printBootStep("Spiral Dispenser      → registered as default hardware");
    AuraCLI::printBootStep("Conveyor Dispenser    → standby unit loaded");

    AuraCLI::printBootSection("Kiosk Factory [Factory Pattern]");
    auto foodKiosk      = KioskFactory::createKiosk("food",      "Metro Station - Gate 2",   spiral, sharedInventory);
    auto pharmacyKiosk_ = KioskFactory::createKiosk("pharmacy",  "City Hospital - Wing B",   spiral, sharedInventory);
    auto emergencyKiosk = KioskFactory::createKiosk("emergency", "Disaster Zone - Sector 4", conveyor, sharedInventory);
    pharmacyKiosk = pharmacyKiosk_;
    AuraCLI::printBootStep("FoodKiosk             → Metro Station - Gate 2");
    AuraCLI::printBootStep("PharmacyKiosk         → City Hospital - Wing B");
    AuraCLI::printBootStep("EmergencyKiosk        → Disaster Zone - Sector 4");

    AuraCLI::printBootSection("Decorator Module Attach [Decorator Pattern]");
    decoratedFood      = std::make_shared<RefrigerationDecorator>(
                         std::make_shared<NetworkDecorator>(foodKiosk, sharedInventory), sharedInventory);
    decoratedEmergency = std::make_shared<SolarDecorator>(emergencyKiosk, sharedInventory);
    AuraCLI::printBootStep("NetworkDecorator      → FoodKiosk      [Signal 94%]");
    AuraCLI::printBootStep("RefrigerationDecorator→ FoodKiosk      [Temp 4 C]");
    AuraCLI::printBootStep("SolarDecorator        → EmergencyKiosk [Battery 87%]");

    AuraCLI::printBootSection("Inventory Catalog Sync [Composite Pattern]");
    // Food
    auto waterBottle = std::make_shared<Product>("Water Bottle", 20.0, 15);
    auto energyBar   = std::make_shared<Product>("Energy Bar",   30.0, 10);
    auto sandwich    = std::make_shared<Product>("Sandwich",     50.0, 12);
    auto fruitJuice  = std::make_shared<Product>("Fruit Juice",  40.0, 15);
    auto chips       = std::make_shared<Product>("Chips",        25.0, 20);

    // Pharmacy
    auto paracetamol = std::make_shared<Product>("Paracetamol",  50.0,  8);
    auto bandage     = std::make_shared<Product>("Bandage",      15.0, 20);
    auto antiseptic  = std::make_shared<Product>("Antiseptic",   60.0, 10);
    auto coughSyrup  = std::make_shared<Product>("Cough Syrup",  80.0,  5);
    auto faceMask    = std::make_shared<Product>("Face Mask",    10.0, 50);

    // Emergency
    auto flashlight  = std::make_shared<Product>("Flashlight",   150.0,  8);
    auto powerBank   = std::make_shared<Product>("Power Bank",   500.0,  4);
    auto firstAidKit = std::make_shared<Product>("First Aid Kit",250.0,  6);

    // Bundles (Composite Pattern)
    auto mealKit = std::make_shared<Bundle>("Meal Kit", 10.0);
    mealKit->add(waterBottle);
    mealKit->add(energyBar);

    auto emergencyKit = std::make_shared<Bundle>("Emergency Kit", 15.0);
    emergencyKit->add(mealKit);
    emergencyKit->add(paracetamol);
    emergencyKit->add(bandage);

    {
        // Silence internal logs during boot
        std::streambuf* orig = std::cout.rdbuf(nullptr);
        registry->addToCatalog(waterBottle,  "food");
        registry->addToCatalog(energyBar,    "food");
        registry->addToCatalog(sandwich,     "food");
        registry->addToCatalog(fruitJuice,   "food");
        registry->addToCatalog(chips,        "food");
        registry->addToCatalog(mealKit,      "food");

        registry->addToCatalog(paracetamol,  "pharmacy");
        registry->addToCatalog(bandage,      "pharmacy");
        registry->addToCatalog(antiseptic,   "pharmacy");
        registry->addToCatalog(coughSyrup,   "pharmacy");
        registry->addToCatalog(faceMask,     "pharmacy");

        registry->addToCatalog(flashlight,   "emergency");
        registry->addToCatalog(powerBank,    "emergency");
        registry->addToCatalog(firstAidKit,  "emergency");
        registry->addToCatalog(emergencyKit, "emergency");
        std::cout.rdbuf(orig);
    }
    AuraCLI::printBootStep("Food Catalog      → 6 items registered  [Composite]");
    AuraCLI::printBootStep("Pharmacy Catalog  → 5 items registered");
    AuraCLI::printBootStep("Emergency Catalog → 4 items registered  [Nested Composite]");

    AuraCLI::printBootSection("System Ready");
    AuraCLI::printBootStep("Payment Gateway       → Standby (Adapter Pattern)");
    AuraCLI::printBootStep("Command Engine        → Loaded (Command Pattern)");
    AuraCLI::printBootStep("Inventory Proxy       → Armed (Proxy Pattern)");
    AuraCLI::printBootStep("Persistence Manager   → Initialized");

    std::cout << "\n" << AuraCLI::Color::BGREEN << AuraCLI::Color::BOLD
              << "  >> AURA RETAIL OS READY  --  ALL SYSTEMS NOMINAL\n"
              << AuraCLI::Color::RESET << "\n";
}
