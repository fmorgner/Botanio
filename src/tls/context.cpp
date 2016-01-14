#include "tls/context.h"

namespace botanio
  {

  using Botan::Credentials_Manager;
  using Botan::TLS::Policy;
  using Botan::TLS::Session_Manager;

  context::context(transmitter & connection, Credentials_Manager & credentials, Policy const & policy, Session_Manager & sessions)
    : m_connection{connection},
      m_server{[&](auto && data, auto && size){ m_connection.handle_outgoing_data(data, size); },
               [&](auto && data, auto && size){ m_connection.handle_incoming_data(data, size); },
               [&](auto && alert, auto && data, auto && size){ m_connection.handle_alert(alert, data, size); },
               [&](auto && session){ return m_connection.handle_handshake(session); },
               sessions, credentials, policy, m_rng}

    {

    }

  Botan::TLS::Server & context::server()
    {
    return m_server;
    }

  }

