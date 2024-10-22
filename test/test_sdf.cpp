//-----------------------------------------------------------------------------
// Copyright (c) Pourya Shirazian
// All rights reserved.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.
//-----------------------------------------------------------------------------

#include "volmesh/trianglemesh.h"
#include "volmesh/basetypes.h"
#include "volmesh/signeddistancefield.h"

#include <gtest/gtest.h>
#include <vector>
#include <filesystem>

using namespace volmesh;

TEST(SignedDistanceField, SaveLoadVTI) {
  std::vector<vec3> vertices = {
    vec3(-4.0, 0.0, 0.0), vec3(4.0, 0.0, 0.0), vec3(0.0, 4.0, 0.0)
  };

  TriangleMesh tmesh;
  EXPECT_TRUE(tmesh.insertAllVertices(vertices));

  HalfFaceIndex fid = tmesh.insertTriangle(vec3i(0, 1, 2));
  EXPECT_EQ(fid, 0);

  tmesh.computeHalfEdgePseudoNormals();
  tmesh.computeVertexPseudoNormals();

  EXPECT_EQ(tmesh.countFaces(), 1);

  SignedDistanceField sdf1;
  EXPECT_TRUE(sdf1.generate(tmesh, vec3(0.1, 0.1, 0.1), 0.1));

  std::filesystem::path vti_path = std::filesystem::temp_directory_path() / "triangle.vti";
  EXPECT_TRUE(sdf1.saveAsVTI(vti_path.string()));

  SignedDistanceField sdf2;
  EXPECT_TRUE(sdf2.loadAsVTI(vti_path.string()));

  // EXPECT_TRUE(FuzzyCompare(sdf1.voxelSize(), sdf2.voxelSize()));

  EXPECT_EQ(sdf1.voxelsCount(), sdf2.voxelsCount());

  EXPECT_EQ(sdf1.gridPointsCount(), sdf2.gridPointsCount());

  EXPECT_EQ(sdf1.totalGridPointsCount(), sdf2.totalGridPointsCount());

  EXPECT_EQ(sdf1.getTotalMemoryUsageInBytes(), sdf2.getTotalMemoryUsageInBytes());
}