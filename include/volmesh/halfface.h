//-----------------------------------------------------------------------------
// Copyright (c) Pourya Shirazian
// All rights reserved.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.
//-----------------------------------------------------------------------------

#pragma once

#include "volmesh/halfedge.h"
#include "volmesh/multikeyhash.h"

#include <array>

namespace volmesh {

/**
 * @class HalfFace
 * @brief Represents a face consisting of a fixed number of half-edges in a mesh or graph structure.
 *
 * The HalfFace class stores a set of half-edges that define the boundaries of a face.
 *
 * @tparam NumEdges The number of edges (or half-edges) that make up the face.
 */
template <int NumEdges>
class HalfFace {
public:
  /**
   * @brief Type definition for an array of half-edge indices.
   *
   * This is an array of size NumEdges, where each element is a HalfEdgeIndex.
   */
  typedef std::array<HalfEdgeIndex, NumEdges> HalfEdgeIndexArray;

  /**
   * @brief Default constructor for the HalfFace class.
   *
   * Initializes an empty HalfFace object.
   */
  HalfFace();

  /**
   * @brief Copy constructor for the HalfFace class.
   *
   * Initializes a new HalfFace object from an existing one.
   *
   * @param rhs The HalfFace object to copy from.
   */
  HalfFace(const HalfFace& rhs);

  /**
   * @brief Constructs a HalfFace with the specified array of half-edge indices.
   *
   * @param in_hedges The array of half-edge indices that define the face.
   */
  explicit HalfFace(const HalfEdgeIndexArray& in_hedges);

  /**
   * @brief Destructor for the HalfFace class.
   *
   * Cleans up resources used by the HalfFace object.
   */
  ~HalfFace();

  /**
   * @brief Copies the contents of another HalfFace object.
   *
   * Copies the half-edge indices from the given HalfFace object.
   *
   * @param rhs The HalfFace object to copy from.
   */
  void copyFrom(const HalfFace& rhs);

  /**
   * @brief Returns the half-edge index at the specified position.
   *
   * @param i The index of the half-edge to retrieve.
   * @return The half-edge index at the specified position.
   */
  const HalfEdgeIndex& halfEdgeIndex(const int i) const;

  /**
   * @brief Returns the number of edges in the HalfFace.
   *
   * This static method returns the template parameter `NumEdges`, which defines how many edges this face has.
   *
   * @return The number of edges in the face.
   */
  static int numEdges();

  /**
   * @brief Compares this HalfFace with another for equality.
   *
   * @param rhs The other HalfFace object to compare to.
   * @return True if the half-faces are equal, otherwise false.
   */
  bool equals(const HalfFace& rhs) const;

  /**
   * @brief Assignment operator for the HalfFace class.
   *
   * Assigns the contents of one HalfFace object to another.
   *
   * @param rhs The HalfFace object to assign from.
   * @return A reference to this HalfFace object.
   */
  const HalfFace& operator=(const HalfFace& rhs);

private:
  HalfEdgeIndexArray half_edge_indices_; /**< Array storing the indices of the half-edges that form the face. */
};

  #include "volmesh/halfface.tpp"

}