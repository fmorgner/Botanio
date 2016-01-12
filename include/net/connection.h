#ifndef __BOTANIO__CONNECTION
#define __BOTANIO__CONNECTION

#include "asio/strand.hpp"
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
#include <vector>

namespace botanio
  {

  struct connection : std::enable_shared_from_this<connection>
    {
    using tcp_socket = asio::ip::tcp::socket;
    using policy = struct policy;
    using pointer = std::shared_ptr<connection>;

    static pointer make_connection(tcp_socket && socket, policy const & policy, Botan::TLS::Session_Manager & manager,
                                   Botan::Credentials_Manager & credentials, struct logger & logger);

    void start();
    void abort();

    void send(std::string const & data);

    private:
      connection(tcp_socket && socket, policy const & policy, Botan::TLS::Session_Manager & manager,
                 Botan::Credentials_Manager & credentials, struct logger & logger);

      void do_send();
      void do_read();

      void handle_outgoing_data(uint8_t const * const data, size_t size);
      void handle_incoming_data(uint8_t const * const data, size_t size);
      void handle_alert(Botan::TLS::Alert const & alert, uint8_t const * const data, size_t const size);
      bool handle_handshake(Botan::TLS::Session const & session);

    private:
      tcp_socket m_socket;
      policy const & m_policy;
      Botan::TLS::Server m_tls;
      struct logger & m_logger;

      Botan::AutoSeeded_RNG m_rng;
      std::array<uint8_t, 128> m_incoming;
      std::deque<std::vector<uint8_t>> m_outgoing;
      asio::io_service::strand m_strand;

      std::string const m_address;
      uint16_t const m_port;
    };

  }

#endif

