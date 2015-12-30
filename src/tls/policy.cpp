#include "tls/policy.h"

namespace botanio
  {

  std::vector<std::string> policy::allowed_ciphers() const
    {
    return {"AES-128/GCM"};
    }

  std::vector<std::string> policy::allowed_signature_hashes() const
    {
    return {"SHA-256"};
    }

  std::vector<std::string> policy::allowed_key_exchange_methods() const
    {
    return {"ECDH"};
    }

  std::vector<std::string> policy::allowed_signature_methods() const
    {
    return {"ECDSA"};
    }

  std::vector<std::string> policy::allowed_ecc_curves() const
    {
    return {"secp256r1"};
    }

  std::vector<std::string> policy::allowed_macs() const
    {
    return {"secp256r1"};
    }

  }

