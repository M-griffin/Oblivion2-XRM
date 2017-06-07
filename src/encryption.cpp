#include "encryption.hpp"

// For Debugging SSL Version
#include <boost/preprocessor/stringize.hpp>

#include <openssl/engine.h>
#include "openssl/evp.h"

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
 * @brief Handle New OpenSSL v1.01, and Conversions for Older. (Debugging OpenSSL Version)
 * @return 
 */
#pragma message("OPENSSL_VERSION_NUMBER=" BOOST_PP_STRINGIZE(OPENSSL_VERSION_NUMBER))
#if OPENSSL_VERSION_NUMBER < 0x10100000L

EVP_MD_CTX *EVP_MD_CTX_new()
{
    //return (EVP_MD_CTX*)OPENSSL_zalloc(sizeof(EVP_MD_CTX));
    
    // OPENSSL_VERSION_NUMBER = 0x100010cfL Need malloc
    // Use this for now, not sure what version have zalloc.
    return (EVP_MD_CTX*)OPENSSL_malloc(sizeof(EVP_MD_CTX));
}

void EVP_MD_CTX_free(EVP_MD_CTX *ctx)
{
    EVP_MD_CTX_cleanup(ctx);
    OPENSSL_free(ctx);
}

#endif

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
 * @param key
 * @param salt
 */
std::string Encrypt::SHA1(std::string key, std::string salt)
{
    bool EncryptOk = true;

    // Setup Encryption for User Password.
    EVP_MD_CTX *mdctx = EVP_MD_CTX_new();       
    
    const EVP_MD *md;
    
    unsigned char md_value[EVP_MAX_MD_SIZE]= {0};
    unsigned int  md_len = 0;

    OpenSSL_add_all_digests();

    md = EVP_get_digestbyname("SHA1");
    if(!md)
    {
        EncryptOk = false;
    }

    std::string salt_result = "";
    for(unsigned char c : salt)
    {
        salt_result += unsignedToHex(c);
    }

    std::string result = "";
    if(EncryptOk)
    {        
        EVP_MD_CTX_init(mdctx);
        EVP_DigestInit_ex(mdctx, md, NULL);
        EVP_DigestUpdate(mdctx, (char *)salt_result.c_str(), salt_result.size());
        EVP_DigestUpdate(mdctx, (char *)key.c_str(), key.size());
        EVP_DigestFinal_ex(mdctx, md_value, &md_len);     
        
        EVP_MD_CTX_free(mdctx);

        for(size_t i = 0; i < md_len; i++)
        {
            result += unsignedToHex(md_value[i]);
        }
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
 * @param key
 * @param salt
 */
std::string Encrypt::PKCS5_PBKDF2(std::string key, std::string salt)
{
    size_t i;
    unsigned char *out;
    out = (unsigned char *) malloc(sizeof(unsigned char) * SHA512_OUTPUT_BYTES);

    std::string salt_result = "";
    for(i = 0; i < salt.size(); i++)
    {
        salt_result += unsignedToHex(salt[i]);
    }

    std::string result = "";
    if(PKCS5_PBKDF2_HMAC(
        (const char *)key.c_str(), key.size(),
        (const unsigned char *)salt_result.c_str(), salt_result.size(),
        ITERATION,
        EVP_sha512(),
        SHA512_OUTPUT_BYTES,
        (unsigned char *)out) != 0)
    {
        for(i = 0; i < SHA512_OUTPUT_BYTES; i++)
        {
            result += unsignedToHex(out[i]);
        }
    }
    else
    {
        std::cout << "PKCS5_PBKDF2_HMAC failed" << std::endl;
    }

    free(out);
    return result;
}


/**
 * @brief generate salt hash key
 * @param key
 * @param salt
 */
std::string Encrypt::generate_salt(std::string key, std::string salt)
{
    std::string generated_salt = SHA1(key, salt);
    return generated_salt;
}


/**
 * @brief generate password hash key
 * @param key
 * @param salt
 */
std::string Encrypt::generate_password(std::string key, std::string salt)
{
    std::string generated_password = PKCS5_PBKDF2(key, salt);
    return generated_password;
}


/**
 * @brief Case Insensitive compare valid password hash
 * @param hash1
 * @param hash2
 * @return
 */
bool Encrypt::compare(std::string hash1, std::string hash2)
{
    return (hash1.compare(hash2) == 0);
}

