#ifndef __BOTANIO_LISTENER
#define __BOTANIO_LISTENER

#include "net/observer.h"

#include "tls/credentials.h"
#include "tls/policy.h"

#include "asio/io_service.hpp"
#include "asio/ip/tcp.hpp"

#include <botan/tls_session_manager.h>

#include <memory>
#include <set>
#include <vector>
#include <thread>

namespace botanio
  {

  struct logger;
  struct connection;

  struct listener : observer
    {
    listener(asio::io_service &, logger & logger);

    void start(std::uint16_t const port);

    void handle_closed(std::shared_ptr<connection> origin);
    void handle_data(std::vector<uint8_t> data, std::shared_ptr<connection> origin);

    private:
      void do_run();
      void do_accept();
      void do_stats();

    private:
      asio::io_service & m_loop;
      asio::ip::tcp::acceptor m_acceptor{m_loop};
      asio::io_service::work m_dummy{m_loop};

      std::shared_ptr<connection> m_temporary{};

      logger & m_logger;

      credentials m_credentials{"cert.pem", "key.pem"};
      policy m_policy{};
      Botan::TLS::Session_Manager_Noop m_manager{};

      std::set<std::shared_ptr<connection>> m_connections{};

      std::vector<std::thread> m_threads{};
    };

  }

#endif

