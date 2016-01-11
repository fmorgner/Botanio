#ifndef __BOTANIO_LOG__LOGGER
#define __BOTANIO_LOG__LOGGER

#include <asio/io_service.hpp>
#include <asio/strand.hpp>

#include <cstdint>
#include <iosfwd>
#include <vector>

namespace botanio
  {

  enum class log_level : uint8_t { debug, info, warn, error, fatal };
  enum class log_manip : uint8_t { reset, set, end };
  enum class log_field : uint8_t { none, minimum };

  struct logger
    {
    logger(std::ostream & out, asio::io_service & loop, log_level const initial = log_level::info,
           log_level const limit = log_level::debug);

    logger & operator<<(std::string const & message);
    logger & operator<<(std::vector<uint8_t> const & data);
    logger & operator<<(log_level const level);
    logger & operator<<(log_manip const manipulator);
    logger & operator<<(log_field const field);

    private:
      std::string prefix();

    private:
      enum class state { base, set };

    private:
      std::ostream & m_out;
      asio::io_service & m_loop;
      log_level const m_initial;
      log_level const m_limit;

      log_level m_current{m_initial};
      log_level m_minimum{m_limit};

      state m_active{state::base};
      log_field m_field{log_field::none};

      bool m_logging{};
      bool m_continuation{};

      asio::io_service::strand m_strand{m_loop};
    };

  }

#endif

