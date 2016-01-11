#include "net/listener.h"

#include <thread>
#include <vector>

int main()
  {
  asio::io_service runLoop{};
  botanio::listener{runLoop}.start(8443);
  }

