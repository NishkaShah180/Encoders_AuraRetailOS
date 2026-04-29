# 🌟 Aura Retail OS

### Smart Retail Kiosk Simulation using C++ & Design Patterns

Aura Retail OS is a modular C++ simulation of an intelligent retail kiosk system deployed across a smart-city environment. The project demonstrates how classic Object-Oriented Design Patterns can be applied to build scalable, flexible, and maintainable systems.

> A structured 11-phase simulation showcasing modern Object-Oriented Design Patterns through inventory security, composite bundles, SMTP integration, runtime hardware swapping, and modular kiosk architecture.

---

## 🚀 Quick Start

### 🔧 Prerequisites

| Tool | Version |
|------|---------|
| C++ Compiler | GCC / MSVC / Clang (C++17 or later) |

### ▶️ Build & Run

```bash
# Compile
g++ main.cpp -o ARS

# Run (Windows)
./ARS.exe

# Run (Linux / macOS)
./ARS
```

---

## ✨ Feature Overview

| # | Feature | Description |
|---|---------|-------------|
| 1 | Multi-type Kiosks | Food, Pharmacy, and Emergency kiosks created via Factory Pattern |
| 2 | Secure Proxy Layer | Inventory access is PIN-controlled, session-managed, and logged |
| 3 | Adapter-based Payments | UPI, Card, and Wallet unified through a common interface |
| 4 | Runtime Hardware Swap | Dispenser swaps from Spiral to Conveyor live without touching kiosk logic |
| 5 | Composite Inventory | Products, Bundles, and Nested Bundles with cascading discounts |
| 6 | Cart System | Multiple items can be added before checkout |
| 7 | SMTP Email Integration | OTP verification and purchase receipt delivery using Gmail SMTP |
| 8 | Command Pattern | Purchase, Refund (undo), and Restock as encapsulated commands |
| 9 | Persistence | Inventory, transactions, and config saved to JSON after every run |
| 10 | Pretty CLI | Colour-coded phase boxes, tables, diagnostics cards, transaction log |

---

## 🏗️ Project Structure

```
AuraRetailOS/
│
├── main.cpp                          # Entry point — 11-phase simulation driver
├── pretty_print.h                    # CLI formatting — tables, phase boxes, colours
│
├── registry/
│   └── central_registry.h            # Singleton — global catalog + dispenser/payment store
│
├── kiosk/                            # Factory Pattern
│   ├── ikiosk.h                      # Kiosk interface
│   ├── kiosk_factory.h               # Factory — creates Food/Pharmacy/Emergency kiosks
│   ├── food_kiosk.h                  # Food kiosk logic
│   ├── pharmacy_kiosk.h              # Pharmacy kiosk logic
│   └── emergency_kiosk.h             # Emergency kiosk logic
│
├── inventory/                        # Proxy Pattern
│   ├── inventory_interface.h         # Inventory abstraction
│   ├── real_inventory.h              # Actual stock store
│   ├── inventory_proxy.h             # Proxy — PIN auth, session management, logging
│   ├── inventory_item.h              # IInventoryItem interface
│   ├── product.h                     # Leaf node (Composite Pattern)
│   ├── bundle.h                      # Composite node — holds products/bundles + discount
│   └── hardware/
│       ├── idispenser.h              # Dispenser interface
│       ├── spiral_dispenser.h        # Spiral dispenser implementation
│       └── conveyor_dispenser.h      # Conveyor dispenser implementation
│
├── payment/                          # Adapter Pattern
│   ├── payment_interface.h           # IPayment — unified interface
│   ├── upi_adapter.h                 # UPI adapter
│   ├── card_adapter.h                # Card adapter
│   └── wallet_adapter.h              # Wallet adapter
│
├── email/                            # SMTP Integration
│   ├── smtp_client.cpp               # SMTP email sending logic
│   └── email_service.h               # OTP + receipt handling
│
├── decorator/                        # Decorator Pattern
│   ├── kiosk_decorator.h             # Base decorator
│   ├── network_decorator.h           # Adds network module to kiosk
│   ├── refrigeration_decorator.h     # Adds refrigeration module
│   └── solar_decorator.h             # Adds solar power module
│
├── commands/                         # Command Pattern
│   ├── icommand.h                    # Command interface
│   ├── purchase_command.h            # Executes a purchase
│   ├── refund_command.h              # Undoes a purchase (undo)
│   ├── restock_command.h             # Restocks inventory
│   └── command_logger.h              # Logs all commands to transaction history
│
└── persistence/
    └── persistence_manager.h         # Saves inventory/transactions/config to JSON
```

---

## 🎭 Design Patterns Used

| Pattern | Location | Role |
|---------|----------|------|
| **Singleton** | `registry/central_registry.h` | One global registry instance for catalog, dispenser, and payment |
| **Factory** | `kiosk/kiosk_factory.h` | Creates Food, Pharmacy, or Emergency kiosk based on type string |
| **Adapter** | `payment/upi_adapter.h`, `card_adapter.h`, `wallet_adapter.h` | Wraps different payment systems under one `IPayment` interface |
| **Proxy** | `inventory/inventory_proxy.h` | PIN-secured, session-aware gatekeeper to `RealInventory` |
| **Decorator** | `decorator/network_decorator.h`, `refrigeration_decorator.h`, `solar_decorator.h` | Attaches optional hardware modules to kiosks at runtime |
| **Composite** | `inventory/product.h`, `inventory/bundle.h` | Treats products and bundles uniformly using recursive pricing |
| **Command** | `commands/purchase_command.h`, `refund_command.h`, `restock_command.h` | Encapsulates operations as objects — enables undo and logging |
| **Persistence** | `persistence/persistence_manager.h` | Serialises system state to JSON files after every run |

---

## 🔄 Transaction Lifecycle

Every purchase follows this sequence:

```text
1. Customer Registration
   → User enters Name + Email

2. OTP Verification
   → SMTP sends a 6-digit verification code

3. Item Selection (Composite)
   → User selects Products or Bundles

4. Cart Building
   → Multiple items added before checkout

5. Inventory Access (Proxy)
   → InventoryProxy validates access and logs request

6. Payment Processing (Adapter)
   → UPI / Card / Wallet handled via common interface

7. Dispensing
   → Hardware abstraction releases items

8. Receipt Delivery
   → Purchase summary printed and emailed

9. Persistence
   → Transactions saved to JSON
```

---
## ⭐ Unique Selling Points

**1. Runtime Hardware Swap**
> The dispenser switches from `Spiral → Conveyor` live in Phase 10 — without modifying any kiosk logic. True hardware abstraction via the `IDispenser` interface.

**2. Proxy Session Management**
> `InventoryProxy` asks for a PIN only once. All subsequent accesses in the same session print `Session already authorized` — stateful security without redundancy.

**3. Nested Composite Bundles**
> `Emergency Kit` contains `Meal Kit` (another bundle) + individual products. Price calculated recursively with a cascading 15% discount on top of the inner bundle's 10%.

**4. Real SMTP Integration**
> OTP verification and receipt delivery are handled using Gmail SMTP, bringing real-world authentication into the simulation.
---

## 🔒 Constraint Scenarios Demonstrated

| Scenario | What Happens |
|----------|-------------|
| Controlled Inventory Access | All requests go through `InventoryProxy` — no direct access to `RealInventory` |
| Session-based Authorization | PIN asked once per session — subsequent calls skip re-auth automatically |
| Unified Payment Handling | UPI / Card / Wallet all called through `IPayment::pay()` — adapter handles conversion |
| Live Hardware Swap | `swapDispenser()` called at runtime — next dispense uses new hardware immediately |
| Command Undo | `RefundCommand::execute()` restores stock and logs a REFUND transaction |
| Nested Bundle Pricing | `Bundle::getPrice()` recurses into child bundles before applying its own discount |
| OTP Verification | User must verify via email before completing purchase |

---

## 🧩 Extending the System

| Goal | How |
|------|-----|
| Add a new kiosk type | Implement `IKiosk`, add a case in `kiosk_factory.h` |
| Add a new payment method | Implement `IPayment` in a new adapter file |
| Add a new dispenser | Implement `IDispenser` and pass it to any kiosk via `swapDispenser()` |
| Add a new decorator module | Extend `KioskDecorator` and wrap any kiosk instance |
| Add a new command | Implement `ICommand` with `execute()` and `undo()` methods |
| Add email notifications | Extend SMTP service inside `email/` module |

---

## 📧 SMTP Integration

Aura Retail OS includes real email verification and receipt delivery.

Features include:

- 6-digit OTP verification
- Gmail SMTP integration
- Purchase receipt email
- Session-based verification

---

## 🎬 Simulation Demo

### ▶️ Recommended Input Sequence

| Phase | Prompt | Input | Purpose |
|-------|--------|-------|---------|
| 6 | Select payment method | `2` | Card payment |
| 7 | Select kiosk | `3` | EmergencyKiosk — most complex |
| 7 | Select item | `1` | Emergency Kit — nested bundle |
| 7 | Enter access PIN | `1234` | Proxy security check |
| 7 | Enter card number | `9876543210123456` | Any 16 digits |
| 7 | Enter CVV | `321` | Any 3 digits |
| 8 | Refund? | `1` | Demonstrates Command undo |
| 9 | Restock | `0` | Skip |
| 10 | Select food item | `1` | Water Bottle via swapped hardware |
| 10 | Enter card number | `9876543210123456` | Same card |
| 10 | Enter CVV | `321` | Same CVV |

---

### 🖥️ Full Simulation Output

<details>
<summary>Click to expand full output</summary>

```text
+==================================================================+
|       AURA RETAIL OS  --  INTERACTIVE TERMINAL SYSTEM            |
+==================================================================+

[1] Customer Session
[2] Admin Terminal
[0] Shutdown
>> Select option: 1

+==================================================================+
| STEP 1 of 3  >>  CUSTOMER REGISTRATION                           |
+==================================================================+
[i] Enter Name: Varu
[i] Enter Email: varu@gmail.com
[i] Validating email format... [OK]
[i] Sending 6-digit OTP to varu@gmail.com...
[SMTP Success] OTP sent successfully via Gmail.

[i] Please enter the OTP sent to your email: 582931
[OK] OTP Verified. Access granted to AURA Kiosks.

+==================================================================+
| STEP 2 of 3  >>  KIOSK SELECTION                                 |
+==================================================================+
[1] Food Kiosk      (Metro Station - Gate 2)
[2] Pharmacy Kiosk  (City Hospital - Wing B)
[3] Emergency Kiosk (Disaster Zone - Sector 4)
>> Select kiosk: 1

+==================================================================+
| STEP 3 of 3  >>  CUSTOMER TERMINAL  //  Metro Station - Gate 2   |
+==================================================================+
[1] Add Item to Cart
[2] View Cart & Checkout
[3] View Current Stock
[0] Exit Terminal
>> Select option: 1

INVENTORY // food
┌────┬────────────────────────┬───────────┬─────────┬───────┐
│ #  │ Item                   │ Type      │ Price   │ Stock │
├────┼────────────────────────┼───────────┼─────────┼───────┤
│ 1  │ Water Bottle           │ Food      │ Rs.20   │ 11    │
│ 6  │ Meal Kit (Bundle)      │ Food      │ Rs.45   │ 6     │
└────┴────────────────────────┴───────────┴─────────┴───────┘
>> Select item number: 6

BUNDLE DETAILS: Meal Kit (10% off)
> Meal Kit (10% off) | Subtotal: Rs.45
  |- Water Bottle    | Food       | Rs.20
  |- Energy Bar      | Food       | Rs.30
-------------------------------------------------------
[i] Enter quantity: 2
[OK] Meal Kit (Bundle) x2 added to cart.

>> Select option: 2

YOUR SHOPPING CART
-----------------------------------
  - Meal Kit (Bundle)    x2  Rs.90
  - Water Bottle         x1  Rs.20
-----------------------------------
[i] TOTAL PAYABLE: Rs.110
>> Proceed to payment? [1=Yes / 0=Cancel]: 1

PAYMENT SETUP   >>   Adapter Pattern
[1] UPI gateway
[2] Card terminal
[3] Digital Wallet
>> Select option: 1

[i] Enter UPI ID: varu@okaxis
[OK] UPI ID verified. Payment locked for session.

╔══════════════════════════════════════════════╗
║         AURA RETAIL OS  --  RECEIPT          ║
╠══════════════════════════════════════════════╣
║  Location:            Metro Station - Gate 2 ║
║  Payment:                     UPI (varu@...) ║
║  ------------------------------------------  ║
║  Meal Kit (Bundle) (x2)               Rs.90  ║
║  Water Bottle (x1)                    Rs.20  ║
║  ------------------------------------------  ║
║  TOTAL PAID:                         Rs.110  ║
║  Status:                   PAID / SUCCESSFUL ║
╚══════════════════════════════════════════════╝

[i] Sending consolidated receipt to varu@gmail.com...
[SMTP Success] Receipt delivered successfully.
[OK] Checkout complete! All items dispensed.

+==================================================================+
| ADMIN TERMINAL  //  System Management                            |
+==================================================================+
[i] Admin Username: aura_admin
[i] Admin Password: •••••••••••
[OK] Admin access granted.

[1] Restock All Kiosks
[2] View Global Audit Log
[3] System Diagnostics
>> Select option: 1

[Admin] Restocking Metro Station - Gate 2... [OK]
[Admin] Restocking City Hospital - Wing B... [OK]
[Admin] Restocking Disaster Zone - Sector 4... [OK]

[OK] All system inventories restored to maximum capacity.

[i] Saving system state to persistence/data/... [OK]
[i] Shutdown complete.
```
</details>
---

### 🔍 Pattern Highlights

| # | Pattern | Key Moment in Output |
|---|---------|----------------------|
| 1 | **Singleton** | Central registry initialized once for the entire system |
| 2 | **Factory** | Kiosk selection dynamically creates Food / Pharmacy / Emergency kiosks |
| 3 | **Composite** | Bundles display nested products recursively |
| 4 | **Adapter** | UPI/Card/Wallet handled through one payment interface |
| 5 | **Proxy** | OTP + inventory validation before transaction |
| 6 | **Command** | Checkout, refund, and admin actions executed as commands |
| 7 | **Decorator** | Optional kiosk modules displayed during diagnostics |
| 8 | **Persistence** | System state saved during shutdown |

---

### ⭐ Unique Selling Points

**1. Runtime Hardware Swap**
> Dispenser switches `Spiral → Conveyor` live in Phase 10 without modifying kiosk logic. Demonstrates true hardware abstraction via `IDispenser`.

**2. Proxy Session Management**
> `InventoryProxy` asks for a PIN once per session. All subsequent accesses print `Session already authorized` — stateful security without redundancy.

**3. Nested Composite Bundles**
> `Emergency Kit` contains `Meal Kit` (another bundle) + individual products. Price calculated recursively with a cascading 15% discount applied on top of the inner 10%.

**4. Real SMTP Integration**
> OTP verification and receipt delivery are handled using Gmail SMTP, bringing real-world authentication into the simulation.

---

## 👥 Group Information

- **Group Name:** Encoders
- **Course:** Object Oriented Programming
- **Instructor:** Sourish Dasgupta

| Name | Roll Number |
|------|-------------|
| Nishka Shah | 202512035 |
| Varunee Agrawal | 202512067 |
| Hardi Dhadhal | 202512085 |
| Zalak Thakkar | 202512091 |

---

## 📚 References

- *Design Patterns: Elements of Reusable Object-Oriented Software* — Gamma, Helm, Johnson, Vlissides
- *The C++ Programming Language* — Bjarne Stroustrup

---

## 📄 License

This project was built as an academic OOP demonstration focused on scalable architecture, real-world integrations, and advanced design-pattern implementation.
