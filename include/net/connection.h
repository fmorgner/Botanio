#ifndef __BOTANIO__CONNECTION
#define __BOTANIO__CONNECTION

#include "tls/transmitter.h"
#include "tls/context.h"

#include "asio/strand.hpp"
#include "asio/ip/tcp.hpp"

#include <array>
#include <memory>
#include <queue>
#include <string>
#include <vector>

namespace botanio
  {

  struct logger;

  struct connection : std::enable_shared_from_this<connection>, transmitter
    {
    enum struct initiator : int { local, remote };

    template<typename... Args>
    static std::shared_ptr<connection> create(Args && ...args)
      {
      return std::shared_ptr<connection>(new connection(std::forward<Args>(args)...));
      }

    void start();
    void abort(initiator const origin = initiator::remote);

    void write(std::string const & data);
    void write(std::vector<uint8_t> const & data);

    asio::ip::tcp::socket & socket();

    private:
      connection(asio::io_service & loop,
                 Botan::Credentials_Manager & credentials,
                 Botan::TLS::Policy const & policy,
                 Botan::TLS::Session_Manager & manager,
                 logger & logger);

      void do_send();
      void do_read();

      void handle_outgoing_data(uint8_t const * const data, size_t size) override;
      void handle_incoming_data(uint8_t const * const data, size_t size) override;
      void handle_alert(Botan::TLS::Alert const & alert, uint8_t const * const data, size_t const size) override;
      bool handle_handshake(Botan::TLS::Session const & session) override;

    private:
      asio::ip::tcp::socket m_socket;
      asio::io_service::strand m_strand;
      context m_tls;
      logger & m_logger;

      std::array<uint8_t, 1024> m_incoming;
      std::deque<std::vector<uint8_t>> m_outgoing;

      std::string m_peer;
    };

  }

#endif

