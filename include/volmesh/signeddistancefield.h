//-----------------------------------------------------------------------------
// Copyright (c) Pourya Shirazian
// All rights reserved.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.
//-----------------------------------------------------------------------------

#pragma once

#include "volmesh/trianglemesh.h"

#include <mutex>

namespace volmesh {

/**
 * @class SignedDistanceField
 * @brief A class for representing and computing a signed distance field (SDF).
 *
 * The `SignedDistanceField` class represents a volumetric signed distance field
 * and provides functionality for generating, querying, and saving/loading the SDF.
 */
class SignedDistanceField {
public:
  /**
   * @brief Default constructor.
   *
   * Initializes an empty `SignedDistanceField` object.
   */
  SignedDistanceField();

  /**
   * @brief Copy constructor.
   *
   * Creates a copy of an existing `SignedDistanceField`.
   *
   * @param rhs The `SignedDistanceField` object to copy.
   */
  SignedDistanceField(const SignedDistanceField& rhs);

  /**
   * @brief Destructor.
   *
   * Cleans up resources used by the `SignedDistanceField` object.
   */
  ~SignedDistanceField();

  /**
   * @brief The default voxel size for the SDF.
   */
  static constexpr const real_t kDefaultVoxelSize = 0.5;

  /**
   * @brief Copies data from another `SignedDistanceField` object.
   *
   * This function copies the state of another `SignedDistanceField` object into this one.
   *
   * @param rhs The `SignedDistanceField` to copy data from.
   */
  void copyFrom(const SignedDistanceField& rhs);

  /**
   * @brief Gets the bounding box (AABB) of the SDF.
   *
   * @return The axis-aligned bounding box (AABB) of the SDF.
   */
  AABB bounds() const;

  /**
   * @brief Retrieves the voxel size of the SDF.
   *
   * @return The size of each voxel.
   */
  real_t voxelSize() const;

  /**
   * @brief Retrieves the number of voxels along each axis.
   *
   * @return A `vec3i` representing the number of voxels along each axis (x, y, z).
   */
  vec3i voxelsCount() const;

  /**
   * @brief Retrieves the number of grid points along each axis.
   *
   * @return A `vec3i` representing the number of grid points along each axis (x, y, z).
   */
  vec3i gridPointsCount() const;

  /**
   * @brief Gets the total number of grid points in the SDF.
   *
   * @return The total number of grid points.
   */
  uint64_t totalGridPointsCount() const;

  /**
   * @brief Retrieves the memory usage of the SDF in bytes.
   *
   * @return The total memory usage of the SDF, in bytes.
   */
  uint64_t getTotalMemoryUsageInBytes() const;

  /**
   * @brief Retrieves the ID of a grid point based on its coordinates.
   *
   * Converts grid coordinates to a linear ID for internal storage.
   *
   * @param coords The grid point coordinates.
   * @return The grid point ID corresponding to the given coordinates.
   */
  uint64_t gridPointId(const vec3i& coords) const;

  /**
   * @brief Retrieves the position of a grid point in 3D space.
   *
   * Converts grid coordinates to the physical position of the corresponding grid point.
   *
   * @param coords The grid point coordinates.
   * @return The 3D position of the grid point.
   */
  vec3 gridPointPosition(const vec3i& coords) const;

  /**
   * @brief Retrieves the position and field value of a grid point.
   *
   * Converts grid coordinates to the physical position and signed distance field value
   * at that grid point.
   *
   * @param coords The grid point coordinates.
   * @return A `vec4` containing the 3D position and the field value of the grid point.
   */
  vec4 gridPointPositionAndFieldValue(const vec3i& coords) const;

  /**
   * @brief Generates a signed distance field from a triangle mesh.
   *
   * This function computes the signed distance field based on an input triangle mesh,
   * with optional expansion and voxel size parameters.
   *
   * @param in_mesh The input triangle mesh to generate the SDF from.
   * @param expansion The expansion vector to apply around the mesh.
   * @param voxel_size The size of the voxels (default is `kDefaultVoxelSize`).
   * @return True if the SDF was successfully generated, otherwise false.
   */
  bool generate(const TriangleMesh& in_mesh,
                const vec3& expansion,
                real_t voxel_size = kDefaultVoxelSize);

  /**
   * @brief Retrieves the field value at a specific point in 3D space.
   *
   * This function computes the signed distance field value at an arbitrary point in space.
   *
   * @param p The 3D point at which to evaluate the field value.
   * @return The signed distance field value at the given point.
   */
  real_t fieldValue(const vec3& p) const;

  /**
   * @brief Retrieves the field value at a specific grid point.
   *
   * This function retrieves the signed distance field value at a specific grid point using grid coordinates.
   *
   * @param coords The grid point coordinates.
   * @return The signed distance field value at the specified grid point.
   */
  real_t fieldValue(const vec3i& coords) const;

  /**
   * @brief Saves the signed distance field to a VTK Image data (VTI) file.
   *
   * This function writes the SDF data to a VTK Image data file for later use.
   *
   * @param filepath The file path where the SDF will be saved.
   * @return True if the SDF was successfully saved, otherwise false.
   */
  bool saveAsVTI(const std::string& filepath) const;

  /**
   * @brief Loads the signed distance field to a VTK Image data (VTI) file.
   *
   * This function loads the SDF data to a VTK Image data file for later use.
   *
   * @param filepath The file path where the SDF will be saved.
   * @return True if the SDF was successfully loaded, otherwise false.
   */
  bool loadAsVTI(const std::string& filepath);

  /**
   * @brief Assignment operator.
   *
   * Assigns the values of one `SignedDistanceField` object to another.
   *
   * @param rhs The `SignedDistanceField` object to copy from.
   * @return A reference to the assigned `SignedDistanceField` object.
   */
  SignedDistanceField& operator=(const SignedDistanceField& rhs);

private:
  /**
   * @brief Checks if the given grid coordinates are valid.
   *
   * This function verifies if the provided grid point coordinates are within bounds.
   *
   * @param coords The grid point coordinates to check.
   * @param gridpoints_count The total number of grid points along each axis.
   * @return True if the coordinates are valid, otherwise false.
   */
  bool isValidGridPointCoords(const vec3i& coords, const vec3i& gridpoints_count) const;

  /**
   * @brief Asserts that the given grid coordinates are valid.
   *
   * Throws an error if the grid point coordinates are out of bounds.
   *
   * @param coords The grid point coordinates to validate.
   */
  void assertGridPointCoords(const vec3i& coords) const;

  bool parseAsciiValues(const std::string& in_data_string,
                        std::vector<real_t>& out_data_values);

private:
  real_t voxel_size_ = kDefaultVoxelSize; /**< The size of each voxel in the SDF. */
  AABB bounds_; /**< The axis-aligned bounding box (AABB) of the SDF. */
  std::vector<real_t> field_values_; /**< The field values at each grid point. */
  mutable std::mutex field_values_mutex_; /**< Mutex for thread-safe access to magnitudes and signs. */
};

}