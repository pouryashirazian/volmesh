//-----------------------------------------------------------------------------
// Copyright (c) Pourya Shirazian
// All rights reserved.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.
//-----------------------------------------------------------------------------

#pragma once

#include <stdint.h>

namespace volmesh {

  static const uint32_t kSentinelIndex = -1;

/**
 * @class BaseIndex
 * @brief A base class for index types that encapsulates a uint32_t index value.
 *
 * The BaseIndex class provides basic operations and comparisons for an index.
 */
class BaseIndex {
public:
  /**
   * @brief Default constructor for the BaseIndex class.
   *
   * Initializes the index to a sentinel value.
   */
  BaseIndex() : index_(kSentinelIndex) {}

  /**
   * @brief Constructs a BaseIndex with the specified index value.
   *
   * @param index The index value to store.
   */
  explicit BaseIndex(const uint32_t index) : index_(index) {}

  /**
   * @brief Copy constructor for the BaseIndex class.
   *
   * Initializes a new BaseIndex object from an existing one.
   *
   * @param rhs The BaseIndex object to copy from.
   */
  BaseIndex(const BaseIndex& rhs);

  /**
   * @brief Checks if the index is valid.
   *
   * A valid index is not equal to the sentinel value.
   *
   * @return True if the index is valid, false otherwise.
   */
  bool valid() const;

  /**
   * @brief Returns the stored index value.
   *
   * @return The index value.
   */
  uint32_t get() const;

  /**
   * @brief Assignment operator for the BaseIndex class.
   *
   * Assigns the contents of one BaseIndex object to another.
   *
   * @param rhs The BaseIndex object to assign from.
   * @return A reference to this BaseIndex object.
   */
  BaseIndex& operator=(const BaseIndex& rhs);

  /**
   * @brief Equality operator for BaseIndex.
   *
   * Compares two BaseIndex objects for equality based on their index values.
   *
   * @param rhs The BaseIndex object to compare to.
   * @return True if the indices are equal, false otherwise.
   */
  bool operator==(const BaseIndex& rhs) const;

  /**
   * @brief Less-than operator for BaseIndex.
   *
   * Compares two BaseIndex objects to determine if one is less than the other.
   *
   * @param rhs The BaseIndex object to compare to.
   * @return True if this index is less than the other, false otherwise.
   */
  bool operator<(const BaseIndex& rhs) const;

  /**
   * @brief Greater-than operator for BaseIndex.
   *
   * Compares two BaseIndex objects to determine if one is greater than the other.
   *
   * @param rhs The BaseIndex object to compare to.
   * @return True if this index is greater than the other, false otherwise.
   */
  bool operator>(const BaseIndex& rhs) const;

  /**
   * @brief Implicit conversion operator to uint32_t.
   *
   * Converts the BaseIndex object to its underlying uint32_t value.
   *
   * @return The index value as a uint32_t.
   */
  inline operator uint32_t() const {
    return index_;
  }

private:
  uint32_t index_; /**< The index value stored by the BaseIndex object. */
};

/**
 * @class VertexIndex
 * @brief A class representing a vertex index, derived from BaseIndex.
 */
class VertexIndex : public BaseIndex {
public:
  /**
   * @brief Constructs a VertexIndex with the specified index value.
   *
   * @param index The index value to store.
   */
  explicit VertexIndex(const uint32_t index) : BaseIndex(index) {}

  /**
   * @brief Factory method to create a VertexIndex.
   *
   * @param index The index value to store.
   * @return A VertexIndex object.
   */
  static VertexIndex create(const uint32_t index);
};

/**
 * @class HalfEdgeIndex
 * @brief A class representing a half-edge index, derived from BaseIndex.
 */
class HalfEdgeIndex : public BaseIndex {
public:
  /**
   * @brief Constructs a HalfEdgeIndex with the specified index value.
   *
   * @param index The index value to store.
   */
  explicit HalfEdgeIndex(const uint32_t index) : BaseIndex(index) {}

  /**
   * @brief Factory method to create a HalfEdgeIndex.
   *
   * @param index The index value to store.
   * @return A HalfEdgeIndex object.
   */
  static HalfEdgeIndex create(const uint32_t index);
};

/**
 * @class HalfFaceIndex
 * @brief A class representing a half-face index, derived from BaseIndex.
 */
class HalfFaceIndex : public BaseIndex {
public:
  /**
   * @brief Constructs a HalfFaceIndex with the specified index value.
   *
   * @param index The index value to store.
   */
  explicit HalfFaceIndex(const uint32_t index) : BaseIndex(index) {}

  /**
   * @brief Factory method to create a HalfFaceIndex.
   *
   * @param index The index value to store.
   * @return A HalfFaceIndex object.
   */
  static HalfFaceIndex create(const uint32_t index);
};

/**
 * @class CellIndex
 * @brief A class representing a cell index, derived from BaseIndex.
 */
class CellIndex : public BaseIndex {
public:
  /**
   * @brief Constructs a CellIndex with the specified index value.
   *
   * @param index The index value to store.
   */
  explicit CellIndex(const uint32_t index) : BaseIndex(index) {}

  /**
   * @brief Factory method to create a CellIndex.
   *
   * @param index The index value to store.
   * @return A CellIndex object.
   */
  static CellIndex create(const uint32_t index);
};

}