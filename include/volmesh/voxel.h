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
#include <stdint.h>

namespace volmesh {

/**
 * @brief Class representing a voxel (3D cube) in a volumetric mesh.
 *
 * The Voxel class provides methods for manipulating and retrieving information
 * about the geometry of a voxel, including its vertices, volume, surface area,
 * and centroid.
 */
class Voxel {
public:
  /// The number of vertices per cell in a voxel.
  static const int kNumVerticesPerCell = 8;

  /// The number of faces on a voxel.
  static const int kNumFaces = 6;

  /// The number of edges on a voxel.
  static const int kNumEdges = 12;

  /// The number of fitting tetrahedra within a voxel.
  static const int kNumFittingTetrahedra = 6;

  /// Array type for storing voxel vertices.
  typedef Eigen::Matrix<real_t, 3, kNumVerticesPerCell> VoxelVertexArray;

  /// Enumeration for vertex location IDs in the voxel.
  enum VertexLocationId : int {
      LBN = 0, ///< Left Bottom Near
      LBF = 1, ///< Left Bottom Far
      LTN = 2, ///< Left Top Near
      LTF = 3, ///< Left Top Far
      RBN = 4, ///< Right Bottom Near
      RBF = 5, ///< Right Bottom Far
      RTN = 6, ///< Right Top Near
      RTF = 7  ///< Right Top Far
  };

  /// Enumeration for the axes of the voxel.
  enum VoxelAxis : int {
      X = 0, ///< X-axis
      Y = 1, ///< Y-axis
      Z = 2  ///< Z-axis
  };

  /**
   * @brief Default constructor.
   *
   * Initializes an empty Voxel object.
   */
  Voxel();

  /**
   * @brief Copy constructor.
   *
   * Creates a new Voxel object as a copy of another Voxel object.
   * @param rhs The Voxel object to copy from.
   */
  Voxel(const Voxel& rhs);

  /**
   * @brief Constructor that initializes the voxel with specified vertices.
   *
   * @param vertices The vertex positions for the voxel.
   */
  explicit Voxel(const VoxelVertexArray& vertices);

  /**
   * @brief Destructor for Voxel.
   */
  ~Voxel();

  /**
   * @brief Returns the vertices of the voxel.
   *
   * @return A constant reference to the array of vertices.
   */
  const VoxelVertexArray& vertices() const;

  /**
   * @brief Returns the vertex at the specified vertex ID.
   *
   * @param vertex_id The ID of the vertex.
   * @return A vec3 representing the coordinates of the vertex.
   */
  vec3 vertex(const int vertex_id) const;

  /**
   * @brief Returns the length of the voxel along a specified axis.
   *
   * @param axis The axis for which to retrieve the length.
   * @return The length along the specified axis.
   */
  real_t axisLength(const VoxelAxis axis) const;

  /**
   * @brief Returns the surface area of the voxel.
   *
   * @return The surface area of the voxel.
   */
  real_t surfaceArea() const;

  /**
   * @brief Returns the centroid of the voxel.
   *
   * @return A vec3 representing the coordinates of the centroid.
   */
  vec3 centroid() const;

  /**
   * @brief Returns the volume of the voxel.
   *
   * @return The volume of the voxel.
   */
  real_t volume() const;

  /**
   * @brief Creates a voxel given a center and axis lengths.
   *
   * @param center The center position of the voxel.
   * @param axis_lengths The lengths along each axis to define the voxel size.
   * @return A new Voxel object created with the specified parameters.
   */
  static Voxel CreateVoxel(const vec3& center,
                           const vec3& axis_lengths);

private:
  /// Array of vertices defining the voxel.
  VoxelVertexArray vertices_;
};

}