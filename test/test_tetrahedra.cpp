#include "volmesh/tetrahedra.h"

#include <gtest/gtest.h>

using namespace volmesh;

static const real_t kEpsilon = 1e-5;

static const Tetrahedra::VertexArray4 kTetVertices = [] {
    Tetrahedra::VertexArray4 tmp;
    //init format:
    //X: v0.x, v1.x, v2.x, v3.x
    //Y: v0.y, v1.y, v2.y, v3.y
    //Z: v0.z, v1.z, v2.z, v3.z
    tmp << -1, 0, 1, 0,
            0, 0, 0, 2,
            0, -2, 0, -1;
    return tmp;
}();

TEST(Tetrahedra, Ctor) {
  Tetrahedra t1(kTetVertices);

  EXPECT_EQ(t1.vertex(0), kTetVertices.col(0));
  EXPECT_EQ(t1.vertex(1), kTetVertices.col(1));
  EXPECT_EQ(t1.vertex(2), kTetVertices.col(2));
  EXPECT_EQ(t1.vertex(3), kTetVertices.col(3));
}

TEST(Tetrahedra, CopyCtor) {
  Tetrahedra t1(kTetVertices);
  Tetrahedra t2(t1);

  EXPECT_EQ(t2.vertex(0), kTetVertices.col(0));
  EXPECT_EQ(t2.vertex(1), kTetVertices.col(1));
  EXPECT_EQ(t2.vertex(2), kTetVertices.col(2));
  EXPECT_EQ(t2.vertex(3), kTetVertices.col(3));
}

TEST(Tetrahedra, InRadiusCircumRadius) {
  Tetrahedra t1(kTetVertices);
  EXPECT_GT(t1.inradius(), 0.0);
  EXPECT_GT(t1.circumradius(), 0.0);
  EXPECT_GT(t1.aspectRatio(), 0.0);
  EXPECT_NEAR(t1.aspectRatio(), t1.circumradius() / (3.0 * t1.inradius()), kEpsilon);
}

TEST(Tetrahedra, Centroid) {
  Tetrahedra t1(kTetVertices);
  vec3 centroid = t1.centroid();
  vec3 gt_centroid = kTetVertices.rowwise().sum() / static_cast<real_t>(Tetrahedra::kNumVerticesPerCell);
  EXPECT_NEAR(centroid.x(), gt_centroid.x(), kEpsilon);
  EXPECT_NEAR(centroid.y(), gt_centroid.y(), kEpsilon);
  EXPECT_NEAR(centroid.z(), gt_centroid.z(), kEpsilon);
}

TEST(Tetrahedra, DeterminantVolume) {
  Tetrahedra t1(kTetVertices);
  const real_t volume = t1.volume();
  const real_t volume_gt = static_cast<real_t>(1.0 / 6.0) * fabs(t1.determinant());
  EXPECT_NEAR(volume, volume_gt, kEpsilon);
}

