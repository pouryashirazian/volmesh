#pragma once

#include "volmesh/basetypes.h"
#include "volmesh/aabb.h"

#include <array>

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

  enum ClosestLineSegmentFeature : int {
    kClosestLineSegmentFeatureNone,  // 0
    kClosestLineSegmentFeatureStart, // 1
    kClosestLineSegmentFeatureEnd,   // 2
    kClosestLineSegmentFeatureMid,   // 3
  };

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
                                  vec3& q,
                                  ClosestLineSegmentFeature& closest_feature);

  enum TriangleVertex : uint8_t {
    kTriangleVertexA = 1,
    kTriangleVertexB = 2,
    kTriangleVertexC = 4,
  };

  #define TriangleEdge(A,B) (A | B)& 0xFF

  enum ClosestTriangleFeature : uint8_t {
    kClosestTriangleFeatureNone = 0,                                    // 0
    kClosestTriangleFeatureVertexA = TriangleVertex::kTriangleVertexA,  // 1
    kClosestTriangleFeatureVertexB = TriangleVertex::kTriangleVertexB,  // 2
    kClosestTriangleFeatureEdgeAB = TriangleEdge(TriangleVertex::kTriangleVertexA, TriangleVertex::kTriangleVertexB),  // 3
    kClosestTriangleFeatureVertexC = TriangleVertex::kTriangleVertexC,  // 4
    kClosestTriangleFeatureEdgeCA = TriangleEdge(TriangleVertex::kTriangleVertexC, TriangleVertex::kTriangleVertexA),  // 5
    kClosestTriangleFeatureEdgeBC = TriangleEdge(TriangleVertex::kTriangleVertexB, TriangleVertex::kTriangleVertexC),  // 6
    kClosestTriangleFeatureInside = 7,  // 7
  };

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
                               vec3& q,
                               ClosestTriangleFeature& closest_feature);

  AABB ComputeTriangleAABB(const std::array<vec3, 3>& vertices);

}