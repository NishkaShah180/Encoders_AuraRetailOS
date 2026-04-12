# 🌟 Aura Retail OS

### Smart Retail Kiosk Simulation using C++ & Design Patterns

Aura Retail OS is a modular C++ simulation of an intelligent retail kiosk system deployed across a smart-city environment.
The project demonstrates how classic Object-Oriented Design Patterns can be applied to build scalable, flexible, and maintainable systems.
> A structured simulation showcasing how design patterns solve real-world system design problems.
---

## 🚀 Quick Start

### 🔧 Prerequisites

| Tool         | Version                             |
| ------------ | ----------------------------------- |
| C++ Compiler | GCC / MSVC / Clang (C++17 or later) |

---

### ▶️ Build & Run

#### 🔹 Compile (GCC example)

```bash
g++ main.cpp -o main
```

#### 🔹 Run

```bash
./main        # Linux / macOS
main.exe      # Windows
```

---

## ✨ Feature Overview

| # | Feature                    | Description                                                        |
| - | -------------------------- | ------------------------------------------------------------------ |
| 1 | Multi-type Kiosks          | Food, Pharmacy, and Emergency kiosks created using Factory Pattern |
| 2 | Factory-based Creation     | Kiosks are dynamically created using a centralized factory         |
| 3 | Secure Proxy Layer         | Inventory access is controlled, validated, and logged via Proxy    |
| 4 | Adapter-based Payments     | UPI, Card, and Wallet systems unified through adapters             |
| 5 | Runtime Hardware Swap      | Dispenser hardware can be changed dynamically at runtime           |
| 6 | Inventory Management       | Stock is updated after each successful transaction                 |
| 7 | Structured Simulation Flow | Execution divided into clear phases for better understanding       |
| 8 | Diagnostics System         | Each kiosk reports its status, hardware, and location              |


---

## 🏗️ Project Structure

```id="finalstruct"
Encoders_AuraRetailOS/
│
├── inventory/                      # Inventory system (Proxy Pattern)
│   ├── hardware/                  # Dispenser implementations
│   │   ├── conveyor_dispenser.h   # Conveyor-based dispensing
│   │   ├── idispenser.h           # Dispenser interface
│   │   └── spiral_dispenser.h     # Spiral-based dispensing
│   │
│   ├── inventory_interface.h      # Inventory abstraction
│   ├── inventory_proxy.h          # Proxy for secure access
│   └── real_inventory.h           # Actual inventory implementation
│
├── kiosk/                         # Kiosk system (Factory Pattern)
│   ├── emergency_kiosk.h          # Emergency kiosk logic
│   ├── food_kiosk.h               # Food kiosk logic
│   ├── kiosk_factory.h            # Factory for kiosk creation
│   ├── kiosk_interface.h          # Base kiosk interface
│   └── pharmacy_kiosk.h           # Pharmacy kiosk logic
│
├── payment/                       # Payment system (Adapter Pattern)
│   ├── card_adapter.h             # Card payment adapter
│   ├── payment_interface.h        # Payment interface
│   ├── upi_adapter.h              # UPI adapter
│   └── wallet_adapter.h           # Wallet adapter
│
├── main.cpp                       # Entry point (simulation driver)
├── main.exe                       # Compiled executable
├── ARS.exe                        # Additional executable
```

---

## 🎭 Design Patterns Used

| Pattern                              | Location                                                                                    | Role                                                                      |
| ------------------------------------ | ------------------------------------------------------------------------------------------- | ------------------------------------------------------------------------- |
| Factory                              | `kiosk/kiosk_factory.h`                                                                     | Creates appropriate kiosk type (Food, Pharmacy, Emergency) based on input |
| Adapter                              | `payment/upi_adapter.h`, `payment/card_adapter.h`, `payment/wallet_adapter.h`               | Provides a unified payment interface for different payment systems        |
| Proxy                                | `inventory/inventory_proxy.h` → `inventory/real_inventory.h`                                | Controls access to inventory with validation, logging, and stock updates  |
| Abstraction (Interface-based Design) | `kiosk/kiosk_interface.h`, `payment/payment_interface.h`, `inventory/inventory_interface.h` | Defines common interfaces for flexibility and loose coupling              |

---

## 🔄 Transaction Lifecycle

Each purchase in the system follows a structured sequence of steps:

```
1. Item Request
   → Customer selects an item at the kiosk

2. Inventory Access (Proxy)
   → InventoryProxy validates authorization
   → Logs access request

3. Payment Processing (Adapter)
   → Selected adapter (UPI / Card / Wallet) processes payment
   → Provides a unified interface to different payment systems

4. Dispensing Item (Hardware)
   → Dispenser (Spiral / Conveyor) releases the item

5. Stock Update
   → InventoryProxy updates remaining stock in RealInventory

6. Confirmation
   → Transaction success message displayed to user
```

---

## 🔒 Constraint Scenarios Demonstrated

| Scenario                    | What Happens                                                                                    |
| --------------------------- | ----------------------------------------------------------------------------------------------- |
| Controlled Inventory Access | All item requests go through `InventoryProxy`, ensuring authorization and logging before access |
| Authorized Transactions     | Each kiosk interacts with inventory using validated user roles (e.g., FoodKiosk_User)           |
| Unified Payment Handling    | Different payment systems (UPI, Card, Wallet) are accessed through a common interface           |
| Hardware Abstraction        | Dispenser type (Spiral / Conveyor) can be swapped at runtime without affecting logic            |
| Structured Execution Flow   | Operations follow a fixed sequence ensuring consistency across all kiosks                       |

---

## 🧩 Extending the System

| Goal                      | How                                                                                      |
| ------------------------- | ---------------------------------------------------------------------------------------- |
| Add a new kiosk type      | Create a new class implementing `kiosk_interface.h` and register it in `kiosk_factory.h` |
| Add a new payment method  | Create a new adapter implementing `payment_interface.h`                                  |
| Add a new dispenser       | Implement the `idispenser.h` interface and integrate it with kiosks                      |
| Modify inventory behavior | Update logic inside `inventory_proxy.h` or `real_inventory.h`                            |

---

## 👥 Group Information

* **Group Name:** Encoders
* **Course:** Object Oriented Programming
* **Instructor:** Sourish Dasgupta

**Members:**

* Nishka Shah — 202512035
* Varunee Agrawal — 202512067
* Hardi Dhadhal — 202512085
* Zalak Thakkar — 202512091

---

## 📚 References

* *Design Patterns: Elements of Reusable Object-Oriented Software*
  (Gamma, Helm, Johnson, Vlissides)

* *The C++ Programming Language* — Bjarne Stroustrup

---

## 📄 License

This project was built as an academic OOP demonstration.
