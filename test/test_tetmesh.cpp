#include "volmesh/tetmesh.h"
#include "volmesh/sampletetmeshes.h"
#include "volmesh/tetrahedra.h"

#include <gtest/gtest.h>

using namespace volmesh;

TEST(TetMesh, OneCell) {
  TetMesh mesh;
  createOneTetrahedra(mesh);

  EXPECT_EQ(mesh.countCells(), 1);
  EXPECT_EQ(mesh.countHalfFaces(), Tetrahedra::kNumFaces * 2);
  EXPECT_EQ(mesh.countEdges(), Tetrahedra::kNumEdges);
  EXPECT_EQ(mesh.countHalfEdges(), Tetrahedra::kNumEdges * 2);
  EXPECT_EQ(mesh.countVertices(), Tetrahedra::kNumVerticesPerCell);
}