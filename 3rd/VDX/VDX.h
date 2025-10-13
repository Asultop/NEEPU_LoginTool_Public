#ifndef VDX_H
#define VDX_H

#include <vector>
#include <cstdint>
#include <string>
#include <stdexcept>
#include <iomanip>
#include <sstream>

class VDX {
private:
    static const uint8_t s_box[256];
    static const uint8_t Rcon[11];
    std::vector<uint32_t> w; // 轮密钥
    int Nr; // 轮数

    uint8_t galois_mult(uint8_t a, uint8_t b);
    void key_expansion(const std::vector<uint8_t>& key);
    void sub_bytes(std::vector<std::vector<uint8_t>>& state);
    void shift_rows(std::vector<std::vector<uint8_t>>& state);
    void mix_columns(std::vector<std::vector<uint8_t>>& state);
    void add_round_key(std::vector<std::vector<uint8_t>>& state, int round);
    std::vector<uint8_t> encrypt_block(const std::vector<uint8_t>& block);
    std::vector<uint8_t> zero_pad(const std::vector<uint8_t>& data);

public:
    VDX(const std::vector<uint8_t>& key);
    VDX(const std::string& keyStr);
    std::vector<uint8_t> encrypt(const std::vector<uint8_t>& plaintext);
    std::vector<uint8_t> encrypt(const std::string& plaintext);
    static std::string to_hex(const std::vector<uint8_t>& data);

public:
    std::vector<uint8_t> decrypt(const std::vector<uint8_t>& ciphertext);
    std::vector<uint8_t> decrypt(const std::string& ciphertext);
    std::string decryptToString(const std::vector<uint8_t>& ciphertext);
    static std::vector<uint8_t> from_hex(const std::string& hex);
private:
    static const uint8_t inv_s_box[256];
    void inv_sub_bytes(std::vector<std::vector<uint8_t>>& state);
    void inv_shift_rows(std::vector<std::vector<uint8_t>>& state);
    void inv_mix_columns(std::vector<std::vector<uint8_t>>& state);
    std::vector<uint8_t> decrypt_block(const std::vector<uint8_t>& block);
    std::vector<uint8_t> unpad(const std::vector<uint8_t>& data);
};

#endif // VDX_H
