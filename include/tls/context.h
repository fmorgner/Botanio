#ifndef __BOTANIO_TLS__CONTEXT
#define __BOTANIO_TLS__CONTEXT

#include "tls/transmitter.h"

#include <botan/auto_rng.h>
#include <botan/credentials_manager.h>
#include <botan/tls_policy.h>
#include <botan/tls_server.h>
#include <botan/tls_session_manager.h>

namespace botanio
  {

  struct context
    {
    context(transmitter & connection,
            Botan::Credentials_Manager & credentials,
            Botan::TLS::Policy const & policy,
            Botan::TLS::Session_Manager & sessions);

    Botan::TLS::Server & server();

    private:
      transmitter & m_connection;

      Botan::TLS::Server m_server;
      Botan::AutoSeeded_RNG m_rng;
    };

  }

#endif

