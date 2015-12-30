#include "net/connection.h"
#include "net/listener.h"
#include "tls/policy.h"

namespace botanio
  {

  listener::listener(asio::io_service & runLoop)
    : m_acceptor{runLoop},
      m_temporary{runLoop}
    {

    }

  void listener::start(std::uint16_t const port)
    {
    m_acceptor.open(asio::ip::tcp::v4());
    m_acceptor.set_option(asio::ip::tcp::socket::reuse_address{true});
    m_acceptor.bind(asio::ip::tcp::endpoint{asio::ip::address{}, port});
    m_acceptor.listen();
    do_accept();
    m_acceptor.get_io_service().run();
    }

  void listener::abort()
    {

    }

  void listener::do_accept()
    {
    m_acceptor.async_accept(m_temporary, [this](auto const & error) {
      if(!error)
        {
        auto inserted = m_connections.insert(connection::make_connection(std::move(m_temporary), policy{}));

        if(inserted.second)
          {
          (*inserted.first)->start();
          }
        }

        do_accept();
      });
    }

  }

