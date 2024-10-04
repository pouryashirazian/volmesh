//-----------------------------------------------------------------------------
// Copyright (c) Pourya Shirazian
// All rights reserved.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.
//-----------------------------------------------------------------------------

#pragma once

#include "volmesh/basetypes.h"

#include <array>

namespace volmesh {

/**
 * @brief Class representing a tetrahedron.
 *
 * The Tetrahedra class defines the structure of a single tetrahedron, providing methods
 * for accessing its vertices, computing various geometric properties such as surface area,
 * volume, and aspect ratio, as well as static lookup methods for face and edge indices.
 */
class Tetrahedra {
public:
  /// Number of vertices per tetrahedron.
  static const int kNumVerticesPerCell = 4;

  /// Number of faces per tetrahedron.
  static const int kNumFaces = 4;

  /// Number of edges per tetrahedron.
  static const int kNumEdges = 6;

  /// Type definition for an array of vertices in a tetrahedron.
  typedef Eigen::Matrix<real_t, 3, kNumVerticesPerCell> TetraVertexArray;

  /**
   * @brief Default constructor.
   *
   * Initializes an empty tetrahedron.
   */
  Tetrahedra();

  /**
   * @brief Copy constructor.
   *
   * @param rhs The Tetrahedra instance to copy from.
   */
  Tetrahedra(const Tetrahedra& rhs);

  /**
   * @brief Constructor from an array of vertices.
   *
   * @param vertices The array of vertex positions.
   */
  explicit Tetrahedra(const TetraVertexArray& vertices);

  /**
   * @brief Destructor for Tetrahedra.
   */
  ~Tetrahedra();

  /**
   * @brief Returns the vertex array of the tetrahedron.
   *
   * @return A const reference to the vertex array.
   */
  const TetraVertexArray& vertices() const;

  /**
   * @brief Returns the position of a specific vertex.
   *
   * @param vertex_id The ID of the vertex (0-3).
   * @return The position of the specified vertex as a vec3.
   */
  vec3 vertex(const int vertex_id) const;

  /**
   * @brief Computes the area of a specific face of the tetrahedron.
   *
   * @param face_id The ID of the face (0-3).
   * @return The area of the specified face.
   */
  real_t faceArea(const int face_id) const;

  /**
   * @brief Computes the total surface area of the tetrahedron.
   *
   * @return The surface area of the tetrahedron.
   */
  real_t surfaceArea() const;

  /**
   * @brief Computes the inradius of the tetrahedron.
   *
   * The inradius is the radius of the largest sphere that fits inside the tetrahedron.
   *
   * @ref Shewchuk, J. R. (n.d.). What is a Good Linear Element? Interpolation, Conditioning, and Quality Measures.
   *
   * @return The inradius of the tetrahedron.
   */
  real_t inradius() const;

  /**
   * @brief Computes the circumradius of the tetrahedron.
   *
   * The circumradius is the radius of the circumscribing sphere that passes through all the vertices of the tetrahedron.
   *
   * @return The circumradius of the tetrahedron.
   */
  real_t circumradius() const;

  /**
   * @brief Computes the aspect ratio of the tetrahedron.
   *
   * The aspect ratio is a quality measure of the tetrahedron, comparing its circumradius and inradius.
   *
   * @return The aspect ratio of the tetrahedron.
   */
  real_t aspectRatio() const;

  /**
   * @brief Computes the centroid of the tetrahedron.
   *
   * The centroid is the average of the vertex positions.
   *
   * @return The centroid of the tetrahedron as a vec3.
   */
  vec3 centroid() const;

  /**
   * @brief Computes the determinant of the tetrahedron.
   *
   * The determinant of the tetrahedron's vertex matrix is used for volume computation and other geometric properties.
   *
   * @return The determinant of the tetrahedron.
   */
  real_t determinant() const;

  /**
   * @brief Computes the volume of the tetrahedron.
   *
   * @return The volume of the tetrahedron.
   */
  real_t volume() const;

  /**
   * @brief Static lookup table for the vertex IDs of a specific face.
   *
   * @param face_id The ID of the face (0-3).
   * @return A vec3i containing the vertex IDs of the specified face.
   */
  static vec3i faceVertexIdsLut(const int face_id);

  /**
   * @brief Static lookup table for the half-edge IDs of a specific face.
   *
   * @param face_id The ID of the face (0-3).
   * @return A vec3i containing the half-edge IDs of the specified face.
   */
  static vec3i faceHalfEdgeIdsLut(const int face_id);

  /**
   * @brief Static lookup table for the vertex IDs of a specific edge.
   *
   * @param edge_id The ID of the edge (0-5).
   * @return A vec2i containing the vertex IDs of the specified edge.
   */
  static vec2i edgeVertexIdsLut(const int edge_id);

private:
  /// Array of vertices in the tetrahedron.
  TetraVertexArray vertices_;
};

}