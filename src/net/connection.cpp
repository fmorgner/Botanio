#include "log/logging.h"
#include "net/connection.h"
#include "tls/policy.h"

using namespace std::placeholders;

namespace botanio
  {

  connection::pointer connection::make_connection(tcp_socket && socket, policy const & policy,
                                                  Botan::TLS::Session_Manager & manager, Botan::Credentials_Manager & credentials)
    {
    return pointer{new connection(std::move(socket), policy, manager, credentials)};
    }

  void connection::start()
    {
    do_read();
    }

  void connection::abort()
    {
    auto error = asio::error_code{};

    m_socket.shutdown(m_socket.shutdown_both, error);
    m_socket.close(error);
    }

  void connection::send(std::string const & data)
    {
    m_tls.send(data);
    }

  connection::connection(tcp_socket && socket, policy const & policy, Botan::TLS::Session_Manager & manager,
                         Botan::Credentials_Manager & credentials)
    : m_socket{std::move(socket)}, m_policy{policy},
      m_tls{[&](auto && data, auto && bytes) { handle_outgoing_data(data, bytes); },
            [&](auto && data, auto && bytes) { handle_incoming_data(data, bytes); },
            [&](auto && alert, auto && data, auto && bytes) { handle_alert(alert, data, bytes); },
            [&](auto && session) { return handle_handshake(session); },
            manager, credentials, policy, m_rng}
    {

    }

  void connection::do_send()
    {
    auto const data = m_outgoing.front();
    m_outgoing.pop_front();

    m_socket.async_send(asio::buffer(data), [this, that = shared_from_this(), &data](auto const &error, auto const) {
      if(!error)
        {
        if(m_outgoing.size())
          {
          do_send();
          }
        }
      else
        {
        abort();
        }
      });
    }

  void connection::do_read()
    {
    m_socket.async_receive(asio::buffer(m_incoming), [this, that = shared_from_this()](auto const & error, auto const bytes) {
      if(!error)
        {
        log_array(std::clog, m_incoming, bytes);

        m_tls.received_data(m_incoming.data(), bytes);
        do_read();
        }
      else
        {
        abort();
        }
      });
    }

  void connection::handle_outgoing_data(uint8_t const * const data, size_t size)
    {
    m_outgoing.emplace_back((char const * const)data, size);
    do_send();
    }

  void connection::handle_incoming_data(uint8_t const * const data, size_t size)
    {
    send({(char const * const)data, size});
    }

  void connection::handle_alert(Botan::TLS::Alert const &, uint8_t const * const, size_t const)
    {

    }

  bool connection::handle_handshake(Botan::TLS::Session const &)
    {
    return false;
    }

  }

