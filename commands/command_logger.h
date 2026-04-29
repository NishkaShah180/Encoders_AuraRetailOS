#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <thread>
#include <chrono>

struct LogEntry {
    std::string type;
    std::string itemName;
    double amount;
    std::string paymentMethod;
    std::string status;
};

class CommandLogger {
    std::vector<LogEntry> log;
    int txnCounter = 1;

public:
    void record(const std::string& type, const std::string& itemName,
                double amount, const std::string& paymentMethod,
                const std::string& status) {
        log.push_back({type, itemName, amount, paymentMethod, status});
    }

    void printLog() {
        std::cout << "\n------------------------------------------------------------\n";
        std::cout << " TRANSACTION LOG\n";
        std::cout << "------------------------------------------------------------\n";

        if (log.empty()) {
            std::cout << "No transactions recorded.\n";
            return;
        }

        for (int i = 0; i < log.size(); i++) {
            std::cout << "#TXN00" << (i + 1)
                      << " | " << log[i].type
                      << " | " << log[i].itemName
                      << " | Rs." << log[i].amount
                      << " | " << log[i].paymentMethod
                      << " | " << log[i].status << "\n";
        }
    }

    int getCount() { return (int)log.size(); }
    const std::vector<LogEntry>& getEntries() const {
        return log;
    }
};