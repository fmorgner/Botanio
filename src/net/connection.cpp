#include "log/logger.h"
#include "net/connection.h"
#include "tls/policy.h"

#include "asio/write.hpp"

#include <iostream>

using namespace std::placeholders;

namespace botanio
  {

  connection::pointer connection::make_connection(tcp_socket && socket, policy const & policy,
                                                  Botan::TLS::Session_Manager & manager, Botan::Credentials_Manager & credentials,
                                                  logger & logger)
    {
    return pointer{new connection(std::move(socket), policy, manager, credentials, logger)};
    }

  void connection::start()
    {
    do_read();
    }

  void connection::abort()
    {
    auto error = asio::error_code{};

    if(m_socket.is_open())
      {
      m_tls.close();
      }

    m_socket.shutdown(m_socket.shutdown_both, error);
    m_socket.close(error);

    m_logger << log_level::info << "Connection to " << m_address << ":" << std::to_string(m_port) << " closed." << log_manip::end;
    }

  void connection::send(std::string const & data)
    {
    m_tls.send(data);
    }

  connection::connection(tcp_socket && socket, policy const & policy, Botan::TLS::Session_Manager & manager,
                         Botan::Credentials_Manager & credentials, logger & logger)
    : m_socket{std::move(socket)}, m_policy{policy},
      m_tls{[&](auto && data, auto && bytes) { handle_outgoing_data(data, bytes); },
            [&](auto && data, auto && bytes) { handle_incoming_data(data, bytes); },
            [&](auto && alert, auto && data, auto && bytes) { handle_alert(alert, data, bytes); },
            [&](auto && session) { return handle_handshake(session); },
            manager, credentials, policy, m_rng},
      m_logger{logger},
      m_strand{m_socket.get_io_service()},
      m_address{m_socket.remote_endpoint().address().to_string()},
      m_port{m_socket.remote_endpoint().port()}
    {

    }

  void connection::do_send()
    {
    std::vector<uint8_t> const & data = m_outgoing[0];

    asio::async_write(m_socket, asio::buffer(data), m_strand.wrap([that = shared_from_this()](auto const & error, auto const ){
      that->m_outgoing.pop_front();

      if(error)
        {
        that->abort();
        }

      if(!that->m_outgoing.empty())
        {
        that->do_send();
        }
    }));
    }

  void connection::do_read()
    {
    m_socket.async_receive(asio::buffer(m_incoming), m_strand.wrap([that = shared_from_this()](auto const & error,
                                                                                               auto const bytes) {
      if(!error)
        {
        that->m_tls.received_data(that->m_incoming.data(), bytes);
        that->do_read();
        }
      else
        {
        that->abort();
        }
      }));
    }

  void connection::handle_outgoing_data(uint8_t const * const data, size_t size)
    {
    m_strand.post([this, vec = std::vector<uint8_t>{data, data + size}](){
      m_outgoing.push_back(std::move(vec));

      if(!(m_outgoing.size() > 1))
        {
        do_send();
        }
      });
    }

  void connection::handle_incoming_data(uint8_t const * const data, size_t size)
    {
    auto incoming = std::string((char const * const) data, size);
    m_logger << incoming << log_manip::end;

    m_strand.post([this, incoming = std::move(incoming)](){
      send(incoming);
      });
    }

  void connection::handle_alert(Botan::TLS::Alert const & alert, uint8_t const * const, size_t const)
    {
    m_logger << log_level::info << "Received TLS alert '" << alert.type_string() << "'" << log_manip::end;
    }

  bool connection::handle_handshake(Botan::TLS::Session const &)
    {
    m_logger << log_level::info << "Handshake with " << m_address << ":" << std::to_string(m_port) << " sucessful."
             << log_manip::end;

    return false;
    }

  }

