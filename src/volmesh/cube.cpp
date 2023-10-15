#include "volmesh/cube.h"

#include <fmt/core.h>

using namespace volmesh;

Cube::Cube() {

}

Cube::Cube(const Cube& rhs):vertices_(rhs.vertices_) {

}

Cube::Cube(const VertexArray8& vertices):vertices_(vertices) {

}

Cube::~Cube() {

}

const Cube::VertexArray8& Cube::vertices() const {
  return vertices_;
}

vec3 Cube::vertex(const int vertex_id) const {
  return vertices_.col(vertex_id);
}

real_t Cube::surfaceArea() const {

}

vec3 Cube::centroid() const {

}

real_t Cube::volume() const {

}
