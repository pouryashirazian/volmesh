#include "volmesh/trianglemesh.h"

#include <gtest/gtest.h>

using namespace volmesh;

TEST(TriangleMesh, Ctor) {
  TriangleMesh tmesh;
  std::vector<vec3> vertices = {
    vec3(-4.0, 0.0, 0.0), vec3(4.0, 0.0, 0.0), vec3(0.0, 4.0, 0.0)
  };

  EXPECT_TRUE(tmesh.insertAllVertices(vertices));
  HalfFaceIndex hface_id = tmesh.insertTriangle(vec3i(0, 1, 2));
  EXPECT_EQ(hface_id.get(), 0);

  EXPECT_EQ(tmesh.countVertices(), 3);
  EXPECT_EQ(tmesh.countHalfEdges(), 6);
  EXPECT_EQ(tmesh.countHalfFaces(), 1);
  EXPECT_EQ(vertices[0], tmesh.vertex(VertexIndex::create(0)));
  EXPECT_EQ(vertices[1], tmesh.vertex(VertexIndex::create(1)));
  EXPECT_EQ(vertices[2], tmesh.vertex(VertexIndex::create(2)));
}