#ifndef ENCRYPTION_HPP
#define ENCRYPTION_HPP


#include <boost/smart_ptr/shared_ptr.hpp>

#include <string>

class Encrypt
{

private:

    #define SHA512_OUTPUT_BYTES 64
    #define ITERATION           1000

public:

    explicit Encrypt();
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
    std::string SHA1(std::string key, std::string salt);

    /**
     * @brief PKCS5_PBKDF2 password encryption
     * @param password
     * @param salt
     */
    std::string PKCS5_PBKDF2(std::string key, std::string salt);

    /**
     * @brief generate salt hash key
     * @param password
     * @param salt
     */
    std::string generate_salt(std::string key, std::string salt);

    /**
     * @brief generate password hash key
     * @param password
     * @param salt
     */
    std::string generate_password(std::string key, std::string salt);

    /**
     * @brief Compare valid password hash
     * @param hash1
     * @param hash2
     * @return
     */
    bool compare(std::string hash1, std::string hash2);

};

typedef boost::shared_ptr<Encrypt> encrypt_ptr;

#endif // ENCRYPTION_HPP
