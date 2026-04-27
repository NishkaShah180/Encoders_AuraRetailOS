#pragma once
#include <iostream>
class IPayment {
public:
    virtual bool pay(double amount) = 0;
    virtual ~IPayment() = default;
};