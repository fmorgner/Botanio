#ifndef __BOTANIO_LISTENER
#define __BOTANIO_LISTENER

#include "tls/credentials.h"
#include "tls/policy.h"

#include "asio/io_service.hpp"
#include "asio/ip/tcp.hpp"

#include <botan/tls_session_manager.h>

#include <memory>
#include <set>
#include <thread>
#include <vector>

namespace botanio
  {

  struct listener
    {
    explicit listener(asio::io_service &);

    void start(std::uint16_t const port);

    private:
      void do_run();
      void do_accept();

    private:
      asio::io_service & m_loop;

      asio::ip::tcp::acceptor m_acceptor{m_loop};
      asio::ip::tcp::socket m_temporary{m_loop};
      credentials m_credentials{"cert.pem", "key.pem"};
      policy m_policy{};
      Botan::TLS::Session_Manager_Noop m_manager{};
      std::set<std::shared_ptr<struct connection>> m_connections{};
      asio::io_service::work m_dummy{m_loop};
      std::vector<std::thread> m_threads{};
    };

  }

#endif

