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

    static constexpr const real_t kDefaultSpacing = 0.5;

    void copyFrom(const SignedDistanceField& rhs);
    AABB bounds() const;
    real_t spacing() const;

    bool generate(const TriangleMesh& in_mesh,
                  const vec3& expansion,
                  real_t spacing = kDefaultSpacing);

    real_t compute(const vec3& p) const;

    SignedDistanceField& operator=(const SignedDistanceField& rhs);
  private:
    AABB bounds_;
    real_t spacing_ = kDefaultSpacing;
  };

}