#include "tls/credentials.h"

#include <botan/auto_rng.h>
#include <botan/pkcs8.h>

namespace botanio
  {

  credentials::credentials(std::string const & certificate, std::string const & key)
    {
    m_key.reset(Botan::PKCS8::load_key(key, m_rng));
    Botan::DataSource_Stream data{certificate};

    while(!data.end_of_data())
      {
      try
        {
        m_certificate = std::make_shared<Botan::X509_Certificate>(data);
        }
      catch(std::exception const & error)
        {

        }
      }
    }

  store_list credentials::trusted_certificate_authorities(std::string const &, std::string const &)
    {
    return {};
    }

  cert_list credentials::cert_chain(std::vector<std::string> const &, std::string const &, std::string const &)
    {
    return {*m_certificate};
    }

  Botan::Private_Key * credentials::private_key_for(Botan::X509_Certificate const &, std::string const &, std::string const &)
    {
    return m_key.get();
    }

  }
