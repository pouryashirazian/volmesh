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

TEST(TriangleMesh, IncidentHalfFacesPerHalfEdge) {
  TriangleMesh tmesh;
  std::vector<vec3> vertices = {
    vec3(-4.0, 0.0, 0.0), vec3(4.0, 0.0, 0.0), vec3(0.0, 4.0, 0.0)
  };

  EXPECT_TRUE(tmesh.insertAllVertices(vertices));
  HalfFaceIndex hface_id = tmesh.insertTriangle(vec3i(0, 1, 2));

  for(uint32_t he = 0; he < tmesh.countHalfEdges(); he++) {
    std::vector<HalfFaceIndex> half_face_ids;
    EXPECT_TRUE(tmesh.getIncidentHalfFacesPerHalfEdge(HalfEdgeIndex::create(he), half_face_ids));

    bool is_forward = (he % 2 == 0);
    if(is_forward) {
      EXPECT_EQ(half_face_ids.size(), 1);
      EXPECT_EQ(half_face_ids[0].get(), 0);
    } else {
      EXPECT_EQ(half_face_ids.size(), 0);
    }
  }
}

TEST(TriangleMesh, IncidentHalfEdgesPerVertex) {
  TriangleMesh tmesh;
  std::vector<vec3> vertices = {
    vec3(-4.0, 0.0, 0.0), vec3(4.0, 0.0, 0.0), vec3(0.0, 4.0, 0.0)
  };

  EXPECT_TRUE(tmesh.insertAllVertices(vertices));
  HalfFaceIndex hface_id = tmesh.insertTriangle(vec3i(0, 1, 2));

  for(uint32_t vid = 0; vid < tmesh.countVertices(); vid++) {
    std::vector<HalfEdgeIndex> half_edge_ids;
    EXPECT_TRUE(tmesh.getIncidentHalfEdgesPerVertex(VertexIndex::create(vid), half_edge_ids));

    EXPECT_EQ(half_edge_ids.size(), 2);
  }
}

TEST(TriangleMesh, HalfFaceNormals) {
  TriangleMesh tmesh;
  std::vector<vec3> vertices = {
    vec3(-4.0, 0.0, 0.0), vec3(4.0, 0.0, 0.0), vec3(0.0, 4.0, 0.0)
  };

  EXPECT_TRUE(tmesh.insertAllVertices(vertices));
  HalfFaceIndex hface_id = tmesh.insertTriangle(vec3i(0, 1, 2));

  EXPECT_THROW(tmesh.halfFaceNormal(HalfFaceIndex::create(0)), std::out_of_range);

  std::vector<vec3> normals = {vec3(0, 0, 1)};
  tmesh.setAllHalfFaceNormals(normals);
  EXPECT_NO_THROW(tmesh.halfFaceNormal(HalfFaceIndex::create(0)));
}