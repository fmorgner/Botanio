#ifndef __BOTANIO__CONNECTION
#define __BOTANIO__CONNECTION

#include "asio/ip/tcp.hpp"

#include <array>
#include <memory>
#include <queue>
#include <string>

namespace botanio
  {

  struct connection : std::enable_shared_from_this<connection>
    {
    static std::shared_ptr<connection> make_connection(asio::ip::tcp::socket && socket);

    void start();
    void abort();

    void send(std::string const & data);

    private:
      explicit connection(asio::ip::tcp::socket && socket);

      void do_send();
      void do_read();

    private:
      asio::ip::tcp::socket m_socket;

      std::array<char, 128> m_incoming;
      std::queue<std::string> m_outgoing;
    };

  }

#endif

