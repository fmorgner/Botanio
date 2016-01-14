#include "log/logger.h"
#include "net/connection.h"
#include "net/listener.h"
#include "tls/policy.h"

namespace botanio
  {

  listener::listener(asio::io_service & runLoop, logger & logger) : m_loop{runLoop}, m_logger{logger}
    {
    m_temporary = connection::create(m_loop, m_credentials, m_policy, m_manager, *this, m_logger);
    }

  void listener::start(std::uint16_t const port)
    {
    m_acceptor.open(asio::ip::tcp::v4());
    m_acceptor.set_option(asio::ip::tcp::socket::reuse_address{true});
    m_acceptor.bind(asio::ip::tcp::endpoint{asio::ip::address{}, port});
    m_acceptor.listen();

    do_run();
    }

  void listener::handle_closed(std::shared_ptr<connection> origin)
    {
    if(m_connections.erase(origin))
      {
      m_logger.debug("Removed connection from connection set.");
      }
    else
      {
      m_logger.debug("Received close notification for unknown connection!");
      }
    }

  void listener::handle_data(std::vector<uint8_t> data, std::shared_ptr<connection> origin)
    {
    if(data.size() >= 2 && data[0] == 'D' && data[1] == 'C')
      {
      origin->abort(connection::initiator::local);
      }
    else
      {
      origin->write(data);
      }
    }

  void listener::do_run()
    {
    for(size_t threadIndex = 0; threadIndex < std::thread::hardware_concurrency(); ++threadIndex)
      {
      m_threads.emplace_back([&]{ m_loop.run(); });
      }

    do_accept();

    for(auto & thread : m_threads)
      {
      thread.join();
      }
    }

  void listener::do_accept()
    {
    m_acceptor.async_accept(m_temporary->socket(), [&](auto const & error) {
      if(!error)
        {
        m_temporary->start();
        m_connections.insert(std::move(m_temporary));
        m_temporary = connection::create(m_loop, m_credentials, m_policy, m_manager, *this, m_logger);

        do_accept();
        }
      });
    }

  }

