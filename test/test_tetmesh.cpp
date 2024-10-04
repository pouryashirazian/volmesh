//-----------------------------------------------------------------------------
// Copyright (c) Pourya Shirazian
// All rights reserved.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.
//-----------------------------------------------------------------------------

#include "volmesh/tetmesh.h"
#include "volmesh/sampletetmeshes.h"
#include "volmesh/tetrahedra.h"

#include <gtest/gtest.h>

using namespace volmesh;

TEST(TetMesh, OneTetrahedra) {
  TetMesh mesh;
  createOneTetrahedra(mesh);

  EXPECT_EQ(mesh.countCells(), 1);
  EXPECT_EQ(mesh.countHalfFaces(), Tetrahedra::kNumFaces * 2);
  EXPECT_EQ(mesh.countEdges(), Tetrahedra::kNumEdges);
  EXPECT_EQ(mesh.countHalfEdges(), Tetrahedra::kNumEdges * 2);
  EXPECT_EQ(mesh.countVertices(), Tetrahedra::kNumVerticesPerCell);
}

TEST(TetMesh, TwoTetrahedra) {
  TetMesh mesh;
  createTwoTetrahedra(mesh);

  EXPECT_EQ(mesh.countCells(), 2);
  EXPECT_EQ(mesh.countHalfEdges(), mesh.countEdges() * 2);
  EXPECT_EQ(mesh.countVertices(), 5);
}

TEST(TetMesh, SphericalShell) {
  TetMesh mesh;

  const int hs = 8;
  const int vs = 8;
  EXPECT_TRUE(createSphericalShell(mesh, hs, vs, 2.0, 0.2));
  EXPECT_EQ(mesh.countHalfEdges(), mesh.countEdges() * 2);
  EXPECT_EQ(mesh.countVertices(), hs * (vs + 1) * 2);
}

TEST(TetMesh, ExtractTriangleMesh) {
  TetMesh tet_mesh;
  createOneTetrahedra(tet_mesh);

  TriangleMesh tri_mesh;
  EXPECT_TRUE(tet_mesh.extractBoundaryTriangleMesh(tri_mesh));
  EXPECT_EQ(tri_mesh.countHalfFaces(), 4);
}