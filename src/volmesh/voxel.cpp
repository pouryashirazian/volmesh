//-----------------------------------------------------------------------------
// Copyright (c) Pourya Shirazian
// All rights reserved.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.
//-----------------------------------------------------------------------------

#include "volmesh/voxel.h"

#include <fmt/core.h>

using namespace volmesh;

Voxel::Voxel():vertices_(CreateVoxel(vec3(0.0f, 0.0f, 0.0f), vec3(2.0f, 2.0f, 2.0f)).vertices()) {

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

real_t Voxel::axisLength(const VoxelAxis axis) const {
  const int bit_selector = 1 << (2 - (int)axis);
  const int complement = (~bit_selector) & 0x07;
  const vec3 start = vertex(VertexLocationId::RTF & complement);
  const vec3 stop = vertex(VertexLocationId::RTF);
  return (stop - start).norm();
}

real_t Voxel::surfaceArea() const {
  const real_t xlen = axisLength(VoxelAxis::X);
  const real_t ylen = axisLength(VoxelAxis::Y);
  const real_t zlen = axisLength(VoxelAxis::Z);
  return (xlen * ylen + xlen * zlen + ylen * zlen);
}

vec3 Voxel::centroid() const {
  vec3 sum = vertex(0);
  for(int i=1; i < Voxel::kNumVerticesPerCell; i++) {
    sum = sum + vertex(i);
  }
  return sum / static_cast<float>(Voxel::kNumVerticesPerCell);
}

real_t Voxel::volume() const {
  return axisLength(VoxelAxis::X) * axisLength(VoxelAxis::Y) * axisLength(VoxelAxis::Z);
}

Voxel Voxel::CreateVoxel(const vec3& center,
                         const vec3& axis_lengths) {
    Voxel::VoxelVertexArray tmp;
    //init format:
    //X: v0.x, v1.x, ..., v7.x
    //Y: v0.y, v1.y, ..., v7.y
    //Z: v0.z, v1.z, ..., v7.z
    const real_t half_width = axis_lengths.x() * 0.5f;
    const real_t half_height = axis_lengths.y() * 0.5f;
    const real_t half_depth = axis_lengths.z() * 0.5f;

    const real_t px = center.x() + half_width;
    const real_t nx = center.x() - half_width;
    const real_t py = center.y() + half_height;
    const real_t ny = center.y() - half_height;
    const real_t pz = center.z() + half_depth;
    const real_t nz = center.z() - half_depth;

    tmp << nx, nx, nx, nx, px, px, px, px,
           ny, ny, py, py, ny, ny, py, py,
           nz, pz, nz, pz, nz, pz, nz, pz;

    return Voxel(tmp);
}

