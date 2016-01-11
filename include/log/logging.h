#ifndef __BOTANIO_LOG__LOGGING
#define __BOTANIO_LOG__LOGGING

#include <array>
#include <iostream>

namespace botanio
  {

  std::string byte_to_hex(unsigned char const byte);

  template<size_t N>
  void log_array(std::ostream & out, std::array<uint8_t, N> const & data, size_t const length = N)
    {
    out << "Binary data (" << length << " bytes): \n";

    for(int index = 0; index < length; ++index)
      {
      if(index > 0 && !(index % 16))
        {
        out << '\n';
        }

      out << byte_to_hex(data[index]) << " ";
      }

    out << "\n\n";
    }

  }

#endif

