#include "log/logger.h"

#include <chrono>
#include <iostream>
#include <memory>
#include <sstream>

namespace botanio
  {

  logger::logger(std::ostream & out) : m_out{out}
    {

    }

  void logger::debug(std::string const & message)
    {
    do_log("[D]: ", message);
    }

  void logger::info(std::string const & message)
    {
    do_log("[I]: ", message);
    }

  void logger::warn(std::string const & message)
    {
    do_log("[W]: ", message);
    }

  void logger::error(std::string const & message)
    {
    do_log("[E]: ", message);
    }

  void logger::fatal(std::string const & message)
    {
    do_log("[F]: ", message);
    }

  void logger::do_log(std::string const & prefix, std::string const & message)
    {
    m_out << prefix << message << "\n";
    }

  }

