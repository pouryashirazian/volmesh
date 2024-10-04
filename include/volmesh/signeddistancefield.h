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

  /*!
  * \brief SignedDistanceField computes and stores the signed distance field (SDF) due to
  * a surface triangle mesh. It provides an API to interpolate the SDF value due to a point in
  * the Cartesian space.
  */
  class SignedDistanceField {
  public:
    SignedDistanceField();
    SignedDistanceField(const SignedDistanceField& rhs);
    ~SignedDistanceField();

    static constexpr const real_t kDefaultVoxelSize = 0.5;

    void copyFrom(const SignedDistanceField& rhs);

    AABB bounds() const;

    real_t voxelSize() const;

    vec3i voxelsCount() const;

    vec3i gridPointsCount() const;

    uint64_t totalGridPointsCount() const;

    uint64_t getTotalMemoryUsageInBytes() const;

    uint64_t gridPointId(const vec3i& coords) const;

    vec3 gridPointPosition(const vec3i& coords) const;

    vec4 gridPointPositionAndFieldValue(const vec3i& coords) const;

    bool generate(const TriangleMesh& in_mesh,
                  const vec3& expansion,
                  real_t voxel_size = kDefaultVoxelSize);

    real_t fieldValue(const vec3& p) const;

    real_t fieldValue(const vec3i& coords) const;

    bool save(const std::string& filepath) const;

    bool load(const std::string& filepath);

    SignedDistanceField& operator=(const SignedDistanceField& rhs);

  private:
    bool isValidGridPointCoords(const vec3i& coords, const vec3i& gridpoints_count) const;
    void assertGridPointCoords(const vec3i& coords) const;
    vec4 gridPointPositionAndMagnitude(const vec3i& coords) const;

  private:
    real_t voxel_size_ = kDefaultVoxelSize;
    AABB bounds_;
    std::vector<real_t> magnitudes_;
    std::vector<real_t> signs_;
    mutable std::mutex magnitude_sign_mutex_;
  };

}