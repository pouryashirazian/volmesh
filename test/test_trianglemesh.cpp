#include "volmesh/trianglemesh.h"
#include "volmesh/stlserializer.h"

#include <gtest/gtest.h>

using namespace volmesh;

static constexpr const real_t kNormalsDirToleranceDegrees = 1.0;

TEST(TriangleMesh, Ctor) {
  std::vector<vec3> vertices = {
    vec3(-4.0, 0.0, 0.0), vec3(4.0, 0.0, 0.0), vec3(0.0, 4.0, 0.0)
  };

  TriangleMesh tmesh;
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
  std::vector<vec3> vertices = {
    vec3(-4.0, 0.0, 0.0), vec3(4.0, 0.0, 0.0), vec3(0.0, 4.0, 0.0)
  };

  TriangleMesh tmesh;
  EXPECT_TRUE(tmesh.insertAllVertices(vertices));
  HalfFaceIndex hface_id = tmesh.insertTriangle(vec3i(0, 1, 2));
  EXPECT_EQ(hface_id.get(), 0);

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
  std::vector<vec3> vertices = {
    vec3(-4.0, 0.0, 0.0), vec3(4.0, 0.0, 0.0), vec3(0.0, 4.0, 0.0)
  };

  TriangleMesh tmesh;
  EXPECT_TRUE(tmesh.insertAllVertices(vertices));
  HalfFaceIndex hface_id = tmesh.insertTriangle(vec3i(0, 1, 2));
  EXPECT_EQ(hface_id.get(), 0);

  for(uint32_t vid = 0; vid < tmesh.countVertices(); vid++) {
    std::vector<HalfEdgeIndex> half_edge_ids;
    EXPECT_TRUE(tmesh.getIncidentHalfEdgesPerVertex(VertexIndex::create(vid), half_edge_ids));

    EXPECT_EQ(half_edge_ids.size(), 2);
  }
}

TEST(TriangleMesh, SetAllHalfFaceNormals) {
  std::vector<vec3> vertices = {
    vec3(-4.0, 0.0, 0.0), vec3(4.0, 0.0, 0.0), vec3(0.0, 4.0, 0.0)
  };

  TriangleMesh tmesh;
  EXPECT_TRUE(tmesh.insertAllVertices(vertices));
  HalfFaceIndex hface_id = tmesh.insertTriangle(vec3i(0, 1, 2));
  EXPECT_EQ(hface_id.get(), 0);

  EXPECT_FALSE(tmesh.hasHalfFaceNormals());
  EXPECT_THROW(tmesh.halfFaceNormal(HalfFaceIndex::create(0)), std::out_of_range);

  std::vector<vec3> normals = {vec3(0, 0, 1)};
  tmesh.setAllHalfFaceNormals(normals);

  EXPECT_TRUE(tmesh.hasHalfFaceNormals());
  EXPECT_NO_THROW(tmesh.halfFaceNormal(HalfFaceIndex::create(0)));
}

TEST(TriangleMesh, ComputeHalfFaceNormals) {
  std::vector<vec3> vertices = {
    vec3(-4.0, 0.0, 0.0), vec3(4.0, 0.0, 0.0), vec3(0.0, 4.0, 0.0)
  };

  TriangleMesh tmesh;
  EXPECT_TRUE(tmesh.insertAllVertices(vertices));
  HalfFaceIndex hface_id = tmesh.insertTriangle(vec3i(0, 1, 2));
  EXPECT_EQ(hface_id.get(), 0);

  EXPECT_FALSE(tmesh.hasHalfFaceNormals());
  EXPECT_THROW(tmesh.halfFaceNormal(HalfFaceIndex::create(0)), std::out_of_range);

  tmesh.computeHalfFaceNormals();

  EXPECT_TRUE(tmesh.hasHalfFaceNormals());
  EXPECT_NO_THROW(tmesh.halfFaceNormal(HalfFaceIndex::create(0)));

  const vec3 computed_normal = tmesh.halfFaceNormal(HalfFaceIndex::create(0));
  EXPECT_TRUE(NormalsDirectionMatch(computed_normal, vec3(0.0, 0.0, 1.0), kNormalsDirToleranceDegrees));
}

TEST(TriangleMesh, ComputeHalfEdgePseudoNormals) {
  std::vector<vec3> vertices = {
    vec3(-4.0, 0.0, 0.0), vec3(4.0, 0.0, 0.0), vec3(0.0, 4.0, 0.0)
  };

  TriangleMesh tmesh;
  EXPECT_TRUE(tmesh.insertAllVertices(vertices));
  HalfFaceIndex hface_id = tmesh.insertTriangle(vec3i(0, 1, 2));
  EXPECT_EQ(hface_id.get(), 0);

  EXPECT_FALSE(tmesh.hasHalfEdgePseudoNormals());
  EXPECT_THROW(tmesh.halfEdgePseudoNormal(HalfEdgeIndex::create(0)), std::out_of_range);

  tmesh.computeHalfEdgePseudoNormals();

  EXPECT_TRUE(tmesh.hasHalfEdgePseudoNormals());
  EXPECT_NO_THROW(tmesh.halfEdgePseudoNormal(HalfEdgeIndex::create(0)));

  for(uint32_t i=0; i < tmesh.countHalfEdges(); i++) {
    const vec3 computed_normal = tmesh.halfEdgePseudoNormal(HalfEdgeIndex::create(i)).normalized();
    EXPECT_TRUE(NormalsDirectionMatch(computed_normal, vec3(0.0, 0.0, 1.0), kNormalsDirToleranceDegrees));
  }
}

TEST(TriangleMesh, ComputeVertexPseudoNormals) {
  std::vector<vec3> vertices = {
    vec3(0.0, 0.0, 0.0), vec3(1.0, 1.0, 0.0), vec3(0.0, 2.0, 0.0), vec3(0.0, 1.0, 1.0)
  };

  TriangleMesh tmesh;
  EXPECT_TRUE(tmesh.insertAllVertices(vertices));
  tmesh.insertTriangle(vec3i(0, 1, 2));
  tmesh.insertTriangle(vec3i(0, 3, 2));

  tmesh.computeVertexPseudoNormals();

  // check existence of all normals
  EXPECT_TRUE(tmesh.hasHalfFaceNormals());
  EXPECT_TRUE(tmesh.hasVertexPseudoNormals());

  // check face normals
  const std::vector<vec3> expected_face_normals = { vec3(0.0, 0.0, 1.0), vec3(-1.0, 0.0, 0.0) };

  EXPECT_TRUE(NormalsDirectionMatch(tmesh.halfFaceNormal(HalfFaceIndex::create(0)),
                                    expected_face_normals[0],
                                    kNormalsDirToleranceDegrees));

  EXPECT_TRUE(NormalsDirectionMatch(tmesh.halfFaceNormal(HalfFaceIndex::create(1)),
                                    expected_face_normals[1],
                                    kNormalsDirToleranceDegrees));

  // check vertex pseudo normals
  const vec3 seam_vertex_normal = (expected_face_normals[0] + expected_face_normals[1]).normalized();
  const std::vector<vec3> expected_vertex_normals = { seam_vertex_normal, vec3(0.0, 0.0, 1.0),
                                                      seam_vertex_normal, vec3(-1.0, 0.0, 0.0) };

  for(uint32_t i=0; i < tmesh.countVertices(); i++) {
    EXPECT_TRUE(NormalsDirectionMatch(tmesh.vertexPseudoNormal(VertexIndex::create(i)),
                                      expected_vertex_normals[i],
                                      kNormalsDirToleranceDegrees));
  }
}
