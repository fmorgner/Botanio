#include "log/logger.h"
#include "net/listener.h"

#include <iostream>

using namespace botanio;

int main()
  {
  asio::io_service runLoop{};
  auto log = logger{std::cout, runLoop};
  listener{runLoop, log}.start(8443);
  }

