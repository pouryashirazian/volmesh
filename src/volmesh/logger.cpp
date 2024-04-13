
#include "volmesh/logger.h"

namespace volmesh {

  void SetLogFormat() {
    spdlog::set_pattern("[%^%l%$] [%Y-%m-%d %H:%M:%S.%e] [%s:%#] %v");
    spdlog::set_level(spdlog::level::debug);
  }

}