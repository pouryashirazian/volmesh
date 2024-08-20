#include "volmesh/signeddistancefield.h"
#include "volmesh/logger.h"

using namespace volmesh;

SignedDistanceField::SignedDistanceField() {

}

SignedDistanceField::SignedDistanceField(const SignedDistanceField& rhs) {
  copyFrom(rhs);
}

SignedDistanceField::~SignedDistanceField() {

}

AABB SignedDistanceField::bounds() const {
  return bounds_;
}

real_t SignedDistanceField::voxelSize() const {
  return voxel_size_;
}

vec3i SignedDistanceField::voxelsCount() const {
  const vec3 extent = bounds_.extent();
  const int nx = static_cast<int>(std::ceil(extent.x() / voxel_size_));
  const int ny = static_cast<int>(std::ceil(extent.y() / voxel_size_));
  const int nz = static_cast<int>(std::ceil(extent.z() / voxel_size_));
  return vec3i(nx, ny, nz);
}

vec3i SignedDistanceField::gridPointsCount() const {
  const vec3i voxels_count = voxelsCount();
  return vec3i(voxels_count.x() + 1, voxels_count.y() + 1, voxels_count.z() + 1);
}

uint64_t SignedDistanceField::totalGridPointsCount() const {
  const vec3i gpc = gridPointsCount();
  return static_cast<uint32_t>(gpc.x()) * static_cast<uint32_t>(gpc.y()) * static_cast<uint32_t>(gpc.z());
}

bool SignedDistanceField::generate(const TriangleMesh& in_mesh,
                                   const vec3& expansion,
                                   real_t voxel_size) {
  if(in_mesh.countFaces() == 0) {
    SPDLOG_ERROR("The supplied mesh does not have any faces.");
    return false;
  }

  if(in_mesh.hasVertexPseudoNormals() == false ||
     in_mesh.hasHalfEdgePseudoNormals() == false) {
    SPDLOG_ERROR("The pseudo normals for vertices and half-edges must be computed before generating the SDF");
    return false;
  }

  if(voxel_size == 0.0) {
    SPDLOG_ERROR("Voxel size can not be zero.");
    return false;
  }

  // computes a bounding box that contains the triangle mesh
  bounds_ = in_mesh.bounds();
  bounds_.expand(expansion);

  // store requested voxel size
  voxel_size_ = voxel_size;

  const vec3i voxels_count = voxelsCount();
  SPDLOG_DEBUG("Number of voxels along x, y, and z axes are [{}, {}, {}]",
               voxels_count.x(),
               voxels_count.y(),
               voxels_count.z());
  values_.resize(totalGridPointsCount());

  std::fill(values_.begin(), values_.end(), std::numeric_limits<real_t>::max());

  // process all faces in the mesh
  const uint32_t count_faces = in_mesh.countFaces();
  for(uint32_t i = 0; i < count_faces; i++) {
    const auto hface_vertices = in_mesh.halfFaceVertices(HalfFaceIndex::create(i));

  }

  return true;
}

real_t SignedDistanceField::computeFieldValue(const vec3& p) const {
  return 0.0;
}

SignedDistanceField& SignedDistanceField::operator=(const SignedDistanceField& rhs) {
  copyFrom(rhs);
  return *this;
}