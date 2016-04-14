#ifndef ENCRYPTION_HPP
#define ENCRYPTION_HPP

#include <string>

class Encrypt
{

private:
    // PKCS5_PBKDF2_HMAC_SHA1
    #define KEY_LEN      32
    #define KEK_KEY_LEN  20
    #define ITERATION     1

public:

    Encrypt();
    ~Encrypt();

    /**
     * @brief Unsigned Char to Hex
     * @param inchar
     * @return
     */
    std::string unsignedToHex(unsigned char inchar);

    /**
     * @brief SHA1 password encryption
     * @param password
     * @param salt
     */
    std::string SHA1(std::string password, std::string salt);

    /**
     * @brief PKCS5_PBKDF2 password encryption
     * @param password
     * @param salt
     */
    std::string PKCS5_PBKDF2(std::string password, std::string salt);

};

#endif // ENCRYPTION_HPP
