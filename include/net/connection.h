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
    using tcp_socket = asio::ip::tcp::socket;
    using policy = struct policy;
    using pointer = std::shared_ptr<connection>;

    static pointer make_connection(tcp_socket && socket, policy const & policy);

    void start();
    void abort();

    void send(std::string const & data);

    private:
      explicit connection(tcp_socket && socket, policy const & policy);

      void do_send();
      void do_read();

    private:
      tcp_socket m_socket;
      policy const & m_policy;

      std::array<char, 128> m_incoming;
      std::queue<std::string> m_outgoing;
    };

  }

#endif

