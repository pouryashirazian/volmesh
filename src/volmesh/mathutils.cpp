#include "volmesh/mathutils.h"

#include <math.h>

namespace volmesh {

  real_t RadToDeg(const real_t in_angle_radians) {
    return (in_angle_radians * static_cast<real_t>(180.0)) / static_cast<real_t>(M_PI);
  }

  real_t DegToRad(const real_t in_angle_degrees) {
    return (in_angle_degrees * static_cast<real_t>(M_PI)) / static_cast<real_t>(180.0);
  }

}