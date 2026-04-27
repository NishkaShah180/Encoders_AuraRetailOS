#pragma once
#include <iostream>
#include <memory>
#include "kiosk_decorator.h"

class RefrigerationDecorator : public KioskDecorator {
    float temperature;

public:
    RefrigerationDecorator(std::shared_ptr<IKiosk> kiosk, float temp = 4.0)
        : KioskDecorator(kiosk), temperature(temp) {
        std::cout << "[RefrigerationDecorator] Module attached. "
                  << "Temperature set to " << temperature << "C\n";
    }

    void purchaseItem(const std::string& itemName,
                      double amount, IPayment& payment) override {
        std::cout << "[RefrigerationDecorator] Checking temperature... "
                  << temperature << "C OK\n";
        wrappedKiosk->purchaseItem(itemName, amount, payment);
    }

    void runDiagnostics() override {
        wrappedKiosk->runDiagnostics();
        std::cout << "[RefrigerationDecorator] Refrigeration: Active | "
                  << "Temp: " << temperature << "C\n";
    }
};