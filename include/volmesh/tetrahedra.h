#pragma once

#include "volmesh/basetypes.h"

#include <array>

namespace volmesh {

  class Tetrahedra {
  public:
    static const int kNumVerticesPerCell = 4;
    static const int kNumFaces = 4;
    static const int kNumEdges = 6;

    typedef Eigen::Matrix<real_t, 3, kNumVerticesPerCell> VertexArray4;

    Tetrahedra();
    Tetrahedra(const Tetrahedra& rhs);
    explicit Tetrahedra(const VertexArray4& vertices);
    ~Tetrahedra();

    const VertexArray4& vertices() const;

    vec3 vertex(const int vertex_id) const;

    real_t surfaceArea() const;

    /*
     * \ref Shewchuk, J. R. (n.d.). What is a Good Linear Element? Interpolation , Conditioning , and Quality Measures.
     */
    real_t inradius() const;

    real_t circumradius() const;

    real_t aspectRatio() const;

    vec3 centroid() const;

    real_t determinant() const;

    real_t volume() const;

    static vec3i faceIndices(const int face_id);

    static vec2i edgeIndices(const int edge_id);

  private:
    VertexArray4 vertices_;
  };

}