//-----------------------------------------------------------------------------
// Copyright (c) Pourya Shirazian
// All rights reserved.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.
//-----------------------------------------------------------------------------

#pragma once

#include <functional>
#include <cstddef>

namespace volmesh {

  //A custom hash for multiple key scenarios
  template <std::size_t S, typename T>
  struct MultiKeyHash
  {
    inline void hash_combine(std::size_t& seed, const T& v) const
    {
      std::hash<T> hasher;
      seed ^= hasher(v) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
    }

    std::size_t operator()(const std::array<T, S>& multikey_array) const
    {
      std::size_t hash_result = 0;
      //The hash_result from the previous cycles is used as the seed for the next cycle
      for (std::size_t i = 0; i < S; i++) {
        hash_combine(hash_result, multikey_array[i]);
      }
      return hash_result;
    }
  };

}