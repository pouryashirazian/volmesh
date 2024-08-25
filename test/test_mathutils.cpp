#include "volmesh/mathutils.h"

#include <gtest/gtest.h>

using namespace volmesh;

TEST(MathUtils, PointLineSegmentDistanceBasic) {
  const vec3 a(2.0, 1.0, 0.0);
  const vec3 b(4.0, 1.0, 0.0);
  const vec3 p(3.0, 0.0, 0.0);
  vec3 q;

  const real_t dist = PointLineSegmentDistance(p, a, b, q);

  EXPECT_TRUE(FuzzyCompare(dist, 1.0));

  EXPECT_TRUE(FuzzyCompare(q.x(), 3.0));
  EXPECT_TRUE(FuzzyCompare(q.y(), 1.0));
  EXPECT_TRUE(FuzzyCompare(q.z(), 0.0));
}

TEST(MathUtils, PointLineSegmentDistanceStart) {
  const vec3 a(1.0, 1.0, 0.0);
  const vec3 b(3.0, 3.0, 0.0);
  const vec3 p(0.0, 0.0, 0.0);
  vec3 q;

  const real_t dist = PointLineSegmentDistance(p, a, b, q);

  EXPECT_TRUE(FuzzyCompare(dist, (p - a).norm()));

  EXPECT_TRUE(FuzzyCompare(q.x(), a.x()));
  EXPECT_TRUE(FuzzyCompare(q.y(), a.y()));
  EXPECT_TRUE(FuzzyCompare(q.z(), a.z()));
}

TEST(MathUtils, PointTriangleDistancePointA) {
  const vec3 a(2.0, 2.0, 0.0);
  const vec3 b(6.0, 2.0, 0.0);
  const vec3 c(4.0, 4.0, 0.0);

  const vec3 p(1.0, 1.0, 0.0);
  vec3 q;

  const real_t dist = PointTriangleDistance(p, a, b, c, q);

  EXPECT_TRUE(FuzzyCompare(dist, (p - a).norm()));

  EXPECT_TRUE(FuzzyCompare(q.x(), a.x()));
  EXPECT_TRUE(FuzzyCompare(q.y(), a.y()));
  EXPECT_TRUE(FuzzyCompare(q.z(), a.z()));
}

TEST(MathUtils, PointTriangleDistancePointB) {
  const vec3 a(2.0, 2.0, 0.0);
  const vec3 b(6.0, 2.0, 0.0);
  const vec3 c(4.0, 4.0, 0.0);

  const vec3 p(7.0, 1.0, 0.0);
  vec3 q;

  const real_t dist = PointTriangleDistance(p, a, b, c, q);

  EXPECT_TRUE(FuzzyCompare(dist, (p - b).norm()));

  EXPECT_TRUE(FuzzyCompare(q.x(), b.x()));
  EXPECT_TRUE(FuzzyCompare(q.y(), b.y()));
  EXPECT_TRUE(FuzzyCompare(q.z(), b.z()));
}

TEST(MathUtils, PointTriangleDistancePointC) {
  const vec3 a(2.0, 2.0, 0.0);
  const vec3 b(6.0, 2.0, 0.0);
  const vec3 c(4.0, 4.0, 0.0);

  const vec3 p(4.0, 5.0, 0.0);
  vec3 q;

  const real_t dist = PointTriangleDistance(p, a, b, c, q);

  EXPECT_TRUE(FuzzyCompare(dist, (p - c).norm()));

  EXPECT_TRUE(FuzzyCompare(q.x(), c.x()));
  EXPECT_TRUE(FuzzyCompare(q.y(), c.y()));
  EXPECT_TRUE(FuzzyCompare(q.z(), c.z()));
}