#include "volmesh/mathutils.h"

#include <math.h>

namespace volmesh {

  real_t RadToDeg(const real_t in_angle_radians) {
    return (in_angle_radians * static_cast<real_t>(180.0)) / static_cast<real_t>(M_PI);
  }

  real_t DegToRad(const real_t in_angle_degrees) {
    return (in_angle_degrees * static_cast<real_t>(M_PI)) / static_cast<real_t>(180.0);
  }

  real_t ComputeInternalAngle(const vec3& a, const vec3& o, const vec3& c) {
    const vec3 oa = (a - o).normalized();
    const vec3 oc = (c - o).normalized();
    return acos(oa.dot(oc));
  }

  bool NormalsDirectionMatch(const vec3& n1, const vec3& n2, real_t tolerance_angle_degrees) {
    const real_t angle = RadToDeg(acos(n1.dot(n2)));
    return (angle < tolerance_angle_degrees);
  }

}