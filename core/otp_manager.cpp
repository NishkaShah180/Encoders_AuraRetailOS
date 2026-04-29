#include "otp_manager.h"
#include <iostream>

// Static storage definition
std::map<std::string, OTPManager::OTPData> OTPManager::otpStore;

// Required definition for constexpr static member
constexpr int OTPManager::EXPIRY_MINUTES;

std::string OTPManager::generateOTP(const std::string& email) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(100000, 999999);

    std::string otp = std::to_string(dis(gen));

    OTPData data;
    data.otp = otp;
    data.expiry = std::chrono::steady_clock::now()
                + std::chrono::minutes(EXPIRY_MINUTES);

    otpStore[email] = data;

    return otp;
}

bool OTPManager::verifyOTP(const std::string& email, const std::string& otp) {
    auto it = otpStore.find(email);

    if (it == otpStore.end()) {
        return false;
    }

    auto now = std::chrono::steady_clock::now();

    // Expired OTP
    if (now > it->second.expiry) {
        otpStore.erase(it);
        return false;
    }

    // Correct OTP
    if (it->second.otp == otp) {
        otpStore.erase(it); // One-time use
        return true;
    }

    return false;
}

void OTPManager::expireOTP(const std::string& email) {
    otpStore.erase(email);
}