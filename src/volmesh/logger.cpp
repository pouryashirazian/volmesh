//-----------------------------------------------------------------------------
// Copyright (c) Pourya Shirazian
// All rights reserved.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.
//-----------------------------------------------------------------------------

#include "volmesh/logger.h"

namespace volmesh {

  void SetLogFormat() {
    spdlog::set_pattern("[%^%l%$] [%Y-%m-%d %H:%M:%S.%e] [%s:%#] %v");
    spdlog::set_level(spdlog::level::debug);
  }

}