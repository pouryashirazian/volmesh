#include "volmesh/signeddistancefield.h"
#include "volmesh/logger.h"
#include "volmesh/mathutils.h"

using namespace volmesh;

SignedDistanceField::SignedDistanceField() {

}

SignedDistanceField::SignedDistanceField(const SignedDistanceField& rhs) {
  copyFrom(rhs);
}

SignedDistanceField::~SignedDistanceField() {

}

void SignedDistanceField::copyFrom(const SignedDistanceField& rhs) {
  voxel_size_ = rhs.voxel_size_;
  bounds_ = rhs.bounds_;
  values_ = rhs.values_;
  signs_ = rhs.signs_;
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

uint64_t SignedDistanceField::gridPointId(const vec3i& coords) const {
  const vec3i gridpoints_count = gridPointsCount();
  if ((coords.x() < 0 || coords.x() >= gridpoints_count.x())||
      (coords.y() < 0 || coords.y() >= gridpoints_count.y())||
      (coords.z() < 0 || coords.z() >= gridpoints_count.z())) {
    std::string message = fmt::format("The supplied grid point id [{}, {}, {}] is out of range, \
                                       Correct values must be in range x = [{}, {}], y = [{}, {}], z = [{}, {}].",
                                       coords.x(), coords.y(), coords.z(),
                                       0, gridpoints_count.x() - 1,
                                       0, gridpoints_count.y() - 1,
                                       0, gridpoints_count.z() - 1);
    throw std::out_of_range(message);
  }

  const uint32_t nx = static_cast<uint32_t>(gridpoints_count.x());
  const uint32_t ny = static_cast<uint32_t>(gridpoints_count.y());
  const uint32_t nz = static_cast<uint32_t>(gridpoints_count.z());
  return static_cast<uint32_t>(coords.z()) * nx * ny +
          static_cast<uint32_t>(coords.y()) * nx +
          static_cast<uint32_t>(coords.x());
}

vec4 SignedDistanceField::gridPointVertexAndFieldValue(const vec3i& coords) const {
  const uint64_t gridpoint_id = gridPointId(coords);
  const vec3 position = bounds_.lower() +
                        vec3(static_cast<real_t>(coords.x()) * voxel_size_,
                             static_cast<real_t>(coords.y()) * voxel_size_,
                             static_cast<real_t>(coords.z()) * voxel_size_);

  return vec4(position.x(), position.y(), position.z(), values_[gridpoint_id]);
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
  signs_.resize(totalGridPointsCount());

  // initialize the voxel grid by settings all values to +infinity
  std::fill(values_.begin(), values_.end(), std::numeric_limits<real_t>::max());
  std::fill(signs_.begin(), signs_.end(), 0.0);

  // process all faces in the mesh
  const uint32_t count_faces = in_mesh.countFaces();
  const vec3i gridpoints_count = gridPointsCount();

  for(uint32_t iface = 0; iface < count_faces; iface++) {

    const HalfFaceIndex hface_id = HalfFaceIndex::create(iface);

    const TriangleMesh::HalfFaceType hface = in_mesh.halfFace(hface_id);
    const auto hface_vertices = in_mesh.halfFaceVertices(hface_id);

    std::array<uint32_t, 3> face_vertex_ids;
    for(uint32_t i=0; i < 3; i++) {
      face_vertex_ids[i] = in_mesh.halfEdge(hface.halfEdgeIndex(i)).start();
    }

    // loop over all voxels and compute the distance to all voxel points
    for(int x = 0; x < gridpoints_count.x(); x++) {
      for(int y = 0; y < gridpoints_count.y(); y++) {
        for(int z = 0; z < gridpoints_count.z(); z++) {
          const vec3i coords = vec3i(x, y, z);
          const vec4 xyzf = gridPointVertexAndFieldValue(coords);
          const vec3 p = xyzf.head<3>();
          vec3 q(0.0, 0.0, 0.0);
          ClosestTriangleFeature closest_feature;

          const real_t dist = PointTriangleDistance(p,
                                                    hface_vertices[0],
                                                    hface_vertices[1],
                                                    hface_vertices[2],
                                                    q,
                                                    closest_feature);
          if (dist < xyzf.w()) {
            // compute distance sign
            vec3 pseudo_normal = in_mesh.halfFaceNormal(hface_id);

            // fetch normal at the closest feature to q
            switch(closest_feature) {
              case(ClosestTriangleFeature::kClosestTriangleFeatureVertexA): {
                pseudo_normal = in_mesh.vertexPseudoNormal(VertexIndex::create(face_vertex_ids[0]));
                break;
              }
              case(ClosestTriangleFeature::kClosestTriangleFeatureVertexB): {
                pseudo_normal = in_mesh.vertexPseudoNormal(VertexIndex::create(face_vertex_ids[1]));
                break;
              }
              case(ClosestTriangleFeature::kClosestTriangleFeatureVertexC): {
                pseudo_normal = in_mesh.vertexPseudoNormal(VertexIndex::create(face_vertex_ids[2]));
                break;
              }
              case(ClosestTriangleFeature::kClosestTriangleFeatureEdgeAB): {
                pseudo_normal = in_mesh.halfEdgePseudoNormal(hface.halfEdgeIndex(0));
                break;
              }
              case(ClosestTriangleFeature::kClosestTriangleFeatureEdgeBC): {
                pseudo_normal = in_mesh.halfEdgePseudoNormal(hface.halfEdgeIndex(1));
                break;
              }
              case(ClosestTriangleFeature::kClosestTriangleFeatureEdgeCA): {
                pseudo_normal = in_mesh.halfEdgePseudoNormal(hface.halfEdgeIndex(2));
                break;
              }
              case(ClosestTriangleFeature::kClosestTriangleFeatureNone):
              case(ClosestTriangleFeature::kClosestTriangleFeatureInside): {
                // no changes
              }
            }

            // directional vector from the intersection point q on the triangle, and the grid point p
            const vec3 dir_vec_qp = p - q;

            const uint64_t gridpoint_id = gridPointId(coords);
            const real_t current_distance = values_[gridpoint_id];

            if (dist < current_distance) {
              values_[gridpoint_id] = dist;

              // replace sign
              signs_[gridpoint_id] = dir_vec_qp.dot(pseudo_normal);
            } else if (FuzzyCompare(dist, current_distance) == true) {
              values_[gridpoint_id] = dist;

              // accumulate sign
              signs_[gridpoint_id] += dir_vec_qp.dot(pseudo_normal);
            }
          }
        }
      }
    }

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