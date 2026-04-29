# 🌟 Aura Retail OS

### Smart Retail Kiosk Simulation using C++ & Design Patterns

Aura Retail OS is a modular C++ simulation of an intelligent retail kiosk system deployed across a smart-city environment. The project demonstrates how classic Object-Oriented Design Patterns can be applied to build scalable, flexible, and maintainable systems.

> An interactive smart-city kiosk simulation showcasing Object-Oriented Design Patterns through inventory security, SMTP verification, composite bundles, runtime hardware abstraction, and modular kiosk architecture.

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

```text
AuraRetailOS/
│
├── main.cpp                          # Entry point — launches CLI manager
├── pretty_print.h                    # CLI formatting — tables, banners, colours
├── .gitignore                        # Git ignored files/config
│
├── core/                             # Core system controllers
│   ├── otp_manager.h                 # OTP generation & verification logic
│   ├── otp_manager.cpp               # OTP implementation
│   ├── simulation_runner.h           # Simulation lifecycle manager
│   └── simulation_runner.cpp         # Startup + runtime orchestration
│
├── ui/                               # Terminal Interface Layer
│   ├── cli_manager.h                 # Interactive terminal menu system
│   └── cli_manager.cpp               # Customer/Admin CLI flow
│
├── registry/                         # Singleton Pattern
│   ├── central_registry.h            # Global catalog + shared services
│   └── central_registry.cpp          # Registry implementation
│
├── kiosk/                            # Factory Pattern
│   ├── kiosk_interface.h             # Base kiosk abstraction
│   ├── kiosk_factory.h               # Creates kiosk types dynamically
│   ├── food_kiosk.h                  # Food kiosk implementation
│   ├── pharmacy_kiosk.h              # Pharmacy kiosk implementation
│   └── emergency_kiosk.h             # Emergency kiosk implementation
│
├── inventory/                        # Proxy + Composite Pattern
│   ├── inventory_interface.h         # Inventory abstraction
│   ├── inventory_item.h              # IInventoryItem interface
│   ├── real_inventory.h              # Actual inventory store
│   ├── inventory_proxy.h             # Access control + logging layer
│   ├── product.h                     # Leaf product node
│   ├── bundle.h                      # Composite bundle node
│   │
│   └── hardware/                     # Hardware abstraction
│       ├── idispenser.h              # Dispenser interface
│       ├── spiral_dispenser.h        # Spiral dispenser implementation
│       └── conveyor_dispenser.h      # Conveyor dispenser implementation
│
├── payment/                          # Adapter Pattern
│   ├── payment_interface.h           # Unified IPayment interface
│   ├── upi_adapter.h                 # UPI payment adapter
│   ├── card_adapter.h                # Card payment adapter
│   └── wallet_adapter.h              # Wallet payment adapter
│
├── email/                            # SMTP Email Integration
│   ├── smtp_client.h                 # SMTP client declaration
│   ├── smtp_client.cpp               # SMTP sending implementation
│   ├── email_service.h               # OTP + receipt service
│   └── email_service.cpp             # Email workflow implementation
│
├── decorator/                        # Decorator Pattern
│   ├── kiosk_decorator.h             # Base kiosk decorator
│   ├── network_decorator.h           # Adds network capability
│   ├── refrigeration_decorator.h     # Adds cooling module
│   └── solar_decorator.h             # Adds solar power module
│
├── commands/                         # Command Pattern
│   ├── icommand.h                    # Command interface
│   ├── purchase_command.h            # Purchase execution
│   ├── refund_command.h              # Undo purchase
│   ├── restock_command.h             # Inventory refill command
│   └── command_logger.h              # Transaction command log
│
├── persistence/                      # Persistence Layer
│   ├── persistence_manager.h         # JSON save/load manager
│   │
│   └── data/
│       ├── inventory.json            # Inventory persistence
│       ├── transactions.json         # Transaction history
│       └── config.json               # System configuration
│
├── ars.exe                           # Compiled executable (generated)
├── AuraOS.exe                        # Alternate compiled build
└── README.md                         # Project documentation
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

| Step | Prompt | Input | Purpose |
|------|--------|--------|---------|
| 1 | Select Main Menu | `1` | Start Customer Session |
| 2 | Enter Name | `Varu` | Customer registration |
| 3 | Enter Email | `varu@gmail.com` | SMTP OTP verification |
| 4 | Enter OTP | `582931` | Email authentication |
| 5 | Select Kiosk | `1` | Food Kiosk |
| 6 | Add Item to Cart | `1` | Open inventory |
| 7 | Select Item | `6` | Meal Kit Bundle |
| 8 | Enter Quantity | `2` | Add bundle to cart |
| 9 | View Cart & Checkout | `2` | Proceed to payment |
| 10 | Confirm Checkout | `1` | Begin payment |
| 11 | Select Payment Method | `1` | UPI payment |
| 12 | Enter UPI ID | `varu@okaxis` | Payment authentication |
| 13 | Admin Terminal | `2` | Access admin features |
| 14 | Admin Action | `1` | Restock all kiosks |
| 15 | Shutdown | `0` | Exit simulation |
---

### 🖥️ Full Simulation Output

<details>
<summary>Click to expand full output</summary>

```text
══════════════════════════════════════════════════════════════════
     AURA RETAIL OS  //  SMART CITY KIOSK NETWORK  v2.0
══════════════════════════════════════════════════════════════════
     Powered by Encoders Team  |  OOP Design Patterns Demo
──────────────────────────────────────────────────────────────────


  >> Singleton Registry Init
  ------------------------------------------------------------
[CentralRegistry] Singleton instance created.
  [  OK  ] CentralRegistry instantiated           [Singleton Pattern]
  [  OK  ] Singleton verified — same pointer returned
  [  OK  ] Shared RealInventory → allocated (stock persists across all sessions)
[Persistence] Inventory loaded from inventory.json
  [  OK  ] PersistenceManager → inventory.json checked and loaded

  >> Hardware Interface Layer
  ------------------------------------------------------------
  [  OK  ] Spiral Dispenser → registered as default hardware
  [  OK  ] Conveyor Dispenser → standby unit loaded

  >> Kiosk Factory [Factory Pattern]
  ------------------------------------------------------------
[KioskFactory] Creating kiosk of type: food
[FoodKiosk] Initialized at Metro Station - Gate 2
[FoodKiosk] Active hardware: Spiral Dispenser

[KioskFactory] Creating kiosk of type: pharmacy
[PharmacyKiosk] Initialized at City Hospital - Wing B
[PharmacyKiosk] Active hardware: Spiral Dispenser

[KioskFactory] Creating kiosk of type: emergency
[EmergencyKiosk] Initialized at Disaster Zone - Sector 4
[EmergencyKiosk] Active hardware: Conveyor Dispenser

  [  OK  ] FoodKiosk → Metro Station - Gate 2
  [  OK  ] PharmacyKiosk → City Hospital - Wing B
  [  OK  ] EmergencyKiosk → Disaster Zone - Sector 4

  >> Decorator Module Attach [Decorator Pattern]
  ------------------------------------------------------------
[NetworkDecorator] Module attached. Signal strength: 94%
[RefrigerationDecorator] Module attached. Temperature set to 4C
[SolarDecorator] Module attached. Battery level: 87%

  >> Inventory Catalog Sync [Composite Pattern]
  ------------------------------------------------------------
  [  OK  ] Food Catalog → 6 items registered
  [  OK  ] Pharmacy Catalog → 5 items registered
  [  OK  ] Emergency Catalog → 4 items registered

  >> System Ready
  ------------------------------------------------------------
  [  OK  ] Payment Gateway → Standby
  [  OK  ] Command Engine → Loaded
  [  OK  ] Inventory Proxy → Armed
  [  OK  ] Persistence Manager → Initialized

  >> AURA RETAIL OS READY  --  ALL SYSTEMS NOMINAL

Press ENTER to launch the kiosk interface...


╔══════════════════════════════════════════════════════════════╗
║  AURA RETAIL OS  //  SELECT ACCESS MODE                      ║
╚══════════════════════════════════════════════════════════════╝

[1] Customer Access
[2] Admin Login
[0] Shut Down System

>> Select mode: 1

>> Enter your name: hardi
>> Enter email address: dhadhalhardi2004@gmail.com

[ INFO ] Generating OTP...
[SMTP Success] Email sent successfully
[ OK ] OTP sent successfully

>> Enter 6-digit OTP: 605552
[ OK ] OTP Verified! Starting session...

╔══════════════════════════════════════════════════════════════╗
║  STEP 1 of 3  >>  SELECT YOUR KIOSK                          ║
╚══════════════════════════════════════════════════════════════╝

[1] Food Kiosk
[2] Pharmacy Kiosk
[3] Emergency Kiosk

>> Select kiosk: 1

╔══════════════════════════════════════════════════════════════╗
║  STEP 2 of 3  >>  KIOSK DASHBOARD                            ║
╚══════════════════════════════════════════════════════════════╝

Food Kiosk → Metro Station - Gate 2
Hardware → Spiral Dispenser
Modules → Network 94%, Refrigeration 4C

╔══════════════════════════════════════════════════════════════╗
║  STEP 3 of 3  >>  CUSTOMER TERMINAL                          ║
╚══════════════════════════════════════════════════════════════╝

[1] Add Item to Cart
[2] View Cart & Checkout
[3] View Current Stock
[4] Refund Last Purchase
[0] Exit Terminal

>> Select option: 1

PRODUCT CATALOGUE // FOOD

Water Bottle → Rs.20
Sandwich → Rs.50
Meal Kit Bundle → Rs.45

>> Select item: Water Bottle
>> Quantity: 1
[ OK ] Added to cart

>> Select item: Sandwich
>> Quantity: 2
[ OK ] Added to cart

YOUR SHOPPING CART
-----------------------------------
Water Bottle x1 → Rs.20
Sandwich x2 → Rs.100
-----------------------------------
TOTAL PAYABLE → Rs.120

>> Proceed to payment: 1

PAYMENT SETUP  // Adapter Pattern

[1] UPI
[2] Card
[3] Wallet

>> Select payment: 1
>> Enter UPI ID: varu@upi

[ OK ] UPI verified and saved for session

[PurchaseCommand] Executing purchase...
[InventoryProxy] PIN verified
[UPIAdapter] Payment processed
[SpiralDispenser] Dispensing item

╔══════════════════════════════════════════════╗
║         AURA RETAIL OS  --  RECEIPT          ║
╠══════════════════════════════════════════════╣
║  Water Bottle (x1)                 Rs.20     ║
║  Sandwich (x2)                    Rs.100     ║
║  ------------------------------------------  ║
║  TOTAL PAID:                      Rs.120     ║
║  Status:                PAID / SUCCESSFUL    ║
╚══════════════════════════════════════════════╝

[SMTP Success] Receipt sent successfully
[ OK ] Checkout complete

>> Refund Last Purchase
[RefundCommand] Refund processed
[InventoryProxy] Stock restored

>> Select mode: 0

╔══════════════════════════════════════════════════════════════╗
║  SAVING SYSTEM STATE  //  Persistence                        ║
╚══════════════════════════════════════════════════════════════╝

[Persistence] Inventory saved
[Persistence] Transactions saved
[Persistence] Config saved

╔════════════════════════════════════════════════════════════════╗
║               AURA RETAIL OS  --  SESSION ENDED              ║
╚════════════════════════════════════════════════════════════════╝

Singleton   → Verified
Factory     → Verified
Adapter     → Verified
Proxy       → Verified
Decorator   → Verified
Composite   → Verified
Command     → Verified
Persistence → Verified
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

## 🧠 System Architecture Highlights

- Shared inventory persists across sessions using Singleton Registry
- Customer + Admin modes run through a centralized CLI manager
- OTP authentication handled through SMTP email integration
- Payment gateways unified using Adapter Pattern
- Inventory protection enforced using Proxy authorization
- Runtime modules added dynamically using Decorator Pattern
- Product bundles recursively calculated using Composite Pattern
- Purchase/refund operations encapsulated using Command Pattern
- JSON persistence restores system state across launches

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
