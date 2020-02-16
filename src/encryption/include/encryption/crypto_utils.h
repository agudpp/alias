#ifndef ENCRYPTION_CRYPTO_UTILS_H_
#define ENCRYPTION_CRYPTO_UTILS_H_

#include <cstdint>
#include <string>

namespace encryption {


class CryptoUtils
{
  public:

    static void
    addPCKS7Padding(std::string& data, std::uint32_t block_size = 16);

    static bool
    unpadPCKS7(std::string& data, std::uint32_t block_size = 16);
};

}

#endif // ENCRYPTION_CRYPTO_UTILS_H_

