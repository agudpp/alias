#ifndef ENCRYPTION_CRYPTO_HANDLER_H_
#define ENCRYPTION_CRYPTO_HANDLER_H_

#include <string>

#include <toolbox/config/config.h>

#include <encryption/lib/aes.hpp>


namespace encryption {

class CryptoHandler
{
  public:
    struct Data {
        /**
         * @brief Constructor of the data required
         * @param the_key key of len AES_KEYLEN (32)
         */
        Data(const std::string& the_key = "") : key(the_key) {}

        std::string key;
    };
  public:
    /**
     * @brief Construct it using the provided data
     * @param data the data
     */
    CryptoHandler(const Data& data);

    bool
    encrypt(const std::string& data, std::string& result);

    bool
    decrypt(const std::string& data, std::string& result);

    /**
     * @brief Performs a simple encryption using a given key of len > 0 and returns the result
     *        over parameter
     * @param key the key to use, do not require to be 32 len
     * @param data the data to be encrypted
     * @param result the resulting encrypted data
     * @return true if success | false otherwise
     */
    static bool
    encrypt(const std::string& key, const std::string& data, std::string& result);

    /**
     * @brief Performs a simple decryption using a given key of len > 0 and returns the result
     *        over parameter
     * @param key the key to use, do not require to be 32 len
     * @param data the data to be decrypted
     * @param result the resulting decrypted data
     * @return true if success | false otherwise
     */
    static bool
    decrypt(const std::string& key, const std::string& data, std::string& result);

    /**
     * @brief Helper method for generating a proper random key to be used by this crypto handler
     * @return the non-empty random key string
     */
    static std::string
    generateRandomKey(void);


  private:
    Data data_;
    struct AES_ctx aes_ctx_;
};

}

#endif // CRYPTO_CRYPTO_HANDLER_H_
