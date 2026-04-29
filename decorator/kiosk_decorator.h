#pragma once
#include <iostream>
#include <memory>
#include "../kiosk/kiosk_interface.h"
#include "../inventory/hardware/spiral_dispenser.h"

class KioskDecorator : public IKiosk {
protected:
    std::shared_ptr<IKiosk> wrappedKiosk;

public:
    KioskDecorator(std::shared_ptr<IKiosk> kiosk, std::shared_ptr<RealInventory> inv)
        : IKiosk("decorated", "Decorator_User",
                 std::make_shared<SpiralDispenser>(), inv),
          wrappedKiosk(kiosk) {}

    void purchaseItem(const std::string& itemName,
                      double amount, IPayment& payment) override {
        wrappedKiosk->purchaseItem(itemName, amount, payment);
    }

    void runDiagnostics() override {
        wrappedKiosk->runDiagnostics();
    }

    void swapDispenser(std::shared_ptr<IDispenser> d) {
        wrappedKiosk->swapDispenser(d);
    }
};