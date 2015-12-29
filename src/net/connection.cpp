#include "net/connection.h"

namespace botanio
  {

  connection::connection(asio::ip::tcp::socket && socket) : m_socket{std::move(socket)}
    {

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

