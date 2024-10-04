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

/**
 * @brief A hash function object for combining multiple keys into a single hash value.
 *
 * The `MultiKeyHash` structure computes a combined hash value from an array of keys. It uses
 * a variant of the Boost hash_combine function to combine the hash values of each key.
 *
 * @tparam S The size of the array of keys.
 * @tparam T The type of each key in the array (must be hashable).
 */
template <std::size_t S, typename T>
struct MultiKeyHash
{
  /**
   * @brief Combines the hash of a value with a seed.
   *
   * This function combines the hash value of `v` with the current `seed` to create
   * a composite hash value. It uses a hash function on the value and performs bitwise
   * operations to further mix the hash.
   *
   * @param seed The current seed (hash result) to combine with.
   * @param v The value to hash and combine with the seed.
   */
  inline void hash_combine(std::size_t& seed, const T& v) const
  {
    std::hash<T> hasher;
    seed ^= hasher(v) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
  }

  /**
   * @brief Computes a combined hash value from an array of keys.
   *
   * This function iterates through the array of keys and combines their individual hash
   * values into a single hash value using the `hash_combine` function.
   *
   * @param multikey_array The array of keys to hash.
   * @return The combined hash value for the array of keys.
   */
  std::size_t operator()(const std::array<T, S>& multikey_array) const
  {
    std::size_t hash_result = 0;
    // The hash_result from the previous cycles is used as the seed for the next cycle
    for (std::size_t i = 0; i < S; i++) {
      hash_combine(hash_result, multikey_array[i]);
    }
    return hash_result;
  }
};

}