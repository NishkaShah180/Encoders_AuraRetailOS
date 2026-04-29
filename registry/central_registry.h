#pragma once
#include <iostream>
#include <memory>
#include <vector>
#include <string>
#include <algorithm>
#include "../inventory/hardware/idispenser.h"
#include "../inventory/inventory_proxy.h"
#include "../inventory/inventory_item.h"
#include "../payment/payment_interface.h"

struct CatalogEntry {
    std::shared_ptr<IInventoryItem> item;
    std::string kioskType;
};

class CentralRegistry {
    std::shared_ptr<IDispenser> dispenser;
    std::shared_ptr<IPayment> payment;
    std::vector<CatalogEntry> catalog;

    CentralRegistry() {
        std::cout << "[CentralRegistry] Singleton instance created.\n";
    }

public:
    // Meyer's Singleton — thread-safe, no separate .cpp needed [Singleton Pattern]
    static CentralRegistry* getInstance() {
        static CentralRegistry inst;
        return &inst;
    }

    void setDispenser(std::shared_ptr<IDispenser> d) { dispenser = d; }
    void setPayment(std::shared_ptr<IPayment> p)     { payment = p; }

    std::shared_ptr<IDispenser> getDispenser() { return dispenser; }
    std::shared_ptr<IPayment>   getPayment()   { return payment; }

    void addToCatalog(std::shared_ptr<IInventoryItem> item,
                      const std::string& kioskType) {
        catalog.push_back({item, kioskType});
        std::cout << "[CentralRegistry] Added to catalog: "
                  << item->getName()
                  << " | Rs." << item->getPrice()
                  << " | Kiosk: " << kioskType << "\n";
    }

    std::shared_ptr<IInventoryItem> selectItemFromCatalog(
                                    const std::string& kioskType) {
        std::vector<std::shared_ptr<IInventoryItem>> filtered;

        for (auto& entry : catalog)
            if (entry.kioskType == kioskType)
                filtered.push_back(entry.item);

        if (filtered.empty()) {
            std::cout << "[CentralRegistry] No items available "
                      << "for kiosk type: " << kioskType << "\n";
            return nullptr;
        }

        std::cout << "\n[CentralRegistry] Available items "
                  << "for " << kioskType << " kiosk:\n";
        for (int i = 0; i < (int)filtered.size(); i++) {
            std::cout << "  " << i + 1 << ". ";
            filtered[i]->display();
        }

        int choice = 0;
        std::cout << "Select item number: ";
        std::cin >> choice;

        if (choice < 1 || choice > (int)filtered.size()) {
            std::cout << "[CentralRegistry] Invalid selection. "
                    << "Please enter between 1 and "
                    << filtered.size() << ".\n";   // ← IMPROVED MESSAGE
            return nullptr;
        }

        return filtered[choice - 1];
    }

    void displayCatalog() {
        std::cout << "\n[CentralRegistry] Full catalog:\n";
        for (auto& entry : catalog)
            entry.item->display();
    }

    const std::vector<CatalogEntry>& getCatalog() const { return catalog; }

    void removeFromCatalog(const std::string& name, const std::string& kioskType) {
        catalog.erase(
            std::remove_if(catalog.begin(), catalog.end(),
                [&](const CatalogEntry& e) {
                    return e.item->getName() == name && e.kioskType == kioskType;
                }),
            catalog.end());
        std::cout << "[CentralRegistry] Removed: " << name << "\n";
    }

    ~CentralRegistry() {}
};
