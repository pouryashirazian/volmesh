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

    bool readFromList(const std::vector<vec3>& in_vertices,
                      const std::vector<vec3>& in_per_face_normals);
  };

}