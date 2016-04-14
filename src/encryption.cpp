#include "encryption.hpp"

#include <openssl/evp.h>

#include <iostream>
#include <string>
#include <sstream>
#include <iomanip>

Encrypt::Encrypt()
{
}

Encrypt::~Encrypt()
{
}


/**
 * @brief Unsigned Char to Hex
 * @param inchar
 * @return
 */
std::string Encrypt::unsignedToHex(unsigned char inchar)
{
    std::ostringstream oss(std::ostringstream::out);
    oss << std::setw(2) << std::setfill('0') << std::hex << static_cast<int>(inchar);
    return oss.str();
}


/**
 * @brief SHA1 password encryption
 * @param password
 * @param salt
 */
std::string Encrypt::SHA1(std::string password, std::string salt)
{
    bool EncryptOk = true;

    // Setup Encryption for User Password.
    EVP_MD_CTX mdctx;
    const EVP_MD *md;
    unsigned char md_value[EVP_MAX_MD_SIZE]= {0};
    unsigned int  md_len = 0;
    size_t i;

    OpenSSL_add_all_digests();

    md = EVP_get_digestbyname("SHA1");
    if(!md)
    {
        EncryptOk = false;
    }

    std::string sale_result = "";
    for(unsigned char c : salt)
    {
        printf("%02x", c);
        sale_result += unsignedToHex(c);
    }
    std::cout << " salt: " << sale_result << std::endl;

    std::string result = "";
    if(EncryptOk)
    {
        EVP_MD_CTX_init(&mdctx);
        EVP_DigestInit_ex(&mdctx, md, NULL);
        EVP_DigestUpdate(&mdctx, (char *)salt.c_str(), salt.size());
        EVP_DigestUpdate(&mdctx, (char *)password.c_str(), password.size());
        EVP_DigestFinal_ex(&mdctx, md_value, &md_len);
        EVP_MD_CTX_cleanup(&mdctx);

        // Testing
        //std::cout << "digest: " << std::flush;
        for(i = 0; i < md_len; i++)
        {
            //printf("%02x",md_value[i]);
            result += unsignedToHex(md_value[i]);
        }
        //std::cout << std::endl;
    }
    else
    {
        std::cout << "SHA1 failed" << std::endl;
    }

    EVP_cleanup();
    return result;
}


/**
 * @brief PKCS5_PBKDF2 password encryption
 * @param password
 * @param salt
 */
std::string Encrypt::PKCS5_PBKDF2(std::string password, std::string salt)
{
    size_t i;
    unsigned char *out;

    out = (unsigned char *) malloc(sizeof(unsigned char) * KEK_KEY_LEN);

    std::cout << "pass: " << password << std::endl;
    std::cout << "salt: " << salt << std::endl;
    std::cout << "iteration: " << ITERATION << std::endl;

    std::string sale_result = "";
    for(i = 0; i < salt.size(); i++)
    {
        printf("%02x", salt[i]);
        sale_result += unsignedToHex(salt[i]);
    }
    std::cout << " salt: " << sale_result << std::endl;

    std::string result = "";
    if(PKCS5_PBKDF2_HMAC_SHA1(
        (const char *)password.c_str(), password.size(),
        (const unsigned char *)salt.c_str(), salt.size(),
        ITERATION, KEK_KEY_LEN,
        (unsigned char *)out) != 0)
    {
        for(i = 0; i < KEK_KEY_LEN; i++)
        {
            printf("%02x", out[i]);
            result += unsignedToHex(out[i]);
        }
    }
    else
    {
        std::cout << "PKCS5_PBKDF2_HMAC_SHA1 failed" << std::endl;
    }
    std::cout << " pass: " << result << std::endl;

    free(out);
    return result;
}

