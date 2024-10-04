//-----------------------------------------------------------------------------
// Copyright (c) Pourya Shirazian
// All rights reserved.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.
//-----------------------------------------------------------------------------

#include "volmesh/voxel.h"

#include <gtest/gtest.h>

using namespace volmesh;

// LBN = 0, LBF = 1, LTN = 2, LTF = 3,
// RBN = 4, RBF = 5, RTN = 6, RTF = 7

static const Voxel::VoxelVertexArray kVoxelVertices = [] {
    Voxel::VoxelVertexArray tmp;
    //init format:
    //X: v0.x, v1.x, ..., v7.x
    //Y: v0.y, v1.y, ..., v7.y
    //Z: v0.z, v1.z, ..., v7.z
    //width = 2
    //height = 4
    //depth = 8
    tmp << 0, 0, 0, 0, 2, 2, 2, 2,
           0, 0, 4, 4, 0, 0, 4, 4,
           0, 8, 0, 8, 0, 8, 0, 8;
    return tmp;
}();

TEST(Voxel, DefaultCtor) {
  Voxel v1;

  Voxel v1_gt = Voxel::CreateVoxel(vec3(0, 0, 0), vec3(2, 2, 2));
  for(int i=0; i < Voxel::kNumVerticesPerCell; i++) {
    EXPECT_EQ(v1.vertex(i), v1_gt.vertex(i));
  }
}

TEST(Voxel, CopyCtor) {
  Voxel v1(kVoxelVertices);
  Voxel v2(v1);

  for(int i=0; i < Voxel::kNumVerticesPerCell; i++) {
    EXPECT_EQ(v1.vertex(i), kVoxelVertices.col(i));
    EXPECT_EQ(v1.vertex(i), v2.vertex(i));
  }
}

TEST(Voxel, AxisLength) {
  Voxel v1(kVoxelVertices);

  EXPECT_EQ(v1.axisLength(Voxel::VoxelAxis::X), 2.0);
  EXPECT_EQ(v1.axisLength(Voxel::VoxelAxis::Y), 4.0);
  EXPECT_EQ(v1.axisLength(Voxel::VoxelAxis::Z), 8.0);
}

TEST(Voxel, SurfaceArea) {
  Voxel v1(kVoxelVertices);

  //2 x 4 + 4 x 8 + 2 x 8 = 56
  EXPECT_EQ(v1.surfaceArea(), 56);
}

TEST(Voxel, Centroid) {
  Voxel v1(kVoxelVertices);
  EXPECT_EQ(v1.centroid(), vec3(1.0, 2.0, 4.0));

  vec3 center(-5.0, -10.0, -7.0);
  vec3 sides(2, 3, 5);
  Voxel v2(Voxel::CreateVoxel(center, sides));
  EXPECT_EQ(v2.centroid(), center);
  EXPECT_EQ(v2.axisLength(Voxel::VoxelAxis::X), sides.x());
  EXPECT_EQ(v2.axisLength(Voxel::VoxelAxis::Y), sides.y());
  EXPECT_EQ(v2.axisLength(Voxel::VoxelAxis::Z), sides.z());
}

TEST(Voxel, Volume) {
  Voxel v1(kVoxelVertices);

  //2 x 4 x 8 = 64
  EXPECT_EQ(v1.volume(), 64);
}