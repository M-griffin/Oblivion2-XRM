#include "util_encrypt.hpp"

#include <openssl/evp.h>
#include <openssl/rand.h>
#include <openssl/crypto.h>

#include <sstream>
#include <iomanip>
#include <vector>

namespace {
    constexpr size_t SALT_BYTES = 16;
    constexpr size_t HASH_BYTES = 32;
    constexpr int PBKDF2_ITERATIONS = 310000;

    std::string toHex(const unsigned char *data, size_t len) {
        std::ostringstream oss;
        for (size_t i = 0; i < len; ++i) {
            oss << std::hex << std::setw(2) << std::setfill('0')
                    << static_cast<int>(data[i]);
        }
        return oss.str();
    }

    std::vector<unsigned char> fromHex(const std::string &hex) {
        std::vector<unsigned char> out(hex.size() / 2);
        for (size_t i = 0; i < out.size(); ++i) {
            out[i] = static_cast<unsigned char>(
                std::stoul(hex.substr(i * 2, 2), nullptr, 16)
            );
        }
        return out;
    }
} // Namespace


std::string UtilEncrypt::generateSalt() {
    unsigned char salt[SALT_BYTES];

    if (RAND_bytes(salt, sizeof(salt)) != 1) {
        throw std::runtime_error("RAND_bytes failed");
    }

    return toHex(salt, sizeof(salt));
}

std::string UtilEncrypt::Pkcs5Pbkdf2(const std::string &key,
                                     const std::string &saltHex) {
    auto salt = fromHex(saltHex);
    std::vector<unsigned char> hash(HASH_BYTES);

    if (PKCS5_PBKDF2_HMAC(
            key.c_str(),
            static_cast<int>(key.size()),
            salt.data(),
            static_cast<int>(salt.size()),
            PBKDF2_ITERATIONS,
            EVP_sha256(),
            static_cast<int>(hash.size()),
            hash.data()) != 1) {
        throw std::runtime_error("PBKDF2 failed");
    }

    std::ostringstream oss;
    oss << PBKDF2_ITERATIONS << ":"
            << saltHex << ":"
            << toHex(hash.data(), hash.size());

    return oss.str();
}

std::string UtilEncrypt::generateHashString(const std::string &key) {
    std::string saltHex = generateSalt();
    return Pkcs5Pbkdf2(key, saltHex);
}

bool UtilEncrypt::compare(const std::string &stored,
                          const std::string &candidate) {
    // Split stored format: iterations:salt:hash
    auto first = stored.find(':');
    auto second = stored.find(':', first + 1);

    if (first == std::string::npos || second == std::string::npos)
        return false;

    int iterations = std::stoi(stored.substr(0, first));
    std::string saltHex = stored.substr(first + 1, second - first - 1);
    std::string hashHex = stored.substr(second + 1);

    auto salt = fromHex(saltHex);
    auto expected = fromHex(hashHex);

    std::vector<unsigned char> actual(expected.size());

    if (PKCS5_PBKDF2_HMAC(
            candidate.c_str(),
            static_cast<int>(candidate.size()),
            salt.data(),
            static_cast<int>(salt.size()),
            iterations,
            EVP_sha256(),
            static_cast<int>(actual.size()),
            actual.data()) != 1) {
        return false;
    }

    return CRYPTO_memcmp(actual.data(),
                         expected.data(),
                         expected.size()) == 0;
}
