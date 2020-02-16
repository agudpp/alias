#include <encryption/crypto_handler.h>

#include <toolbox/debug/debug.h>
#include <toolbox/utils/string_utils.h>

#include <encryption/crypto_utils.h>


/**
 * Definition of how it will work:
 * encryptes messages will look like:
 * [ iv_used | message | padding ]
 * We need to store the IV used when encrypting first since it willl be needed when decrypting
 * later
 */

namespace encryption {

static inline std::string
getRandomIV(void)
{
  return toolbox::StringUtils::generateRandom(AES_BLOCKLEN);
}


CryptoHandler::CryptoHandler(const Data& data) :
  data_(data)
{
  ASSERT(!data.key.empty());
  ASSERT(data.key.size() >= AES_KEYLEN);
  AES_init_ctx(&aes_ctx_, reinterpret_cast<const std::uint8_t*>(data.key.data()));
}

bool
CryptoHandler::encrypt(const std::string& data, std::string& result)
{
  // [ iv_used | messag | padding ]
  const std::string random_iv = getRandomIV();
  result = random_iv;
  result.append(data);
  CryptoUtils::addPCKS7Padding(result, AES_BLOCKLEN);

  AES_ctx_set_iv(&aes_ctx_, reinterpret_cast<const std::uint8_t*>(random_iv.data()));
  assert((result.size() % AES_BLOCKLEN) == 0);
  AES_CBC_encrypt_buffer(&aes_ctx_,
                         reinterpret_cast<std::uint8_t*>(&(result[0])),
                         result.size());
  return true;
}

bool
CryptoHandler::decrypt(const std::string& data, std::string& result)
{
  // for decrypting we need to get first the IV USED at the beginning of the string
  if (data.size() < AES_BLOCKLEN) {
    LOG_ERROR("The data seems to not be encrypted with our crypto cipher");
    return false;
  }
  const std::string iv_used = data.substr(0, AES_BLOCKLEN);
  AES_ctx_set_iv(&aes_ctx_, reinterpret_cast<const std::uint8_t*>(iv_used.data()));

  result = data.substr(AES_BLOCKLEN);
  AES_CBC_decrypt_buffer(&aes_ctx_,
                         reinterpret_cast<std::uint8_t*>(&(result[0])),
                         result.size());
  if (!CryptoUtils::unpadPCKS7(result, AES_BLOCKLEN)) {
    LOG_ERROR("We couldn't unpad the encrypted block, seems to be invalid?");
    return false;
  }
  return true;
}

bool
CryptoHandler::encrypt(const std::string& key, const std::string& data, std::string& result)
{
  std::string the_key = key.size() < AES_KEYLEN
      ? toolbox::StringUtils::duplicateTillNth(key, AES_KEYLEN)
      : key;
  CryptoHandler::Data crypto_data(the_key);
  CryptoHandler crypto_handler(crypto_data);
  return crypto_handler.encrypt(data, result);
}

bool
CryptoHandler::decrypt(const std::string& key, const std::string& data, std::string& result)
{
  std::string the_key = key.size() < AES_KEYLEN
      ? toolbox::StringUtils::duplicateTillNth(key, AES_KEYLEN)
      : key;
  CryptoHandler::Data crypto_data(the_key);
  CryptoHandler crypto_handler(crypto_data);
  return crypto_handler.decrypt(data, result);
}

std::string
CryptoHandler::generateRandomKey(void)
{
  return toolbox::StringUtils::generateRandom(AES_KEYLEN);
}


}
