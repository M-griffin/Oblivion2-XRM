#ifndef ENCRYPTION_HPP
#define ENCRYPTION_HPP

#include <string>

class UtilEncrypt {
public:
    UtilEncrypt() = default;
    ~UtilEncrypt() = default;

    std::string encrypt(const std::string &key, const std::string &salt);

    std::string Pkcs5Pbkdf2(const std::string &key, const std::string &salt);

    std::string generateSalt(const std::string &key, const std::string &salt);

    std::string generateHashString(const std::string &key, const std::string &salt);

    bool compare(const std::string &hash1, const std::string &hash2);
};

#endif
