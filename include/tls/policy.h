#ifndef __BOTANIO_TLS__POLICY
#define __BOTANIO_TLS__POLICY

#include <botan/tls_policy.h>

namespace botanio
  {

  struct policy : Botan::TLS::Policy
    {

    std::vector<std::string> allowed_ciphers() const;
    std::vector<std::string> allowed_signature_hashes() const;
    std::vector<std::string> allowed_key_exchange_methods() const;
    std::vector<std::string> allowed_signature_methods() const;

    std::vector<std::string> allowed_macs() const;
    std::vector<std::string> allowed_ecc_curves() const;

    };

  }

#endif
