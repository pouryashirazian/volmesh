#pragma once

#include "volmesh/surfacemesh.h"

namespace volmesh {

  static const int kTriangleMeshNumEdgesPerFace = 3;

  template <int kNumEdgesPerFace = kTriangleMeshNumEdgesPerFace>
  struct TriangleMeshLayout {
    typedef HalfFace<kNumEdgesPerFace> HalfFaceType;
  };

  class TriangleMesh: public SurfaceMesh<kTriangleMeshNumEdgesPerFace, TriangleMeshLayout> {
  public:
    TriangleMesh();
    virtual ~TriangleMesh();

    HalfFaceIndex insertTriangle(const uint32_t a, const uint32_t b, const uint32_t c);
    HalfFaceIndex insertTriangle(const vec3i& in_face_vertex_ids);

    bool readFromList(const std::vector<vec3>& in_triangle_vertices,
                      const std::vector<vec3>& in_triangle_normals);

    static vec2i edgeVertexIdsLut(const int edge_id);
    static bool validateFaceVertexIds(const vec3i& in_face_vertex_ids,
                                      const int in_total_vertex_count);
  };

}