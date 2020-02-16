#include <encryption/crypto_utils.h>


namespace encryption {

void
CryptoUtils::addPCKS7Padding(std::string& data, std::uint32_t block_size)
{
  if (data.empty()) {
    return;
  }
  const std::uint32_t bytes_last_block = data.size() % block_size;
  const std::uint32_t pad_bytes_to_add = (bytes_last_block == block_size) ? block_size : block_size - bytes_last_block;
  data.append(pad_bytes_to_add, static_cast<unsigned char>(pad_bytes_to_add));
}

bool
CryptoUtils::unpadPCKS7(std::string& data, std::uint32_t block_size)
{
  if ((data.size() % block_size) != 0) {
    return false;
  }
  const std::uint32_t pad_bytes_added = static_cast<std::uint32_t>(data.back());
  if (pad_bytes_added == 0 || pad_bytes_added > block_size) {
    return false;
  }
  const std::uint32_t padding_start_pos = data.size() - pad_bytes_added;
  for (std::uint32_t i = padding_start_pos; i < data.size(); i++) {
    if (static_cast<std::uint32_t>(data[i]) != pad_bytes_added) {
      return false;
    }
  }
  data.erase(padding_start_pos);
  return true;
}


}
