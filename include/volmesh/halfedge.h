//-----------------------------------------------------------------------------
// Copyright (c) Pourya Shirazian
// All rights reserved.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.
//-----------------------------------------------------------------------------

#pragma once

#include <stdint.h>

#include "volmesh/index.h"

namespace volmesh {

/**
 * @class HalfEdge
 * @brief Represents a half-edge in a mesh or graph structure.
 *
 * The HalfEdge class stores the start and end vertices of a directed edge.
 */
class HalfEdge {
public:

  /**
   * @brief Default constructor for the HalfEdge class.
   *
   * Initializes an empty HalfEdge object.
   */
  HalfEdge();

  /**
   * @brief Copy constructor for the HalfEdge class.
   *
   * Initializes a new HalfEdge object from an existing one.
   *
   * @param rhs The HalfEdge object to copy from.
   */
  HalfEdge(const HalfEdge& rhs);

  /**
   * @brief Constructs a HalfEdge with the specified start and end vertices.
   *
   * @param start The starting vertex index of the half-edge.
   * @param end The ending vertex index of the half-edge.
   */
  explicit HalfEdge(const VertexIndex& start, const VertexIndex& end);

  /**
   * @brief Destructor for the HalfEdge class.
   *
   * Cleans up resources used by the HalfEdge object.
   */
  ~HalfEdge();

  /**
   * @brief Returns the starting vertex of the half-edge.
   *
   * @return The vertex index of the starting vertex.
   */
  const VertexIndex& start() const;

  /**
   * @brief Returns the ending vertex of the half-edge.
   *
   * @return The vertex index of the ending vertex.
   */
  const VertexIndex& end() const;

  /**
   * @brief Compares this half-edge with another for equality.
   *
   * @param rhs The other HalfEdge object to compare to.
   * @return True if the half-edges are equal, otherwise false.
   */
  bool equals(const HalfEdge& rhs) const;

  /**
   * @brief Returns a new HalfEdge with sorted vertex indices.
   *
   * Creates a HalfEdge where the smaller vertex index is the start and the larger is the end.
   *
   * @return A HalfEdge object with sorted vertex indices.
   */
  HalfEdge sortedVertexIds() const;

  /**
   * @brief Assignment operator for the HalfEdge class.
   *
   * Assigns the contents of one HalfEdge object to another.
   *
   * @param rhs The HalfEdge object to assign from.
   * @return A reference to this HalfEdge object.
   */
  const HalfEdge& operator=(const HalfEdge& rhs);

private:
  VertexIndex start_; /**< The starting vertex index of the half-edge. */
  VertexIndex end_;   /**< The ending vertex index of the half-edge. */
};

}