#ifndef __BOTANIO_NET__OBSERVER
#define __BOTANIO_NET__OBSERVER

#include <memory>
#include <vector>

namespace botanio
  {

  struct connection;

  struct observer
    {

    virtual void handle_closed(std::shared_ptr<connection> origin) = 0;
    virtual void handle_data(std::vector<uint8_t> data, std::shared_ptr<connection> origin) = 0;

    };

  }

#endif

