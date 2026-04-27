#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <iomanip>
#include <sstream>

// ─────────────────────────────────────────────
//  AURA RETAIL OS  --  Pretty Print Utilities
// ─────────────────────────────────────────────

namespace AuraCLI {

// ── Colours ───────────────────────────────────
namespace Color {
    const std::string RESET   = "\033[0m";
    const std::string BOLD    = "\033[1m";
    const std::string DIM     = "\033[2m";
    const std::string CYAN    = "\033[36m";
    const std::string YELLOW  = "\033[33m";
    const std::string MAGENTA = "\033[35m";
    const std::string WHITE   = "\033[97m";
    const std::string BCYAN   = "\033[96m";
    const std::string BGREEN  = "\033[92m";
    const std::string BRED    = "\033[91m";
}

// ── Box chars (bytes literal so no width confusion) ──
const std::string tTL = "\xe2\x94\x8c"; // ┌
const std::string tTR = "\xe2\x94\x90"; // ┐
const std::string tBL = "\xe2\x94\x94"; // └
const std::string tBR = "\xe2\x94\x98"; // ┘
const std::string tML = "\xe2\x94\x9c"; // ├
const std::string tMR = "\xe2\x94\xa4"; // ┤
const std::string tTM = "\xe2\x94\xac"; // ┬
const std::string tBM = "\xe2\x94\xb4"; // ┴
const std::string tXX = "\xe2\x94\xbc"; // ┼
const std::string tH  = "\xe2\x94\x80"; // ─
const std::string tV  = "\xe2\x94\x82"; // │

// ── Helpers ───────────────────────────────────

inline std::string repeatStr(const std::string& s, int n) {
    std::string r;
    for (int i = 0; i < n; i++) r += s;
    return r;
}

// Count visible chars (skip ANSI codes + UTF-8 continuation bytes)
inline int visibleLen(const std::string& s) {
    int len = 0; bool esc = false;
    for (unsigned char c : s) {
        if (c == '\033') { esc = true; continue; }
        if (esc) { if (c == 'm') esc = false; continue; }
        if ((c & 0xC0) != 0x80) len++;  // skip UTF-8 continuation bytes
    }
    return len;
}

inline std::string padStr(const std::string& s, int width) {
    int vl = visibleLen(s);
    if (vl >= width) return s;
    return s + std::string(width - vl, ' ');
}

// ═══════════════════════════════════════════════
//  PHASE BOXES  —  pure ASCII, guaranteed align
// ═══════════════════════════════════════════════

inline void printBanner() {
    std::cout << Color::BCYAN << Color::BOLD;
    std::cout << "\n+==================================================================+\n";
    std::cout <<   "|          AURA RETAIL OS  --  FINAL SIMULATION                   |\n";
    std::cout <<   "+==================================================================+\n";
    std::cout << Color::RESET << "\n";
}

inline void printSimComplete() {
    std::cout << Color::BCYAN << Color::BOLD;
    std::cout << "\n+==================================================================+\n";
    std::cout <<   "|                    SIMULATION COMPLETE                          |\n";
    std::cout <<   "+==================================================================+\n";
    std::cout << Color::RESET;
}

inline void printPhase(const std::string& title) {
    const int inner = 68;
    int rpad = inner - 2 - visibleLen(title);  // ← visibleLen not title.size()
    if (rpad < 0) rpad = 0;
    std::cout << "\n" << Color::CYAN << Color::BOLD;
    std::cout << "+" << std::string(inner, '=') << "+\n";
    std::cout << "| " << title << std::string(rpad, ' ') << " |\n";
    std::cout << "+" << std::string(inner, '=') << "+\n";
    std::cout << Color::RESET;
}

// ═══════════════════════════════════════════════
//  STATUS LINES
// ═══════════════════════════════════════════════

inline void ok(const std::string& msg) {
    std::cout << Color::BGREEN << "  [OK] " << Color::RESET << msg << "\n";
}
inline void info(const std::string& msg) {
    std::cout << Color::CYAN   << "  [i]  " << Color::RESET << msg << "\n";
}
inline void warn(const std::string& msg) {
    std::cout << Color::YELLOW << "  [!]  " << Color::RESET << msg << "\n";
}
inline void fail(const std::string& msg) {
    std::cout << Color::BRED   << "  [x]  " << Color::RESET << msg << "\n";
}
inline void arrow(const std::string& msg) {
    std::cout << Color::MAGENTA << "  -->  " << Color::RESET << msg << "\n";
}

// ═══════════════════════════════════════════════
//  TABLE PRIMITIVES  —  thin box chars (per-cell
//  width is fine, no full-width repeat needed)
// ═══════════════════════════════════════════════

struct TableCol {
    std::string header;
    int width;
};

inline void printTableTop(const std::vector<TableCol>& cols) {
    std::cout << tTL;
    for (int i = 0; i < (int)cols.size(); i++) {
        std::cout << repeatStr(tH, cols[i].width + 2);
        std::cout << (i + 1 < (int)cols.size() ? tTM : tTR);
    }
    std::cout << "\n";
}

inline void printTableMid(const std::vector<TableCol>& cols) {
    std::cout << tML;
    for (int i = 0; i < (int)cols.size(); i++) {
        std::cout << repeatStr(tH, cols[i].width + 2);
        std::cout << (i + 1 < (int)cols.size() ? tXX : tMR);
    }
    std::cout << "\n";
}

inline void printTableBot(const std::vector<TableCol>& cols) {
    std::cout << tBL;
    for (int i = 0; i < (int)cols.size(); i++) {
        std::cout << repeatStr(tH, cols[i].width + 2);
        std::cout << (i + 1 < (int)cols.size() ? tBM : tBR);
    }
    std::cout << "\n";
}

inline void printTableHeader(const std::vector<TableCol>& cols) {
    printTableTop(cols);
    std::cout << tV;
    for (int i = 0; i < (int)cols.size(); i++) {
        std::cout << " " << Color::BOLD << Color::WHITE
                  << padStr(cols[i].header, cols[i].width)
                  << Color::RESET << " " << tV;
    }
    std::cout << "\n";
    printTableMid(cols);
}

inline void printTableRow(const std::vector<TableCol>& cols,
                          const std::vector<std::string>& vals) {
    std::cout << tV;
    for (int i = 0; i < (int)cols.size() && i < (int)vals.size(); i++) {
        std::cout << " " << padStr(vals[i], cols[i].width) << " " << tV;
    }
    std::cout << "\n";
}

// ═══════════════════════════════════════════════
//  INVENTORY TABLE
// ═══════════════════════════════════════════════

inline void printInventoryTable(
    const std::vector<std::string>& names,
    const std::vector<std::string>& prices,
    const std::vector<std::string>& qtys,
    const std::vector<std::string>& types)
{
    std::vector<TableCol> cols = {
        {"#",     2},
        {"Item",  22},
        {"Type",  9},
        {"Price", 7},
        {"Stock", 5}
    };
    printTableHeader(cols);
    for (int i = 0; i < (int)names.size(); i++) {
        std::string idxStr   = std::to_string(i + 1);
        std::string priceCol = Color::YELLOW + prices[i] + Color::RESET;
        std::string qtyStr   = qtys[i];
        std::string qtyCol;

        if (qtyStr == "-" || qtyStr == "N/A") {
            qtyCol = Color::DIM + " -" + Color::RESET;
        } else {
            int q = 0;
            try { q = std::stoi(qtyStr); } catch (...) {}
            qtyCol = (q <= 3)
                ? Color::BRED   + qtyStr + Color::RESET
                : Color::BGREEN + qtyStr + Color::RESET;
        }

        printTableRow(cols, {idxStr, names[i], types[i], priceCol, qtyCol});
    }
    printTableBot(cols);
}

// ═══════════════════════════════════════════════
//  TRANSACTION TABLE
// ═══════════════════════════════════════════════

struct TxnEntry {
    std::string id;
    std::string type;
    std::string item;
    std::string amount;
    std::string method;
    std::string status;
};

inline void printTransactionTable(const std::vector<TxnEntry>& txns) {
    std::vector<TableCol> cols = {
        {"TXN",    7},
        {"Type",   8},
        {"Item",  15},
        {"Amt",    7},
        {"Method", 7},
        {"Status", 8}
    };
    printTableHeader(cols);
    for (auto& t : txns) {
        std::string statusCol;
        if      (t.status == "Success")  statusCol = Color::BGREEN + "Success" + Color::RESET;
        else if (t.status == "Failed")   statusCol = Color::BRED   + "Failed"  + Color::RESET;
        else if (t.status == "Refunded") statusCol = Color::YELLOW + "Refund"  + Color::RESET;
        else                             statusCol = t.status;

        printTableRow(cols, {t.id, t.type, t.item, t.amount, t.method, statusCol});
    }
    printTableBot(cols);
}

// ═══════════════════════════════════════════════
//  PATTERN SUMMARY TABLE
// ═══════════════════════════════════════════════

struct PatternResult {
    std::string name;
    std::string detail;
    bool passed;
};

inline void printPatternSummary(const std::vector<PatternResult>& results) {
    std::vector<TableCol> cols = {
        {"",        2},
        {"Pattern", 11},
        {"Verified", 40}
    };
    printTableHeader(cols);
    for (auto& r : results) {
        std::string tick    = r.passed
            ? Color::BGREEN + "OK" + Color::RESET
            : Color::BRED   + "!!" + Color::RESET;
        std::string nameCol = r.passed
            ? Color::WHITE + r.name + Color::RESET
            : Color::DIM   + r.name + Color::RESET;
        printTableRow(cols, {tick, nameCol, r.detail});
    }
    printTableBot(cols);
}

// ═══════════════════════════════════════════════
//  DIAGNOSTICS CARD
// ═══════════════════════════════════════════════

inline void printDiagCard(
    const std::string& kioskName,
    const std::string& location,
    const std::string& hardware,
    const std::vector<std::string>& modules,
    bool healthy = true)
{
    // Fixed inner width — thin box chars are per-cell so no repeat issue
    const int w = 52;

    auto border = [&](const std::string& l, const std::string& mid, const std::string& r) {
        std::cout << l << repeatStr(tH, w + 2) << r << "\n";
    };

    auto fieldRow = [&](const std::string& label, const std::string& val) {
        int valW = w - (int)label.size();
        if (valW < 0) valW = 0;
        std::cout << tV << " " << Color::DIM << label << Color::RESET
                  << padStr(val, valW) << " " << tV << "\n";
    };

    std::string statusTxt = healthy ? "ONLINE" : "OFFLINE";
    std::string statusCol = healthy
        ? Color::BGREEN + statusTxt + Color::RESET
        : Color::BRED   + statusTxt + Color::RESET;

    border(tTL, "", tTR);

    // title: "  KioskName" left-aligned, "ONLINE" right-aligned
    // visible: 2 + nameLen + spaces + statusLen(6/7) = w+2
    int statusVis = (int)statusTxt.size();
    int nameVis   = (int)kioskName.size();
    int spaces    = w + 2 - 2 - nameVis - statusVis;
    if (spaces < 1) spaces = 1;
    std::cout << tV
              << "  " << Color::BOLD << Color::BCYAN
              << kioskName << Color::RESET
              << std::string(spaces, ' ')
              << statusCol
              << tV << "\n";

    border(tML, "", tMR);
    fieldRow("Location : ", location);
    fieldRow("Hardware : ", hardware);
    for (auto& m : modules)
        fieldRow("Module   : ", m);
    border(tBL, "", tBR);
}

// ═══════════════════════════════════════════════
//  PAYMENT MENU
// ═══════════════════════════════════════════════

inline void printPaymentMenu() {
    std::cout << "\n";
    std::vector<TableCol> cols = {
        {"#", 2}, {"Method", 8}, {"Description", 24}
    };
    printTableHeader(cols);
    printTableRow(cols, {"1", Color::BCYAN   + "UPI"    + Color::RESET, "Instant bank transfer"});
    printTableRow(cols, {"2", Color::YELLOW  + "Card"   + Color::RESET, "Debit / Credit card"});
    printTableRow(cols, {"3", Color::MAGENTA + "Wallet" + Color::RESET, "Digital wallet balance"});
    printTableBot(cols);
}

// ═══════════════════════════════════════════════
//  SECTION LABEL
// ═══════════════════════════════════════════════

inline void printSection(const std::string& label) {
    std::cout << "\n" << Color::CYAN << Color::BOLD
              << "  > " << label << Color::RESET << "\n"
              << Color::DIM << "  " << std::string(55, '-')
              << Color::RESET << "\n";
}

} // namespace AuraCLI