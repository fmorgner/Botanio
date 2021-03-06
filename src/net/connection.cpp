#include "log/logger.h"
#include "net/connection.h"
#include "tls/context.h"

#include "asio/write.hpp"

using namespace std::placeholders;
using namespace std::literals;

using asio::io_service;
using asio::ip::tcp;
using Botan::TLS::Policy;
using Botan::TLS::Session_Manager;
using Botan::Credentials_Manager;

namespace botanio
  {

  void connection::start()
    {
    auto error = asio::error_code{};
    auto const & remote = m_socket.remote_endpoint(error);

    if(!error)
      {
      m_peer = remote.address().to_string() + ":" + std::to_string(remote.port());
      do_read();
      }
    }

  void connection::abort(initiator origin)
    {
    if(m_socket.is_open())
      {
      m_socket.cancel();

      if(origin == initiator::local)
        {
        m_strand.post([that = shared_from_this()]{
          if(that->m_tls.server().is_active())
            {
            that->m_tls.server().close();
            }
          });
        }
      else
        {
        auto ignored = asio::error_code{};
        m_logger.info("Closing connection to '"s + m_peer + "'.");
        m_socket.shutdown(asio::ip::tcp::socket::shutdown_both, ignored);
        m_socket.close(ignored);
        m_observer.handle_closed(shared_from_this());
        }
      }
    }

  void connection::write(std::string const & data)
    {
    m_strand.post([data = std::move(data), that = shared_from_this()]{
      if(that->m_tls.server().is_active())
        {
        that->m_tls.server().send(data);
        }
    });
    }

  void connection::write(std::vector<uint8_t> const & data)
    {
    m_strand.post([data = std::move(data), that = shared_from_this()]{
      if(that->m_tls.server().is_active())
        {
        that->m_tls.server().send(data);
        }
    });
    }

  tcp::socket & connection::socket()
    {
    return m_socket;
    }

  connection::connection(io_service & loop,
                         Credentials_Manager & credentials,
                         Policy const & policy,
                         Session_Manager & sessions,
                         observer & observer,
                         logger & logger)
    : m_socket{loop},
      m_strand{loop},
      m_tls{*this, credentials, policy, sessions},
      m_observer{observer},
      m_logger{logger}
    {

    }

  void connection::do_send()
    {
    auto const & data = m_outgoing[0];

    asio::async_write(m_socket, asio::buffer(data), m_strand.wrap([that = shared_from_this()](auto && error, auto &&){
      that->m_outgoing.pop_front();

      if(!error && !that->m_outgoing.empty())
        {
        that->do_send();
        }

      if(error)
        {
        that->abort();
        }
    }));
    }

  void connection::do_read()
    {
    m_socket.async_receive(asio::buffer(m_incoming), m_strand.wrap([that = shared_from_this()](auto && error, auto && xfered) {
      if(!error)
        {
        that->m_tls.server().received_data(that->m_incoming.data(), xfered);
        that->do_read();
        }
      else
        {
        that->abort();
        }
      }));
    }

  void connection::handle_outgoing_data(uint8_t const data[], size_t size)
    {
    m_outgoing.emplace_back(data, data + size);

    if(!(m_outgoing.size() > 1))
      {
      do_send();
      }
    }

  void connection::handle_incoming_data(uint8_t const data[], size_t size)
    {
    m_logger.debug("Received " + std::to_string(size) + " bytes from '" + m_peer + "'.");
    m_observer.handle_data({data, data + size}, shared_from_this());
    }

  void connection::handle_alert(Botan::TLS::Alert const & alert, uint8_t const * const, size_t const)
    {
    if(alert.type() == alert.CLOSE_NOTIFY)
      {
      m_logger.debug("Received TLS 'close' alert from '"s + m_peer + "'.");
      abort();
      }
    }

  bool connection::handle_handshake(Botan::TLS::Session const &)
    {
    m_logger.info("Session with '"s + m_peer + "' established."s);
    return false;
    }

  }

