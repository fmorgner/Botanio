#ifndef __BOTANIO_TLS__CREDENTIALS
#define __BOTANIO_TLS__CREDENTIALS

#include <botan/auto_rng.h>
#include <botan/credentials_manager.h>

#include <memory>
#include <string>

namespace botanio
  {

    using cert_list = std::vector<Botan::X509_Certificate>;
    using store_list = std::vector<Botan::Certificate_Store *>;

    struct credentials : Botan::Credentials_Manager
      {

      credentials(std::string const & certificate, std::string const & key);

      store_list trusted_certificate_authorities(std::string const & type, std::string const & context);

      Botan::Private_Key * private_key_for(Botan::X509_Certificate const &, std::string const & type, std::string const & context);

      cert_list cert_chain(std::vector<std::string> const & algos, std::string const & type, std::string const & context);

      private:
        Botan::AutoSeeded_RNG m_rng{};

        std::shared_ptr<Botan::X509_Certificate> m_certificate;
        std::shared_ptr<Botan::Private_Key> m_key;
      };

  }

#endif

