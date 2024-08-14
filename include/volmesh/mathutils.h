#pragma once

#include "volmesh/basetypes.h"

namespace volmesh {

  real_t RadToDeg(const real_t in_angle_radians);

  real_t DegToRad(const real_t in_angle_degrees);

  /*!
  * \brief Computes the internal angle between three vertices aoc, where vertex o is the
  * origin.
  * \return returns the computed angle in Radians
  */
  real_t ComputeInternalAngle(const vec3& a, const vec3& o, const vec3& c);

  bool NormalsDirectionMatch(const vec3& n1, const vec3& n2, real_t tolerance_angle_degrees);
}