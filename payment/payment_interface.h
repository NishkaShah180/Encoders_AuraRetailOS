#pragma once
#include <iostream>

class IPayment {
public:
    virtual void pay(double amount) = 0;
    virtual ~IPayment() = default;
};