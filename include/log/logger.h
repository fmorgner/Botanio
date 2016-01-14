#ifndef __BOTANIO_LOG__LOGGER
#define __BOTANIO_LOG__LOGGER

#include <asio/io_service.hpp>
#include <asio/strand.hpp>

#include <cstdint>
#include <iosfwd>
#include <vector>

namespace botanio
  {

  struct logger
    {
    logger(std::ostream & out);

    void debug(std::string const & message);
    void info(std::string const & message);
    void warn(std::string const & message);
    void error(std::string const & message);
    void fatal(std::string const & message);

    private:
      void do_log(std::string const & prefix, std::string const & message);

    private:
      std::ostream & m_out;
    };

  }

#endif

