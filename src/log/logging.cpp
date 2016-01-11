#include "log/logging.h"

namespace botanio
  {

  static constexpr char lookup[16] = { '0', '1', '2', '3',
                                       '4', '5', '6', '7',
                                       '8', '9', 'a', 'b',
                                       'c', 'd', 'e', 'f' };

  std::string byte_to_hex(unsigned char const byte)
    {
    using namespace std::literals;
    auto encoded = "00"s;

    encoded[0] = lookup[((byte >> 4) & 0xF)];
    encoded[1] = lookup[(byte & 0x0F)];

    return encoded;
    }

  }

