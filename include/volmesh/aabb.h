#pragma once

#include "volmesh/basetypes.h"

namespace volmesh {

  class AABB {
  public:
    AABB();
    AABB(const vec3& lower, const vec3& upper);
    AABB(const AABB& rhs);
    ~AABB();

    vec3 lower() const;
    vec3 upper() const;
    vec3 extent() const;

    void expand(const real_t dx, const real_t dy, const real_t dz);
    void expand(const vec3& expansion);
    bool intersects(const AABB& rhs) const;

    AABB& operator=(const AABB& rhs);
  private:
    vec3 lower_ = {0.0, 0.0, 0.0};
    vec3 upper_ = {0.0, 0.0, 0.0};
  };

}