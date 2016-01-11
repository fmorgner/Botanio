#ifndef __BOTANIO_LISTENER
#define __BOTANIO_LISTENER

#include "tls/credentials.h"
#include "tls/policy.h"

#include "asio/io_service.hpp"
#include "asio/ip/tcp.hpp"

#include <botan/tls_session_manager.h>

#include <memory>
#include <set>

namespace botanio
  {

  struct listener
    {
    explicit listener(asio::io_service &);

    void start(std::uint16_t const port);
    void abort();

    private:
      void do_accept();

    private:
      asio::ip::tcp::acceptor m_acceptor;
      asio::ip::tcp::socket m_temporary;

      credentials m_credentials;
      policy m_policy;

      Botan::TLS::Session_Manager_Noop m_manager{};
      std::set<std::shared_ptr<struct connection>> m_connections{};
    };

  }

#endif

