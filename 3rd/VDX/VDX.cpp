#include "VDX.h"


const uint8_t VDX::s_box[256] = {
    0x63, 0x7c, 0x77, 0x7b, 0xf2, 0x6b, 0x6f, 0xc5, 0x30, 0x01, 0x67, 0x2b, 0xfe, 0xd7, 0xab, 0x76,
    0xca, 0x82, 0xc9, 0x7d, 0xfa, 0x59, 0x47, 0xf0, 0xad, 0xd4, 0xa2, 0xaf, 0x9c, 0xa4, 0x72, 0xc0,
    0xb7, 0xfd, 0x93, 0x26, 0x36, 0x3f, 0xf7, 0xcc, 0x34, 0xa5, 0xe5, 0xf1, 0x71, 0xd8, 0x31, 0x15,
    0x04, 0xc7, 0x23, 0xc3, 0x18, 0x96, 0x05, 0x9a, 0x07, 0x12, 0x80, 0xe2, 0xeb, 0x27, 0xb2, 0x75,
    0x09, 0x83, 0x2c, 0x1a, 0x1b, 0x6e, 0x5a, 0xa0, 0x52, 0x3b, 0xd6, 0xb3, 0x29, 0xe3, 0x2f, 0x84,
    0x53, 0xd1, 0x00, 0xed, 0x20, 0xfc, 0xb1, 0x5b, 0x6a, 0xcb, 0xbe, 0x39, 0x4a, 0x4c, 0x58, 0xcf,
    0xd0, 0xef, 0xaa, 0xfb, 0x43, 0x4d, 0x33, 0x85, 0x45, 0xf9, 0x02, 0x7f, 0x50, 0x3c, 0x9f, 0xa8,
    0x51, 0xa3, 0x40, 0x8f, 0x92, 0x9d, 0x38, 0xf5, 0xbc, 0xb6, 0xda, 0x21, 0x10, 0xff, 0xf3, 0xd2,
    0xcd, 0x0c, 0x13, 0xec, 0x5f, 0x97, 0x44, 0x17, 0xc4, 0xa7, 0x7e, 0x3d, 0x64, 0x5d, 0x19, 0x73,
    0x60, 0x81, 0x4f, 0xdc, 0x22, 0x2a, 0x90, 0x88, 0x46, 0xee, 0xb8, 0x14, 0xde, 0x5e, 0x0b, 0xdb,
    0xe0, 0x32, 0x3a, 0x0a, 0x49, 0x06, 0x24, 0x5c, 0xc2, 0xd3, 0xac, 0x62, 0x91, 0x95, 0xe4, 0x79,
    0xe7, 0xc8, 0x37, 0x6d, 0x8d, 0xd5, 0x4e, 0xa9, 0x6c, 0x56, 0xf4, 0xea, 0x65, 0x7a, 0xae, 0x08,
    0xba, 0x78, 0x25, 0x2e, 0x1c, 0xa6, 0xb4, 0xc6, 0xe8, 0xdd, 0x74, 0x1f, 0x4b, 0xbd, 0x8b, 0x8a,
    0x70, 0x3e, 0xb5, 0x66, 0x48, 0x03, 0xf6, 0x0e, 0x61, 0x35, 0x57, 0xb9, 0x86, 0xc1, 0x1d, 0x9e,
    0xe1, 0xf8, 0x98, 0x11, 0x69, 0xd9, 0x8e, 0x94, 0x9b, 0x1e, 0x87, 0xe9, 0xce, 0x55, 0x28, 0xdf,
    0x8c, 0xa1, 0x89, 0x0d, 0xbf, 0xe6, 0x42, 0x68, 0x41, 0x99, 0x2d, 0x0f, 0xb0, 0x54, 0xbb, 0x16
};
const uint8_t VDX::inv_s_box[256] = {
    0x52, 0x09, 0x6a, 0xd5, 0x30, 0x36, 0xa5, 0x38, 0xbf, 0x40, 0xa3, 0x9e, 0x81, 0xf3, 0xd7, 0xfb,
    0x7c, 0xe3, 0x39, 0x82, 0x9b, 0x2f, 0xff, 0x87, 0x34, 0x8e, 0x43, 0x44, 0xc4, 0xde, 0xe9, 0xcb,
    0x54, 0x7b, 0x94, 0x32, 0xa6, 0xc2, 0x23, 0x3d, 0xee, 0x4c, 0x95, 0x0b, 0x42, 0xfa, 0xc3, 0x4e,
    0x08, 0x2e, 0xa1, 0x66, 0x28, 0xd9, 0x24, 0xb2, 0x76, 0x5b, 0xa2, 0x49, 0x6d, 0x8b, 0xd1, 0x25,
    0x72, 0xf8, 0xf6, 0x64, 0x86, 0x68, 0x98, 0x16, 0xd4, 0xa4, 0x5c, 0xcc, 0x5d, 0x65, 0xb6, 0x92,
    0x6c, 0x70, 0x48, 0x50, 0xfd, 0xed, 0xb9, 0xda, 0x5e, 0x15, 0x46, 0x57, 0xa7, 0x8d, 0x9d, 0x84,
    0x90, 0xd8, 0xab, 0x00, 0x8c, 0xbc, 0xd3, 0x0a, 0xf7, 0xe4, 0x58, 0x05, 0xb8, 0xb3, 0x45, 0x06,
    0xd0, 0x2c, 0x1e, 0x8f, 0xca, 0x3f, 0x0f, 0x02, 0xc1, 0xaf, 0xbd, 0x03, 0x01, 0x13, 0x8a, 0x6b,
    0x3a, 0x91, 0x11, 0x41, 0x4f, 0x67, 0xdc, 0xea, 0x97, 0xf2, 0xcf, 0xce, 0xf0, 0xb4, 0xe6, 0x73,
    0x96, 0xac, 0x74, 0x22, 0xe7, 0xad, 0x35, 0x85, 0xe2, 0xf9, 0x37, 0xe8, 0x1c, 0x75, 0xdf, 0x6e,
    0x47, 0xf1, 0x1a, 0x71, 0x1d, 0x29, 0xc5, 0x89, 0x6f, 0xb7, 0x62, 0x0e, 0xaa, 0x18, 0xbe, 0x1b,
    0xfc, 0x56, 0x3e, 0x4b, 0xc6, 0xd2, 0x79, 0x20, 0x9a, 0xdb, 0xc0, 0xfe, 0x78, 0xcd, 0x5a, 0xf4,
    0x1f, 0xdd, 0xa8, 0x33, 0x88, 0x07, 0xc7, 0x31, 0xb1, 0x12, 0x10, 0x59, 0x27, 0x80, 0xec, 0x5f,
    0x60, 0x51, 0x7f, 0xa9, 0x19, 0xb5, 0x4a, 0x0d, 0x2d, 0xe5, 0x7a, 0x9f, 0x93, 0xc9, 0x9c, 0xef,
    0xa0, 0xe0, 0x3b, 0x4d, 0xae, 0x2a, 0xf5, 0xb0, 0xc8, 0xeb, 0xbb, 0x3c, 0x83, 0x53, 0x99, 0x61,
    0x17, 0x2b, 0x04, 0x7e, 0xba, 0x77, 0xd6, 0x26, 0xe1, 0x69, 0x14, 0x63, 0x55, 0x21, 0x0c, 0x7d
};

const uint8_t VDX::Rcon[11] = {0x00, 0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80, 0x1b, 0x36};

uint8_t VDX::galois_mult(uint8_t a, uint8_t b) {
    uint8_t result = 0;
    for (int i = 0; i < 8; ++i) {
        if (b & 1) {
            result ^= a;
        }
        bool carry = a & 0x80;
        a <<= 1;
        if (carry) {
            a ^= 0x11b;
        }
        b >>= 1;
    }
    return result;
}


void VDX::key_expansion(const std::vector<uint8_t>& key) {
    int key_size = key.size() * 8;
    int Nk = key_size / 32;
    Nr = Nk + 6;
    w.resize(4 * (Nr + 1));
    for (int i = 0; i < Nk; ++i) {
        w[i] = (key[4*i] << 24) | (key[4*i+1] << 16) | (key[4*i+2] << 8) | key[4*i+3];
    }
    
    for (int i = Nk; i < 4 * (Nr + 1); ++i) {
        uint32_t temp = w[i-1];
        if (i % Nk == 0) {
            temp = (temp << 8) | (temp >> 24);

            uint8_t* bytes = reinterpret_cast<uint8_t*>(&temp);
            bytes[0] = s_box[bytes[0]];
            bytes[1] = s_box[bytes[1]];
            bytes[2] = s_box[bytes[2]];
            bytes[3] = s_box[bytes[3]];

            temp ^= (Rcon[i/Nk] << 24);
        } else if (Nk > 6 && i % Nk == 4) { // 添加256位密钥处理
            uint8_t* bytes = reinterpret_cast<uint8_t*>(&temp);
            bytes[0] = s_box[bytes[0]];
            bytes[1] = s_box[bytes[1]];
            bytes[2] = s_box[bytes[2]];
            bytes[3] = s_box[bytes[3]];
        }
        w[i] = w[i - Nk] ^ temp;
    }
}


void VDX::sub_bytes(std::vector<std::vector<uint8_t>>& state) {
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            state[i][j] = s_box[state[i][j]];
        }
    }
}
void VDX::inv_sub_bytes(std::vector<std::vector<uint8_t>>& state) {
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            state[i][j] = inv_s_box[state[i][j]];
        }
    }
}

void VDX::shift_rows(std::vector<std::vector<uint8_t>>& state) {
    uint8_t temp = state[1][0];
    state[1][0] = state[1][1];
    state[1][1] = state[1][2];
    state[1][2] = state[1][3];
    state[1][3] = temp;
    
    temp = state[2][0];
    state[2][0] = state[2][2];
    state[2][2] = temp;
    temp = state[2][1];
    state[2][1] = state[2][3];
    state[2][3] = temp;
    
    temp = state[3][3];
    state[3][3] = state[3][2];
    state[3][2] = state[3][1];
    state[3][1] = state[3][0];
    state[3][0] = temp;
}

void VDX::inv_shift_rows(std::vector<std::vector<uint8_t>>& state) {
    uint8_t temp = state[1][3];
    state[1][3] = state[1][2];
    state[1][2] = state[1][1];
    state[1][1] = state[1][0];
    state[1][0] = temp;

    temp = state[2][0];
    state[2][0] = state[2][2];
    state[2][2] = temp;
    temp = state[2][1];
    state[2][1] = state[2][3];
    state[2][3] = temp;
    
    temp = state[3][0];
    state[3][0] = state[3][1];
    state[3][1] = state[3][2];
    state[3][2] = state[3][3];
    state[3][3] = temp;
}

void VDX::mix_columns(std::vector<std::vector<uint8_t>>& state) {
    uint8_t mix_matrix[4][4] = {
        {2, 3, 1, 1},
        {1, 2, 3, 1},
        {1, 1, 2, 3},
        {3, 1, 1, 2}
    };
    
    for (int col = 0; col < 4; ++col) {
        uint8_t column[4] = {state[0][col], state[1][col], state[2][col], state[3][col]};
        for (int row = 0; row < 4; ++row) {
            state[row][col] = galois_mult(mix_matrix[row][0], column[0]) ^
                             galois_mult(mix_matrix[row][1], column[1]) ^
                             galois_mult(mix_matrix[row][2], column[2]) ^
                             galois_mult(mix_matrix[row][3], column[3]);
        }
    }
}

void VDX::inv_mix_columns(std::vector<std::vector<uint8_t>>& state) {
    uint8_t inv_mix_matrix[4][4] = {
        {0x0e, 0x0b, 0x0d, 0x09},
        {0x09, 0x0e, 0x0b, 0x0d},
        {0x0d, 0x09, 0x0e, 0x0b},
        {0x0b, 0x0d, 0x09, 0x0e}
    };
    
    for (int col = 0; col < 4; ++col) {
        uint8_t column[4] = {state[0][col], state[1][col], state[2][col], state[3][col]};
        for (int row = 0; row < 4; ++row) {
            state[row][col] = galois_mult(inv_mix_matrix[row][0], column[0]) ^
                             galois_mult(inv_mix_matrix[row][1], column[1]) ^
                             galois_mult(inv_mix_matrix[row][2], column[2]) ^
                             galois_mult(inv_mix_matrix[row][3], column[3]);
        }
    }
}

void VDX::add_round_key(std::vector<std::vector<uint8_t>>& state, int round) {
    for (int i = 0; i < 4; ++i) {
        uint32_t round_key = w[round * 4 + i];
        state[0][i] ^= (round_key >> 24) & 0xFF;
        state[1][i] ^= (round_key >> 16) & 0xFF;
        state[2][i] ^= (round_key >> 8) & 0xFF;
        state[3][i] ^= round_key & 0xFF;
    }
}

std::vector<uint8_t> VDX::encrypt_block(const std::vector<uint8_t>& block) {
    std::vector<std::vector<uint8_t>> state(4, std::vector<uint8_t>(4));
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            state[j][i] = block[i * 4 + j];
        }
    }
    
    add_round_key(state, 0);
    
    for (int round = 1; round < Nr; ++round) {
        sub_bytes(state);
        shift_rows(state);
        mix_columns(state);
        add_round_key(state, round);
    }
    
    sub_bytes(state);
    shift_rows(state);
    add_round_key(state, Nr);

    std::vector<uint8_t> encrypted_block(16);
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            encrypted_block[i * 4 + j] = state[j][i];
        }
    }
    
    return encrypted_block;
}

std::vector<uint8_t> VDX::decrypt_block(const std::vector<uint8_t>& block) {
    std::vector<std::vector<uint8_t>> state(4, std::vector<uint8_t>(4));
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            state[j][i] = block[i * 4 + j];
        }
    }
    
    add_round_key(state, Nr);

    for (int round = Nr - 1; round > 0; --round) {
        inv_shift_rows(state);
        inv_sub_bytes(state);
        add_round_key(state, round);
        inv_mix_columns(state);
    }
    
    inv_shift_rows(state);
    inv_sub_bytes(state);
    add_round_key(state, 0);
    
    std::vector<uint8_t> decrypted_block(16);
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            decrypted_block[i * 4 + j] = state[j][i];
        }
    }
    
    return decrypted_block;
}

std::vector<uint8_t> VDX::zero_pad(const std::vector<uint8_t>& data) {
    std::size_t pad_length = 16 - (data.size() % 16);
    std::vector<uint8_t> padded = data;
    padded.insert(padded.end(), pad_length, 0x00);
    return padded;
}

std::vector<uint8_t> VDX::unpad(const std::vector<uint8_t>& data) {
    size_t last_non_zero = data.size();
    while (last_non_zero > 0 && data[last_non_zero - 1] == 0) {
        last_non_zero--;
    }
    return std::vector<uint8_t>(data.begin(), data.begin() + last_non_zero);
}

VDX::VDX(const std::vector<uint8_t>& key) {
    if (key.size() != 16 && key.size() != 24 && key.size() != 32) {
        throw std::invalid_argument("密钥长度必须为16、24或32字节（128、192或256位）");
    }
    key_expansion(key);
}

VDX::VDX(const std::string &keyStr)
{
    std::vector<uint8_t> key;
    for(int i=0;i<keyStr.length();i++){
        key.push_back(keyStr[i]);
    }
    if (key.size() != 16 && key.size() != 24 && key.size() != 32) {
        throw std::invalid_argument("密钥长度必须为16、24或32字节（128、192或256位）");
    }
    key_expansion(key);
}

std::vector<uint8_t> VDX::encrypt(const std::vector<uint8_t>& plaintext) {
    std::vector<uint8_t> padded = zero_pad(plaintext);
    std::vector<uint8_t> ciphertext;
    
    for (std::size_t i = 0; i < padded.size(); i += 16) {
        std::vector<uint8_t> block(padded.begin() + i, padded.begin() + i + 16);
        std::vector<uint8_t> encrypted = encrypt_block(block);
        ciphertext.insert(ciphertext.end(), encrypted.begin(), encrypted.end());
    }
    
    return ciphertext;
}

std::vector<uint8_t> VDX::encrypt(const std::string &plaintext){
    return encrypt(std::vector<uint8_t>(plaintext.begin(), plaintext.end()));
}

std::vector<uint8_t> VDX::decrypt(const std::vector<uint8_t>& ciphertext) {
    if (ciphertext.size() % 16 != 0) {
        throw std::invalid_argument("密文长度必须是16的倍数");
    }
    
    std::vector<uint8_t> plaintext;
    
    for (std::size_t i = 0; i < ciphertext.size(); i += 16) {
        std::vector<uint8_t> block(ciphertext.begin() + i, ciphertext.begin() + i + 16);
        std::vector<uint8_t> decrypted = decrypt_block(block);
        plaintext.insert(plaintext.end(), decrypted.begin(), decrypted.end());
    }
    return unpad(plaintext);
}
std::vector<uint8_t> VDX::decrypt(const std::string &ciphertext) {
    return decrypt(std::vector<uint8_t>(ciphertext.begin(), ciphertext.end()));
}
std::string VDX::decryptToString(const std::vector<uint8_t>& ciphertext) {
    std::vector<uint8_t> plaintext = decrypt(ciphertext);
    return std::string(plaintext.begin(), plaintext.end());
}
std::string VDX::to_hex(const std::vector<uint8_t>& data) {
    std::stringstream ss;
    ss << std::hex << std::setfill('0');
    for (uint8_t byte : data) {
        ss << std::setw(2) << static_cast<int>(byte);
    }
    return ss.str();
}
std::vector<uint8_t> VDX::from_hex(const std::string& hex) {
    std::vector<uint8_t> bytes;
    
    // 检查字符串长度是否为偶数
    if (hex.length() % 2 != 0) {
        throw std::invalid_argument("十六进制字符串长度必须为偶数");
    }
    
    // 检查是否包含非法字符
    for (char c : hex) {
        if (!isxdigit(static_cast<unsigned char>(c))) {
            throw std::invalid_argument("十六进制字符串包含非法字符");
        }
    }
    
    for (size_t i = 0; i < hex.length(); i += 2) {
        std::string byteString = hex.substr(i, 2);
        try {
            uint8_t byte = static_cast<uint8_t>(std::stoi(byteString, nullptr, 16));
            bytes.push_back(byte);
        } catch (const std::exception& e) {
            throw std::invalid_argument("解析十六进制字符串失败: " + std::string(e.what()));
        }
    }
    
    return bytes;
}