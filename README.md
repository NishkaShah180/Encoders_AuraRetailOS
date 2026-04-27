# 🌟 Aura Retail OS

### Smart Retail Kiosk Simulation using C++ & Design Patterns

Aura Retail OS is a modular C++ simulation of an intelligent retail kiosk system deployed across a smart-city environment. The project demonstrates how classic Object-Oriented Design Patterns can be applied to build scalable, flexible, and maintainable systems.

> A structured 11-phase simulation showcasing how 8 design patterns solve real-world system design problems — with a polished CLI, live transaction logging, and runtime hardware swapping.

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
| 5 | Decorator Modules | Network, Refrigeration, Solar modules attached optionally per kiosk |
| 6 | Composite Inventory | Products, Bundles, and Nested Bundles with cascading discounts |
| 7 | Command Pattern | Purchase, Refund (undo), and Restock as encapsulated commands |
| 8 | Persistence | Inventory, transactions, and config saved to JSON after every run |
| 9 | Pretty CLI | Colour-coded phase boxes, tables, diagnostics cards, transaction log |

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
| **Composite** | `inventory/product.h`, `inventory/bundle.h` | Treats single items and bundles (incl. nested bundles) uniformly |
| **Command** | `commands/purchase_command.h`, `refund_command.h`, `restock_command.h` | Encapsulates operations as objects — enables undo and logging |
| **Persistence** | `persistence/persistence_manager.h` | Serialises system state to JSON files after every run |

---

## 🔄 Transaction Lifecycle

Every purchase follows this sequence:

```
1. Kiosk Selection
   → User picks Food / Pharmacy / Emergency kiosk

2. Item Selection (Composite)
   → User picks a Product, Bundle, or Nested Bundle
   → Discounted price calculated recursively

3. Inventory Access (Proxy)
   → InventoryProxy checks PIN / session authorization
   → Logs access request with timestamp and user

4. Payment Processing (Adapter)
   → Selected adapter (UPI / Card / Wallet) processes payment
   → All adapters called through unified IPayment interface

5. Dispensing (Hardware Abstraction)
   → Active dispenser (Spiral / Conveyor) releases the item

6. Stock Update
   → InventoryProxy updates stock in RealInventory

7. Command Logging
   → CommandLogger records TXN ID, type, item, amount, status

8. Undo (Optional)
   → RefundCommand reverses purchase and restores stock
```

---

## ⭐ Unique Selling Points

**1. Runtime Hardware Swap**
> The dispenser switches from `Spiral → Conveyor` live in Phase 10 — without modifying any kiosk logic. True hardware abstraction via the `IDispenser` interface.

**2. Proxy Session Management**
> `InventoryProxy` asks for a PIN only once. All subsequent accesses in the same session print `Session already authorized` — stateful security without redundancy.

**3. Nested Composite Bundles**
> `Emergency Kit` contains `Meal Kit` (another bundle) + individual products. Price calculated recursively with a cascading 15% discount on top of the inner bundle's 10%.

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

---

## 🧩 Extending the System

| Goal | How |
|------|-----|
| Add a new kiosk type | Implement `IKiosk`, add a case in `kiosk_factory.h` |
| Add a new payment method | Implement `IPayment` in a new adapter file |
| Add a new dispenser | Implement `IDispenser` and pass it to any kiosk via `swapDispenser()` |
| Add a new decorator module | Extend `KioskDecorator` and wrap any kiosk instance |
| Add a new command | Implement `ICommand` with `execute()` and `undo()` methods |

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

```
+==================================================================+
|          AURA RETAIL OS  --  FINAL SIMULATION                   |
+==================================================================+


+====================================================================+
| PHASE 1: Central Registry  ──  Singleton Pattern                   |
+====================================================================+
[CentralRegistry] Initializing system...
[CentralRegistry] Singleton instance created.
  [OK] Singleton verified -- same instance returned.

+====================================================================+
| PHASE 2: Hardware Abstraction  ──  Dispenser Setup                 |
+====================================================================+
  [i]  Active dispenser: Spiral Dispenser

+====================================================================+
| PHASE 3: Kiosk Creation  ──  Factory Pattern                       |
+====================================================================+
[KioskFactory] Creating kiosk of type: food
[FoodKiosk] Initialized at Metro Station - Gate 2
[KioskFactory] Creating kiosk of type: pharmacy
[PharmacyKiosk] Initialized at City Hospital - Wing B
[KioskFactory] Creating kiosk of type: emergency
[EmergencyKiosk] Initialized at Disaster Zone - Sector 4

┌──────────────────┬────────────────────────────┬────────────────────┐
│ Kiosk            │ Location                   │ Hardware           │
├──────────────────┼────────────────────────────┼────────────────────┤
│ FoodKiosk        │ Metro Station - Gate 2     │ Spiral Dispenser   │
│ PharmacyKiosk    │ City Hospital - Wing B     │ Spiral Dispenser   │
│ EmergencyKiosk   │ Disaster Zone - Sector 4   │ Conveyor Dispenser │
└──────────────────┴────────────────────────────┴────────────────────┘

+====================================================================+
| PHASE 4: Optional Modules  ──  Decorator Pattern                   |
+====================================================================+
  [OK] NetworkDecorator       attached -> Signal: 94%
  [OK] RefrigerationDecorator attached -> Temp: 4C
  [OK] SolarDecorator         attached -> Battery: 87%

+====================================================================+
| PHASE 5: Inventory Setup  ──  Composite Pattern                    |
+====================================================================+

  > Single Products
  -------------------------------------------------------
┌────┬────────────────────────┬───────────┬─────────┬───────┐
│ #  │ Item                   │ Type      │ Price   │ Stock │
├────┼────────────────────────┼───────────┼─────────┼───────┤
│ 1  │ Water Bottle           │ Food      │ Rs.20   │ 15    │
│ 2  │ Energy Bar             │ Food      │ Rs.30   │ 10    │
│ 3  │ Paracetamol            │ Pharmacy  │ Rs.50   │ 8     │
│ 4  │ Bandage                │ Pharmacy  │ Rs.15   │ 20    │
└────┴────────────────────────┴───────────┴─────────┴───────┘

  > Bundles
  -------------------------------------------------------
┌────┬────────────────────────┬───────────┬─────────┬───────┐
│ #  │ Item                   │ Type      │ Price   │ Stock │
├────┼────────────────────────┼───────────┼─────────┼───────┤
│ 1  │ Meal Kit (10% off)     │ Food      │ Rs.45   │  -    │
│ 2  │ Emrg. Kit (15% off)    │ Emergency │ Rs.93.5 │  -    │
└────┴────────────────────────┴───────────┴─────────┴───────┘
  [OK] All items registered in Central Catalog.

+====================================================================+
| PHASE 6: Payment Providers  ──  Adapter Pattern                    |
+====================================================================+

┌────┬──────────┬──────────────────────────┐
│ #  │ Method   │ Description              │
├────┼──────────┼──────────────────────────┤
│ 1  │ UPI      │ Instant bank transfer    │
│ 2  │ Card     │ Debit / Credit card      │
│ 3  │ Wallet   │ Digital wallet balance   │
└────┴──────────┴──────────────────────────┘
Select payment method: 2
  [OK] Card adapter selected. All use IPayment interface.

+====================================================================+
| PHASE 7: Purchase Simulation  ──  Proxy + Command                  |
+====================================================================+
┌────┬────────────────┬──────────────────────────────┐
│ #  │ Kiosk          │ Location                     │
├────┼────────────────┼──────────────────────────────┤
│ 1  │ FoodKiosk      │ Metro Station - Gate 2       │
│ 2  │ PharmacyKiosk  │ City Hospital - Wing B       │
│ 3  │ EmergencyKiosk │ Disaster Zone - Sector 4     │
└────┴────────────────┴──────────────────────────────┘
Select kiosk: 3
Select item number: 1

[PurchaseCommand] Executing purchase...
[InventoryProxy] Access request for: Emergency Kit
[InventoryProxy] Authorization required for: SimulationUser
[InventoryProxy] Enter access PIN: 1234
[InventoryProxy] PIN verified. Access GRANTED for this session.
[InventoryProxy] Access logged: Emergency Kit by SimulationUser
[Card Payment] Enter Card Number (any digits): 9876543210123456
[Card Payment] Enter CVV: 321
[CardAdapter] Card **** **** **** 3456 accepted.
[Card System] Charging Rs.93.5 to card ending in 3456
Paid Rs.93.5 via Card.
[SpiralDispenser] Dispensing: Emergency Kit
[InventoryProxy] Stock updated: Emergency Kit => 4 remaining
[PurchaseCommand] Purchase of Emergency Kit complete.
  [OK] Purchase of Emergency Kit complete.

+====================================================================+
| PHASE 8: Refund  ──  Command Pattern Undo                          |
+====================================================================+
Would you like to refund? (1=Yes / 0=No): 1

[RefundCommand] Refund of Rs.93.5 processed. Stock restored.
  [OK] Refund of Rs.93 processed. Stock restored.

+====================================================================+
| PHASE 9: Restock  ──  Command Pattern                              |
+====================================================================+
Restock item name (or 0 to skip): 0
  [i]  Restock skipped.

+====================================================================+
| PHASE 10: Runtime Hardware Swap  ──  FoodKiosk                     |
+====================================================================+
  [i]  FoodKiosk current: Spiral Dispenser
  -->  Swapping to Conveyor Dispenser...
[HardwareManager] Swapping dispenser: Spiral Dispenser -> Conveyor Dispenser
  [OK] Swap complete -> Active: Conveyor Dispenser

Select item number: 1

[InventoryProxy] Session already authorized. Access GRANTED.
[ConveyorDispenser] Dispensing: Water Bottle
[InventoryProxy] Stock updated: Water Bottle => 14 remaining
  [OK] Test purchase of Water Bottle via Conveyor successful.

+====================================================================+
| PHASE 11: Diagnostics  ──  All Kiosks                              |
+====================================================================+
┌──────────────────────────────────────────────────────┐
│  FoodKiosk                                     ONLINE│
├──────────────────────────────────────────────────────┤
│ Location : Metro Station - Gate 2                    │
│ Hardware : Conveyor Dispenser                        │
│ Module   : Network   : Connected | Signal 94%        │
│ Module   : Refrigeration : Active | Temp 4C          │
└──────────────────────────────────────────────────────┘

┌──────────────────────────────────────────────────────┐
│  PharmacyKiosk                                 ONLINE│
├──────────────────────────────────────────────────────┤
│ Location : City Hospital - Wing B                    │
│ Hardware : Spiral Dispenser                          │
└──────────────────────────────────────────────────────┘

┌──────────────────────────────────────────────────────┐
│  EmergencyKiosk                                ONLINE│
├──────────────────────────────────────────────────────┤
│ Location : Disaster Zone - Sector 4                  │
│ Hardware : Conveyor Dispenser                        │
│ Module   : Solar Power : Active | Battery 87%        │
└──────────────────────────────────────────────────────┘

+====================================================================+
| TRANSACTION LOG                                                    |
+====================================================================+
┌─────────┬──────────┬─────────────────┬─────────┬─────────┬──────────┐
│ TXN     │ Type     │ Item            │ Amt     │ Method  │ Status   │
├─────────┼──────────┼─────────────────┼─────────┼─────────┼──────────┤
│ #TXN001 │ PURCHASE │ Emergency Kit   │ Rs.93   │ Card    │ Success  │
│ #TXN002 │ REFUND   │ Emergency Kit   │ Rs.93   │ Card    │ Refund   │
│ #TXN003 │ PURCHASE │ Water Bottle    │ Rs.20   │ Card    │ Success  │
└─────────┴──────────┴─────────────────┴─────────┴─────────┴──────────┘

+====================================================================+
| PERSISTENCE  ──  Saving System State                               |
+====================================================================+
  [OK] inventory.json    saved.
  [OK] transactions.json saved.
  [OK] config.json       saved.

+==================================================================+
|                    SIMULATION COMPLETE                          |
+==================================================================+
┌────┬─────────────┬──────────────────────────────────────────┐
│    │ Pattern     │ Verified                                 │
├────┼─────────────┼──────────────────────────────────────────┤
│ OK │ Singleton   │ CentralRegistry - single instance enforced│
│ OK │ Factory     │ 3 kiosk types created via KioskFactory   │
│ OK │ Adapter     │ UPI / Card / Wallet unified via IPayment │
│ OK │ Proxy       │ PIN-secured inventory + session auth     │
│ OK │ Decorator   │ Refrigeration, Network, Solar modules    │
│ OK │ Composite   │ Products -> Bundles -> Nested Bundles    │
│ OK │ Command     │ Purchase, Refund, Restock + undo + log   │
│ OK │ Persistence │ Saved to inventory/transactions/config   │
└────┴─────────────┴──────────────────────────────────────────┘
```

</details>

---

### 🔍 Pattern Highlights

| # | Pattern | Key Moment in Output |
|---|---------|----------------------|
| 1 | **Singleton** | `Singleton verified -- same instance returned` |
| 2 | **Factory** | 3 kiosk types created in table with correct hardware assigned |
| 3 | **Decorator** | Network + Refrigeration on Food, Solar on Emergency — shown in diagnostics |
| 4 | **Composite** | Emergency Kit = nested bundle (Meal Kit + Paracetamol + Bandage) |
| 5 | **Adapter** | Card/UPI/Wallet all called through same `IPayment::pay()` |
| 6 | **Proxy** | PIN asked once → `Session already authorized` on second access |
| 7 | **Command** | Purchase executed → Refund undoes it → stock restored |
| 8 | **Persistence** | 3 JSON files saved at end of every run |

---

### ⭐ Unique Selling Points

**1. Runtime Hardware Swap**
> Dispenser switches `Spiral → Conveyor` live in Phase 10 without modifying kiosk logic. Demonstrates true hardware abstraction via `IDispenser`.

**2. Proxy Session Management**
> `InventoryProxy` asks for a PIN once per session. All subsequent accesses print `Session already authorized` — stateful security without redundancy.

**3. Nested Composite Bundles**
> `Emergency Kit` contains `Meal Kit` (another bundle) + individual products. Price calculated recursively with a cascading 15% discount applied on top of the inner 10%.

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

This project was built as an academic OOP demonstration.