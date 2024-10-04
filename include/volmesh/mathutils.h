//-----------------------------------------------------------------------------
// Copyright (c) Pourya Shirazian
// All rights reserved.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.
//-----------------------------------------------------------------------------

#pragma once

#include "volmesh/basetypes.h"
#include "volmesh/aabb.h"

#include <array>

namespace volmesh {

/**
 * @brief Checks if a double precision floating-point value is approximately zero.
 *
 * This function compares the value of a double-precision floating-point number to zero,
 * allowing for a small tolerance to account for floating-point inaccuracies.
 *
 * @param d The double value to check.
 * @return True if the value is close to zero, otherwise false.
 */
bool FuzzyIsNull(double d);

/**
 * @brief Checks if a single precision floating-point value is approximately zero.
 *
 * This function compares the value of a single-precision floating-point number to zero,
 * allowing for a small tolerance to account for floating-point inaccuracies.
 *
 * @param f The float value to check.
 * @return True if the value is close to zero, otherwise false.
 */
bool FuzzyIsNull(float f);

/**
 * @brief Compares two double precision floating-point values for approximate equality.
 *
 * This function compares two double-precision floating-point numbers to determine
 * if they are approximately equal, considering potential floating-point inaccuracies.
 *
 * @param p1 The first double value to compare.
 * @param p2 The second double value to compare.
 * @return True if the values are approximately equal, otherwise false.
 */
bool FuzzyCompare(double p1, double p2);

/**
 * @brief Compares two single precision floating-point values for approximate equality.
 *
 * This function compares two single-precision floating-point numbers to determine
 * if they are approximately equal, considering potential floating-point inaccuracies.
 *
 * @param p1 The first float value to compare.
 * @param p2 The second float value to compare.
 * @return True if the values are approximately equal, otherwise false.
 */
bool FuzzyCompare(float p1, float p2);

/**
 * @brief Converts an angle from radians to degrees.
 *
 * This function converts the input angle from radians to degrees.
 *
 * @param in_angle_radians The angle in radians.
 * @return The angle converted to degrees.
 */
real_t RadToDeg(const real_t in_angle_radians);

/**
 * @brief Converts an angle from degrees to radians.
 *
 * This function converts the input angle from degrees to radians.
 *
 * @param in_angle_degrees The angle in degrees.
 * @return The angle converted to radians.
 */
real_t DegToRad(const real_t in_angle_degrees);

/**
 * @brief Computes the internal angle between three points in 3D space.
 *
 * Given three points `a`, `o`, and `c`, this function computes the internal angle at point `o`.
 *
 * @param a First point forming the angle.
 * @param o The vertex of the angle.
 * @param c Second point forming the angle.
 * @return The internal angle at `o` in radians.
 */
real_t ComputeInternalAngle(const vec3& a, const vec3& o, const vec3& c);

/**
 * @brief Checks if the directions of two normal vectors match within a given tolerance.
 *
 * This function compares two normal vectors `n1` and `n2` to determine if they point in
 * roughly the same direction, within a specified angular tolerance.
 *
 * @param n1 The first normal vector.
 * @param n2 The second normal vector.
 * @param tolerance_angle_degrees The maximum allowed angle difference in degrees.
 * @return True if the normal vectors match within the tolerance, otherwise false.
 */
bool NormalsDirectionMatch(const vec3& n1, const vec3& n2, real_t tolerance_angle_degrees);

/**
 * @enum ClosestLineSegmentFeature
 * @brief Represents the feature of a line segment closest to a point.
 */
enum ClosestLineSegmentFeature : int {
    kClosestLineSegmentFeatureNone,  /**< No closest feature. */
    kClosestLineSegmentFeatureStart, /**< The start point of the line segment is closest. */
    kClosestLineSegmentFeatureEnd,   /**< The end point of the line segment is closest. */
    kClosestLineSegmentFeatureMid,   /**< The midpoint of the line segment is closest. */
};

/**
 * @brief Computes the minimum distance between a point and a line segment in 3D space.
 *
 * Given a point `p` and a line segment defined by points `a` and `b`, this function computes the
 * closest point on the line segment to `p`, stores it in `q`, and returns the distance.
 *
 * @param p The point from which the distance is computed.
 * @param a The start point of the line segment.
 * @param b The end point of the line segment.
 * @param q The output point on the line segment closest to `p`.
 * @param closest_feature The feature of the line segment that is closest to `p`.
 * @return The minimum distance between point `p` and the line segment.
 */
real_t PointLineSegmentDistance(const vec3& p,
                                const vec3& a,
                                const vec3& b,
                                vec3& q,
                                ClosestLineSegmentFeature& closest_feature);

/**
 * @enum TriangleVertex
 * @brief Represents the vertices of a triangle.
 */
enum TriangleVertex : uint8_t {
    kTriangleVertexA = 1, /**< Vertex A of the triangle. */
    kTriangleVertexB = 2, /**< Vertex B of the triangle. */
    kTriangleVertexC = 4, /**< Vertex C of the triangle. */
};

/**
 * @brief Macro to define a triangle edge between two vertices.
 */
#define TriangleEdge(A,B) (A | B) & 0xFF

/**
 * @enum ClosestTriangleFeature
 * @brief Represents the feature of a triangle closest to a point.
 */
enum ClosestTriangleFeature : uint8_t {
    kClosestTriangleFeatureNone = 0,                                     /**< No closest feature. */
    kClosestTriangleFeatureVertexA = TriangleVertex::kTriangleVertexA,   /**< Vertex A is closest. */
    kClosestTriangleFeatureVertexB = TriangleVertex::kTriangleVertexB,   /**< Vertex B is closest. */
    kClosestTriangleFeatureEdgeAB = TriangleEdge(TriangleVertex::kTriangleVertexA, TriangleVertex::kTriangleVertexB), /**< Edge AB is closest. */
    kClosestTriangleFeatureVertexC = TriangleVertex::kTriangleVertexC,   /**< Vertex C is closest. */
    kClosestTriangleFeatureEdgeCA = TriangleEdge(TriangleVertex::kTriangleVertexC, TriangleVertex::kTriangleVertexA), /**< Edge CA is closest. */
    kClosestTriangleFeatureEdgeBC = TriangleEdge(TriangleVertex::kTriangleVertexB, TriangleVertex::kTriangleVertexC), /**< Edge BC is closest. */
    kClosestTriangleFeatureInside = 7, /**< The point is inside the triangle. */
};

/**
 * @brief Computes the minimum distance between a point and a triangle in 3D space.
 *
 * Given a point `p` and a triangle defined by points `a`, `b`, and `c`, this function computes the
 * closest point on the triangle to `p`, stores it in `q`, and returns the distance.
 *
 * @param p The point from which the distance is computed.
 * @param a First vertex of the triangle.
 * @param b Second vertex of the triangle.
 * @param c Third vertex of the triangle.
 * @param q The output point on the triangle closest to `p`.
 * @param closest_feature The feature of the triangle that is closest to `p`.
 * @return The minimum distance between point `p` and the triangle.
 */
real_t PointTriangleDistance(const vec3& p,
                             const vec3& a,
                             const vec3& b,
                             const vec3& c,
                             vec3& q,
                             ClosestTriangleFeature& closest_feature);

/**
 * @brief Computes the axis-aligned bounding box (AABB) of a triangle.
 *
 * Given the three vertices of a triangle, this function computes the AABB that contains the triangle.
 *
 * @param vertices An array of three vertices representing the triangle.
 * @return The computed AABB for the triangle.
 */
AABB ComputeTriangleAABB(const std::array<vec3, 3>& vertices);

}