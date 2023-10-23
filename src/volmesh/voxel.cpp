#include "volmesh/voxel.h"

#include <fmt/core.h>

using namespace volmesh;

Voxel::Voxel() {

}

Voxel::Voxel(const Voxel& rhs):vertices_(rhs.vertices_) {

}

Voxel::Voxel(const VoxelVertexArray& vertices):vertices_(vertices) {

}

Voxel::~Voxel() {

}

const Voxel::VoxelVertexArray& Voxel::vertices() const {
  return vertices_;
}

vec3 Voxel::vertex(const int vertex_id) const {
  return vertices_.col(vertex_id);
}

real_t Voxel::surfaceArea() const {

}

vec3 Voxel::centroid() const {

}

real_t Voxel::volume() const {

}
