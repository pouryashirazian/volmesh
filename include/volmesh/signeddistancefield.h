#pragma once

#include "volmesh/trianglemesh.h"

namespace volmesh {

  /*!
  * \brief SignedDistanceField computes and stores the signed distance field (SDF) due to
  * a surface triangle mesh. It provides an API to interpolate the SDF value due to a point in Cartesian space.
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

    uint64_t gridPointId(const vec3i& coords) const;

    vec4 gridPointVertexAndFieldValue(const vec3i& coords) const;

    bool generate(const TriangleMesh& in_mesh,
                  const vec3& expansion,
                  real_t voxel_size = kDefaultVoxelSize);

    real_t computeFieldValue(const vec3& p) const;

    SignedDistanceField& operator=(const SignedDistanceField& rhs);
  private:
    real_t voxel_size_ = kDefaultVoxelSize;
    AABB bounds_;
    std::vector<real_t> values_;
    std::vector<real_t> signs_;
  };

}