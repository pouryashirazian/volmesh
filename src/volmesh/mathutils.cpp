#include "volmesh/mathutils.h"

#include <math.h>

namespace volmesh {

  bool FuzzyIsNull(double d) {
    return std::fabs(d) < 0.000000000001;
  }

  bool FuzzyIsNull(float f) {
    return std::fabs(f) < 0.00001f;
  }

  bool FuzzyCompare(double p1, double p2) {
    return (std::fabs(p1 - p2) * 1000000000000. <= std::min<double>(std::fabs(p1), std::fabs(p2)));
  }

  bool FuzzyCompare(float p1, float p2) {
    return (std::fabs(p1 - p2) * 100000.f <= std::min<float>(std::fabs(p1), std::fabs(p2)));
  }

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

  real_t PointLineSegmentDistance(const vec3& p, const vec3& a, const vec3& b, vec3& q) {
    const real_t len = (b - a).norm();

    // initialize the output intersection point x
    q = vec3(0.0, 0.0, 0.0);

    if (FuzzyIsNull(len)) {
      return std::numeric_limits<real_t>::max();
    }

    const real_t len_inv = 1.0 / len;

    // compute a unit vector along ab
    const vec3 u = (b - a) * len_inv;

    // distance of the intersection point q from a
    const real_t dist_q_a = (p - a).dot(u);

    if (dist_q_a < 0) {
      q = a;
    } else if (dist_q_a > len) {
      q = b;
    } else {
      // intersection point between a and b
      q = a + dist_q_a * u;
    }

    // point line distance is the distance between q and p
    return (p - q).norm();
  }

  real_t PointTriangleDistance(const vec3& p,
                               const vec3& a,
                               const vec3& b,
                               const vec3& c,
                               vec3& q) {
    const vec3 n = (b - a).cross(c - a);
    const real_t n_mag_squared = n.squaredNorm();

    real_t shortest_distance = std::numeric_limits<real_t>::max();
    q = vec3(0.0, 0.0, 0.0);

    if (n_mag_squared == 0.0) {
      return shortest_distance;
    }

    const vec3 tn = n.normalized();

    //vector ap = p - a, takes us from the triangle point a to the query point p
    //the dot product between ap and the triangle normal n is the distance
    const real_t pprime_distance = (p - a).dot(tn);

    // compute  in the plane of the triangle
    const vec3 pprime = p - pprime_distance * tn;

    // check if pprime is inside the triangle boundary
    // compute the three normals inside our triangle
    const vec3 na = (pprime - c).cross(b - c);
    const vec3 nb = (pprime - a).cross(c - a);
    const vec3 nc = (b - a).cross(pprime - a);

    // u = n.nc / ||n|| ^ 2
    // v = n.nb / ||n|| ^ 2
    // w = n.na / ||n|| ^ 2
    const real_t u = n.dot(nc) / n_mag_squared;
    const real_t v = n.dot(nb) / n_mag_squared;
    const real_t w = n.dot(na) / n_mag_squared;

    // pprime is inside the triangle if all barycentric coords are between 0 and 1 exclusive
    if ((u > 0 && u < 1)&&(v > 0 && v < 1)&&(w > 0 && w < 1)) {
      q = pprime;
      shortest_distance = pprime_distance;
    } else {
      int count_zero_coords = FuzzyIsNull(u) + FuzzyIsNull(v) + FuzzyIsNull(w);

      // compute distance to all edges
      if (count_zero_coords <= 1) {
        // compute distance from p to ab line
        vec3 q_ab, q_bc, q_ca;
        real_t d_ab, d_bc, d_ca;

        d_ab = PointLineSegmentDistance(p, a, b, q_ab);
        d_bc = PointLineSegmentDistance(p, b, c, q_bc);
        d_ca = PointLineSegmentDistance(p, c, a, q_ca);

        if (d_ab < d_bc) {
          shortest_distance = d_ab;
          q = q_ab;
        } else {
          shortest_distance = d_bc;
          q = q_bc;
        }

        if (d_ca < shortest_distance) {
          shortest_distance = d_ca;
          q = q_ca;
        }
      } //compute distance to all vertices
      else if (count_zero_coords == 2) {
        const real_t ap = (p - a).norm();
        const real_t bp = (p - b).norm();
        const real_t cp = (p - c).norm();

        if (ap < bp) {
          shortest_distance = ap;
          q = a;
        } else {
          shortest_distance = bp;
          q = b;
        }

        if (cp < shortest_distance) {
          shortest_distance = cp;
          q = c;
        }
      }
    }

    return shortest_distance;
  }

}