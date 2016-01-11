#include "log/logger.h"

#include <chrono>
#include <iostream>
#include <memory>
#include <sstream>

namespace botanio
  {

  namespace
    {
    static constexpr char lookup[16] = { '0', '1', '2', '3',
                                         '4', '5', '6', '7',
                                         '8', '9', 'a', 'b',
                                         'c', 'd', 'e', 'f' };

    std::string byte_to_hex(uint8_t const byte)
      {
      using namespace std::literals;
      auto encoded = "00"s;

      encoded[0] = lookup[((byte >> 4) & 0xF)];
      encoded[1] = lookup[(byte & 0x0F)];

      return encoded;
      }

    std::vector<std::string> split(std::string const & message)
      {
      auto parts = std::vector<std::string>{};
      auto buffer = std::string{};
      auto stream = std::istringstream{message};

      while(getline(stream, buffer))
        {
        parts.push_back(std::move(buffer));
        }

      return parts;
      }
    }

  logger::logger(std::ostream & out, log_level const initial, log_level const limit)
    : m_out{out},
      m_initial{initial},
      m_limit{limit}
    {

    }

  logger & logger::operator<<(std::string const & message)
    {
    auto parts = split(message);

    if(m_current >= m_minimum)
      {
      if(!m_logging)
        {
        m_out << prefix();
        m_logging = true;
        }

      for(auto const & part : parts)
        {
        if(m_continuation)
          {
          m_out << '\n' << std::string(prefix().length(), ' ');
          }

        m_out << part;
        m_continuation = parts.size() > 1;
        }
      }

    return *this;
    }

  logger & logger::operator<<(std::vector<uint8_t> const & data)
    {
    auto buf = std::ostringstream{};

    for(size_t index{}; index < data.size(); ++index)
      {
      if(index > 0 && !(index % 16))
        {
        buf << '\n';
        }

      buf << byte_to_hex(data[index]) << ' ';
      }

    return ((*this) << buf.str());
    }

  logger & logger::operator<<(log_level const level)
    {
    switch(m_active)
      {
      case state::set:
        switch(m_field)
          {
          case log_field::none:
            break;
          case log_field::minimum:
            m_minimum = level;
            break;
          }
        m_active = state::base;
      case state::base:
        m_current = level;
      }

    return *this;
    }

  logger & logger::operator<<(log_manip const manipulator)
    {
    switch(manipulator)
      {
      case log_manip::reset:
        m_current = m_initial;
        m_minimum = m_limit;
        m_logging = false;
        m_continuation = false;
        break;
      case log_manip::set:
        m_active = state::set;
        break;
      case log_manip::end:
        if(m_current >= m_minimum)
          {
          m_out << '\n';
          }
        m_active = state::base;
        m_current = m_initial;
        break;
      }

    return *this;
    }

  logger & logger::operator<<(log_field const field)
    {
    if(m_active == state::set)
      {
      m_field = field;
      }

    return *this;
    }

  std::string logger::prefix()
    {
    switch(m_current)
      {
      case log_level::debug:
        return "[DEBUG] ";
      case log_level::info:
        return "[INFO] ";
      case log_level::warn:
        return "[WARNING] ";
      case log_level::error:
        return "[ERROR] ";
      case log_level::fatal:
        return "[FATAL] ";
      }
    }

  }

