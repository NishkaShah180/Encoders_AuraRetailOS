#pragma once
#include <string>
#include <chrono>
#include <random>
#include <map>

class OTPManager {
public:
    static std::string generateOTP(const std::string& email);
    static bool verifyOTP(const std::string& email, const std::string& otp);
    static void expireOTP(const std::string& email);

private:
    struct OTPData {
        std::string otp;
        std::chrono::steady_clock::time_point expiry;
    };

    static std::map<std::string, OTPData> otpStore;

    // OTP valid for 5 minutes
    static constexpr int EXPIRY_MINUTES = 5;
};