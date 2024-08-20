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

  real_t ComputeMinPointTriangleDistance(const vec3& p,
                                         const vec3& a,
                                         const vec3& b,
                                         const vec3& c,
                                         vec3& q) {
    const vec3 n = (b - a).cross(c - a);
    const real_t n_mag_squared = n.norm() * n.norm();
    const vec3 tn = n.normalized();

    //vector ap = p - a, takes us from the triangle point a to the query point p
    //the dot product between ap and the triangle normal n is the distance
    const real_t dist = (p - a).dot(tn);

    // compute q in the plane of the triangle
    const vec3 q = p - dist * tn;

    // check if q is inside the triangle boundary
    const vec3 u =
  }

}