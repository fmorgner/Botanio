#ifndef __BOTANIO_LISTENER
#define __BOTANIO_LISTENER

#include "asio/io_service.hpp"
#include "asio/ip/tcp.hpp"

#include <memory>
#include <set>

namespace botanio
  {

  struct listener
    {
    explicit listener(asio::io_service &);

    void start(std::uint16_t const port);
    void abort();

    private:
      void do_accept();

    private:
      asio::ip::tcp::acceptor m_acceptor;
      asio::ip::tcp::socket m_temporary;

      std::set<std::shared_ptr<struct connection>> m_connections{};
    };

  }

#endif

