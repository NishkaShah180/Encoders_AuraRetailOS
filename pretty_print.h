#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <iomanip>
#include <sstream>
#ifdef _WIN32
#include <windows.h>
inline void _aura_sleep(int ms) { Sleep(ms); }
#else
#include <unistd.h>
inline void _aura_sleep(int ms) { usleep(ms * 1000); }
#endif

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
    const std::string BLUE    = "\033[34m";
    const std::string BBLUE   = "\033[94m";
    const std::string GREEN   = "\033[32m";
}

// ── Box chars ──────────────────────────────────
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
const std::string tDH = "\xe2\x95\x90"; // ═  double horizontal
const std::string tDTL= "\xe2\x95\x94"; // ╔
const std::string tDTR= "\xe2\x95\x97"; // ╗
const std::string tDBL= "\xe2\x95\x9a"; // ╚
const std::string tDBR= "\xe2\x95\x9d"; // ╝
const std::string tDV = "\xe2\x95\x91"; // ║

// ── Helpers ───────────────────────────────────

inline void delay(int ms) {
    _aura_sleep(ms);
}

inline std::string repeatStr(const std::string& s, int n) {
    std::string r;
    for (int i = 0; i < n; i++) r += s;
    return r;
}

inline int visibleLen(const std::string& s) {
    int len = 0; bool esc = false;
    for (unsigned char c : s) {
        if (c == '\033') { esc = true; continue; }
        if (esc) { if (c == 'm') esc = false; continue; }
        if ((c & 0xC0) != 0x80) len++;
    }
    return len;
}

inline std::string padStr(const std::string& s, int width) {
    int vl = visibleLen(s);
    if (vl >= width) return s;
    return s + std::string(width - vl, ' ');
}

// ═══════════════════════════════════════════════
//  BOOT BANNER
// ═══════════════════════════════════════════════

inline void printBanner() {
    std::cout << Color::BCYAN << Color::BOLD;
    std::cout << "\n" << repeatStr(tDH, 66) << "\n";
    std::cout << "     AURA RETAIL OS  //  SMART CITY KIOSK NETWORK  v2.0\n";
    std::cout << repeatStr(tDH, 66) << "\n";
    std::cout << Color::DIM << Color::WHITE;
    std::cout << "     Powered by Encoders Team  |  OOP Design Patterns Demo\n";
    std::cout << repeatStr(tH, 66) << "\n";
    std::cout << Color::RESET << "\n";
}

inline void printBootStep(const std::string& msg, bool ok = true, int delayMs = 80) {
    delay(delayMs);
    if (ok) {
        std::cout << Color::DIM << "  [" << Color::BGREEN << Color::BOLD << "  OK  "
                  << Color::RESET << Color::DIM << "] " << Color::RESET << msg << "\n";
    } else {
        std::cout << Color::DIM << "  [" << Color::BRED << Color::BOLD << " FAIL "
                  << Color::RESET << Color::DIM << "] " << Color::RESET << msg << "\n";
    }
    std::cout.flush();
}

inline void printBootSection(const std::string& title) {
    std::cout << "\n" << Color::CYAN << Color::BOLD << "  >> " << title << Color::RESET << "\n";
    std::cout << Color::DIM << "  " << repeatStr("-", 60) << Color::RESET << "\n";
}

// ═══════════════════════════════════════════════
//  STATUS TAGS
// ═══════════════════════════════════════════════

inline std::string tag(const std::string& label, const std::string& color) {
    return color + Color::BOLD + "[" + label + "]" + Color::RESET;
}

inline void ok(const std::string& msg) {
    std::cout << "  " << tag("  OK  ", Color::BGREEN) << "  " << msg << "\n";
}
inline void info(const std::string& msg) {
    std::cout << "  " << tag(" INFO ", Color::BCYAN) << "  " << msg << "\n";
}
inline void warn(const std::string& msg) {
    std::cout << "  " << tag(" WARN ", Color::YELLOW) << "  " << msg << "\n";
}
inline void fail(const std::string& msg) {
    std::cout << "  " << tag(" FAIL ", Color::BRED) << "  " << msg << "\n";
}
inline void arrow(const std::string& msg) {
    std::cout << "  " << Color::MAGENTA << "  ---> " << Color::RESET << msg << "\n";
}

// ═══════════════════════════════════════════════
//  SECTION DIVIDERS
// ═══════════════════════════════════════════════

inline void printPhase(const std::string& title) {
    const int totalWidth = 66;
    int textLen = visibleLen(title);
    int pad = totalWidth - textLen - 4; // 4 = "║ " + " ║"
    if (pad < 0) pad = 0;
    
    std::cout << "\n" << Color::CYAN << Color::BOLD;
    std::cout << tDTL << repeatStr(tDH, totalWidth - 2) << tDTR << "\n";
    std::cout << tDV << " " << title << std::string(pad, ' ') << " " << tDV << "\n";
    std::cout << tDBL << repeatStr(tDH, totalWidth - 2) << tDBR << "\n";
    std::cout << Color::RESET;
}

inline void printSection(const std::string& label) {
    std::cout << "\n" << Color::YELLOW << Color::BOLD
              << "  >> " << label << Color::RESET << "\n"
              << Color::DIM << "  " << repeatStr(tH, 60)
              << Color::RESET << "\n";
}

inline void printSectionHeader(const std::string& label) {
    std::cout << "\n" << Color::BCYAN << Color::BOLD;
    std::cout << "  " << tTL << repeatStr(tH, 62) << tTR << "\n";
    int pad = 62 - 2 - (int)label.size();
    std::cout << "  " << tV << "  " << label << std::string(pad < 0 ? 0 : pad, ' ') << tV << "\n";
    std::cout << "  " << tBL << repeatStr(tH, 62) << tBR << "\n";
    std::cout << Color::RESET;
}

// ═══════════════════════════════════════════════
//  TABLE PRIMITIVES
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
//  INVENTORY TABLE (with color-coded stock)
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
        {"Price", 8},
        {"Stock", 6}
    };
    printTableHeader(cols);
    for (int i = 0; i < (int)names.size(); i++) {
        std::string idxStr   = std::to_string(i + 1);
        std::string priceCol = Color::YELLOW + prices[i] + Color::RESET;
        std::string qtyStr   = qtys[i];
        std::string qtyCol;

        if (qtyStr == "-" || qtyStr == "N/A") {
            qtyCol = Color::DIM + " N/A" + Color::RESET;
        } else {
            int q = 0;
            try { q = std::stoi(qtyStr); } catch (...) {}
            if (q == 0)
                qtyCol = Color::BRED + Color::BOLD + "OUT" + Color::RESET;
            else if (q <= 3)
                qtyCol = Color::BRED + qtyStr + " LOW" + Color::RESET;
            else if (q <= 7)
                qtyCol = Color::YELLOW + qtyStr + Color::RESET;
            else
                qtyCol = Color::BGREEN + qtyStr + Color::RESET;
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
    if (txns.empty()) {
        info("No transactions recorded yet.");
        return;
    }
    std::vector<TableCol> cols = {
        {"TXN",    7},
        {"Type",   8},
        {"Item",  16},
        {"Amt",    8},
        {"Method", 7},
        {"Status", 10}
    };
    printTableHeader(cols);
    for (auto& t : txns) {
        std::string statusCol;
        if      (t.status == "Success")  statusCol = Color::BGREEN + tag("SUCCESS", Color::BGREEN) + Color::RESET;
        else if (t.status == "Failed")   statusCol = Color::BRED   + tag("FAILED",  Color::BRED)   + Color::RESET;
        else if (t.status == "Refunded") statusCol = Color::YELLOW + tag("REFUND",  Color::YELLOW) + Color::RESET;
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
        {"",        4},
        {"Pattern", 11},
        {"Verified", 42}
    };
    printTableHeader(cols);
    for (auto& r : results) {
        std::string tick    = r.passed
            ? Color::BGREEN + Color::BOLD + " OK " + Color::RESET
            : Color::BRED   + Color::BOLD + " !! " + Color::RESET;
        std::string nameCol = r.passed
            ? Color::WHITE + r.name + Color::RESET
            : Color::DIM   + r.name + Color::RESET;
        printTableRow(cols, {tick, nameCol, r.detail});
    }
    printTableBot(cols);
}

// ═══════════════════════════════════════════════
//  KIOSK DASHBOARD CARD
// ═══════════════════════════════════════════════

inline void printDiagCard(
    const std::string& kioskName,
    const std::string& location,
    const std::string& hardware,
    const std::vector<std::string>& modules,
    bool healthy = true)
{
    const int w = 54;

    auto line = [&](const std::string& label, const std::string& val, const std::string& valColor = "") {
        std::string valStr = valColor.empty() ? val : (valColor + val + Color::RESET);
        int usedW = (int)label.size() + visibleLen(valStr);
        int pad = w - usedW;
        if (pad < 0) pad = 0;
        std::cout << tV << " " << Color::DIM << label << Color::RESET
                  << valStr << std::string(pad, ' ') << " " << tV << "\n";
    };

    std::string statusTxt = healthy ? "ONLINE" : "OFFLINE";
    std::string statusCol = healthy ? Color::BGREEN : Color::BRED;

    std::cout << tTL << repeatStr(tH, w + 2) << tTR << "\n";

    // Title row
    int nameVis   = (int)kioskName.size();
    int statusVis = (int)statusTxt.size() + 2; // brackets
    int spaces    = w + 2 - 2 - nameVis - statusVis;
    if (spaces < 1) spaces = 1;
    std::cout << tV
              << "  " << Color::BOLD << Color::BCYAN << kioskName << Color::RESET
              << std::string(spaces, ' ')
              << statusCol << Color::BOLD << "[" << statusTxt << "]" << Color::RESET
              << tV << "\n";

    std::cout << tML << repeatStr(tH, w + 2) << tMR << "\n";

    line("  Location : ", location);
    line("  Hardware : ", hardware, Color::BBLUE);
    for (auto& m : modules)
        line("  Module   : ", m, Color::BGREEN);
    std::cout << tBL << repeatStr(tH, w + 2) << tBR << "\n";
}

// ═══════════════════════════════════════════════
//  PAYMENT MENU
// ═══════════════════════════════════════════════

inline void printPaymentMenu() {
    std::cout << "\n";
    printSection("Select Payment Method  [Adapter Pattern]");
    std::vector<TableCol> cols = {
        {"#", 2}, {"Method", 8}, {"Gateway", 30}, {"Status", 8}
    };
    printTableHeader(cols);
    printTableRow(cols, {"1", Color::BCYAN   + "UPI"    + Color::RESET, "Instant Bank Transfer (BHIM/GPay)", tag("LIVE", Color::BGREEN)});
    printTableRow(cols, {"2", Color::YELLOW  + "Card"   + Color::RESET, "Debit / Credit (Visa/Mastercard)", tag("LIVE", Color::BGREEN)});
    printTableRow(cols, {"3", Color::MAGENTA + "Wallet" + Color::RESET, "Digital Wallet (Paytm/PhonePe)",   tag("LIVE", Color::BGREEN)});
    printTableBot(cols);
}

// ═══════════════════════════════════════════════
//  SIM COMPLETE
// ═══════════════════════════════════════════════

inline void printSimComplete() {
    const int totalWidth = 66;
    std::string title = "AURA RETAIL OS  --  SESSION ENDED";
    int textLen = visibleLen(title);
    int pad = totalWidth - textLen - 4;
    
    std::cout << "\n" << Color::BCYAN << Color::BOLD;
    std::cout << tDTL << repeatStr(tDH, totalWidth - 2) << tDTR << "\n";
    std::cout << tDV << " " << std::string(pad/2, ' ') << title << std::string(pad - pad/2, ' ') << " " << tDV << "\n";
    std::cout << tDBL << repeatStr(tDH, totalWidth - 2) << tDBR << "\n";
    std::cout << Color::RESET;
}

} // namespace AuraCLI