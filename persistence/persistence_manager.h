#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "../inventory/inventory_proxy.h"
#include "../commands/command_logger.h"

class PersistenceManager {
public:
    static void saveInventory(InventoryProxy& inventory,
                               const std::vector<std::string>& itemNames) {
        std::ofstream file("persistence/data/inventory.json");
        if (!file.is_open()) {
            std::cout << "[Persistence] Could not open inventory.json\n";
            return;
        }

        file << "{\n";
        file << "  \"inventory\": [\n";

        for (int i = 0; i < itemNames.size(); i++) {
            int qty = inventory.getStock(itemNames[i]);
            file << "    { \"item\": \"" << itemNames[i]
                 << "\", \"quantity\": " << qty << " }";
            if (i < itemNames.size() - 1) file << ",";
            file << "\n";
        }

        file << "  ]\n";
        file << "}\n";
        file.close();

        std::cout << "[Persistence] Inventory saved to inventory.json\n";
    }

    static void saveTransactions(CommandLogger& logger,
                                  const std::vector<LogEntry>& entries) {
        std::ofstream file("persistence/data/transactions.json");
        if (!file.is_open()) {
            std::cout << "[Persistence] Could not open transactions.json\n";
            return;
        }

        file << "{\n";
        file << "  \"transactions\": [\n";

        for (int i = 0; i < entries.size(); i++) {
            file << "    {\n";
            file << "      \"type\": \""    << entries[i].type          << "\",\n";
            file << "      \"item\": \""    << entries[i].itemName      << "\",\n";
            file << "      \"amount\": "    << entries[i].amount        << ",\n";
            file << "      \"payment\": \"" << entries[i].paymentMethod << "\",\n";
            file << "      \"status\": \""  << entries[i].status        << "\"\n";
            file << "    }";
            if (i < entries.size() - 1) file << ",";
            file << "\n";
        }

        file << "  ]\n";
        file << "}\n";
        file.close();

        std::cout << "[Persistence] Transactions saved to transactions.json\n";
    }

    static void saveConfig(const std::string& dispenserType,
                            const std::string& location) {
        std::ofstream file("persistence/data/config.json");
        if (!file.is_open()) {
            std::cout << "[Persistence] Could not open config.json\n";
            return;
        }

        file << "{\n";
        file << "  \"config\": {\n";
        file << "    \"dispenser\": \"" << dispenserType << "\",\n";
        file << "    \"location\": \""  << location      << "\"\n";
        file << "  }\n";
        file << "}\n";
        file.close();

        std::cout << "[Persistence] Config saved to config.json\n";
    }

    static void saveAll(InventoryProxy& inventory,
                        const std::vector<std::string>& itemNames,
                        CommandLogger& logger,
                        const std::vector<LogEntry>& entries,
                        const std::string& dispenserType,
                        const std::string& location) {
        system("if not exist persistence\\data mkdir persistence\\data");
        std::cout << "\n------------------------------------------------------------\n";
        std::cout << " PHASE: Persistence -- Saving System State\n";
        std::cout << "------------------------------------------------------------\n";
        saveInventory(inventory, itemNames);
        saveTransactions(logger, entries);
        saveConfig(dispenserType, location);
        std::cout << "[Persistence] All data saved successfully.\n";
    }
};