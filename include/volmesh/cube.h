#pragma once

#include "volmesh/basetypes.h"

#include <array>

namespace volmesh {

  class Cube {
  public:
    static const int kNumVerticesPerCell = 8;
    static const int kNumFaces = 6;
    static const int kNumEdges = 12;
    static const int kNumFittingTetrahedra = 6;

    typedef Eigen::Matrix<real_t, 3, kNumVerticesPerCell> VertexArray8;

    //Vertex location Id
    enum VertexLocationId : int {
      LBN = 0, LBF = 1,
      LTN = 2, LTF = 3,
      RBN = 4, RBF = 5,
      RTN = 6, RTF = 7
    };

    Cube();
    Cube(const Cube& rhs);
    explicit Cube(const VertexArray8& vertices);
    ~Cube();

    const VertexArray8& vertices() const;

    vec3 vertex(const int vertex_id) const;

    real_t surfaceArea() const;

    vec3 centroid() const;

    real_t volume() const;

  private:
    VertexArray8 vertices_;
  };

}