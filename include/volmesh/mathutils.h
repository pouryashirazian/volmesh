#pragma once

#include "volmesh/basetypes.h"

namespace volmesh {

  bool FuzzyIsNull(double d);

  bool FuzzyIsNull(float f);

  bool FuzzyCompare(double p1, double p2);

  bool FuzzyCompare(float p1, float p2);

  real_t RadToDeg(const real_t in_angle_radians);

  real_t DegToRad(const real_t in_angle_degrees);

  /*!
  * \brief computes the internal angle between three vertices aoc, where vertex o is the
  * origin.
  * \return returns the computed angle in Radians
  */
  real_t ComputeInternalAngle(const vec3& a, const vec3& o, const vec3& c);

  bool NormalsDirectionMatch(const vec3& n1, const vec3& n2, real_t tolerance_angle_degrees);

  /*!
  * \brief computes the minimum distance between a point and a line segment.
  * \param p input point p
  * \param a line start point
  * \param b line end point
  * \param q output closest point
  * \return returns the minimum distance
  */
  real_t PointLineSegmentDistance(const vec3& p,
                                  const vec3& a,
                                  const vec3& b,
                                  vec3& q);

  /*!
  * \brief computes the minimum distance between a point and a triangle.
  * \param p input point p
  * \param a first triangle point
  * \param b second triangle point
  * \param c third triangle point
  * \param q output closest point
  * \return returns the minimum distance
  */
  real_t PointTriangleDistance(const vec3& p,
                               const vec3& a,
                               const vec3& b,
                               const vec3& c,
                               vec3& q);
}