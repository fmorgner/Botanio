#include "net/connection.h"

namespace botanio
  {

  connection::pointer connection::make_connection(tcp_socket && socket, policy const & policy)
    {
    return pointer{new connection(std::move(socket), policy)};
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
    m_outgoing.push(data);
    do_send();
    }

  connection::connection(tcp_socket && socket, policy const & policy)
    : m_socket{std::move(socket)},
      m_policy{policy}
    {

    }

  void connection::do_send()
    {
    m_socket.async_send(asio::buffer(m_outgoing.front()), [this, that = shared_from_this()](auto const &, auto const) {
      m_outgoing.pop();

      if(m_outgoing.size())
        {
        do_send();
        }
      });
    }

  void connection::do_read()
    {
    m_socket.async_receive(asio::buffer(m_incoming), [this, that = shared_from_this()](auto const & error, auto const bytes) {
      if(!error)
        {
        auto data = std::string{m_incoming.cbegin(), m_incoming.cbegin() + bytes};
        m_outgoing.push(data);
        do_send();
        do_read();
        }
      else
        {
        abort();
        }
      });
    }

  }

