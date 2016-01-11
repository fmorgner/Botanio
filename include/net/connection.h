#ifndef __BOTANIO__CONNECTION
#define __BOTANIO__CONNECTION

#include "asio/ip/tcp.hpp"

#include "botan/auto_rng.h"
#include "botan/credentials_manager.h"
#include "botan/tls_alert.h"
#include "botan/tls_server.h"
#include "botan/tls_session.h"
#include "botan/tls_session_manager.h"

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

    static pointer make_connection(tcp_socket && socket, policy const & policy, Botan::TLS::Session_Manager & manager,
                                   Botan::Credentials_Manager & credentials);

    void start();
    void abort();

    void send(std::string const & data);

    private:
      connection(tcp_socket && socket, policy const & policy, Botan::TLS::Session_Manager & manager,
                 Botan::Credentials_Manager & credentials);

      void do_send();
      void do_read();

      void handle_outgoing_data(uint8_t const * const data, size_t size);
      void handle_incoming_data(uint8_t const * const data, size_t size);
      void handle_alert(Botan::TLS::Alert const & alert, uint8_t const * const data, size_t const size);
      bool handle_handshake(Botan::TLS::Session const & session);

    private:
      tcp_socket m_socket;
      policy const & m_policy;

      Botan::AutoSeeded_RNG m_rng;
      Botan::TLS::Server m_tls;

      std::array<uint8_t, 128> m_incoming;
      std::deque<std::string> m_outgoing;
    };

  }

#endif

