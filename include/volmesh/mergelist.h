//-----------------------------------------------------------------------------
// Copyright (c) Pourya Shirazian
// All rights reserved.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.
//-----------------------------------------------------------------------------

#pragma once

#include "volmesh/basetypes.h"

#include <vector>
#include <mutex>
#include <atomic>

namespace volmesh {

/**
 * @class MergeList
 * @brief A class for managing and merging unique elements based on hashed values.
 *
 * The `MergeList` class maintains a list of unique elements identified by hash values. It provides
 * functionalities for adding elements, checking their existence, and retrieving associated data.
 */
class MergeList {
private:
  /**
   * @brief Deleted copy constructor.
   *
   * Copying of `MergeList` objects is disabled to avoid unintended behavior.
   *
   * @param other The `MergeList` object to copy from (not allowed).
   */
  MergeList(const MergeList& other) = delete;

public:
  /**
   * @brief Constructs a `MergeList` object with a specified capacity.
   *
   * Initializes a `MergeList` with a given capacity for storing unique elements.
   *
   * @param capacity The initial capacity of the `MergeList`.
   */
  MergeList(const std::size_t capacity);

  /**
   * @brief Destructor for the `MergeList` class.
   *
   * Cleans up resources used by the `MergeList`.
   */
  virtual ~MergeList();

  /**
   * @brief Retrieves the capacity of the `MergeList`.
   *
   * @return The maximum number of elements that can be stored in the `MergeList`.
   */
  std::size_t capacity() const;

  /**
   * @brief Resets the `MergeList` to its initial state.
   *
   * Clears all stored elements and resets internal data structures.
   */
  void reset();

  /**
   * @brief Adds a hash and its associated element ID to the `MergeList`.
   *
   * If the hash is not already present, it is added along with its associated element ID.
   *
   * @param hash The hash value representing the element.
   * @param element_id The unique ID of the element.
   */
  void addHash(const std::size_t hash, const uint32_t element_id);

  /**
   * @brief Checks if a hash exists in the `MergeList`.
   *
   * This function checks if a hash value already exists in the list and optionally returns
   * the associated index.
   *
   * @param hash The hash value to check.
   * @param out_index Optional output pointer to store the index if the hash exists.
   * @return True if the hash exists, otherwise false.
   */
  bool exists(const std::size_t hash, uint32_t* out_index = nullptr) const;

  /**
   * @brief Retrieves the number of unique elements in the `MergeList`.
   *
   * @return The number of unique elements stored in the list.
   */
  std::size_t uniqueElements() const;

  /**
   * @brief Retrieves a vector of boolean masks.
   *
   * Returns a vector of boolean values, where each entry represents the existence of a corresponding element.
   *
   * @return A vector of boolean masks.
   */
  std::vector<bool> masks() const;

  /**
   * @brief Retrieves a vector of element indices.
   *
   * Returns a vector containing the indices of the elements in the `MergeList`.
   *
   * @return A vector of element indices.
   */
  std::vector<uint32_t> indices() const;

private:
  std::size_t capacity_ = 0; /**< The capacity of the `MergeList`. */
  std::atomic<uint32_t> next_unique_id_ = {0}; /**< The next unique ID to be assigned. */

  mutable std::mutex hash_id_map_mutex_; /**< Mutex for thread-safe access to the hash map. */
  std::unordered_map<std::size_t, uint32_t> hash_id_map_; /**< Maps hash values to element IDs. */
  std::vector<bool> masks_; /**< A vector of masks indicating active elements. */
  std::vector<uint32_t> indices_; /**< A vector of element indices. */
};

}