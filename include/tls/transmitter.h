#ifndef __BOTANIO_TLS__TRANSMITTER
#define __BOTANIO_TLS__TRANSMITTER

#include <botan/tls_alert.h>
#include <botan/tls_session.h>

#include <cstdint>
#include <cstddef>

namespace botanio
  {

  struct transmitter
    {

    virtual void handle_outgoing_data(uint8_t const bytes[], size_t const size) = 0;
    virtual void handle_incoming_data(uint8_t const bytes[], size_t const size) = 0;
    virtual void handle_alert(Botan::TLS::Alert const & alert, uint8_t const bytes[], size_t const size) = 0;
    virtual bool handle_handshake(Botan::TLS::Session const & session) = 0;

    };

  }

#endif

