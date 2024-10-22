//-----------------------------------------------------------------------------
// Copyright (c) Pourya Shirazian
// All rights reserved.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.
//-----------------------------------------------------------------------------

#include "volmesh/signeddistancefield.h"
#include "volmesh/logger.h"
#include "volmesh/mathutils.h"

#include <fstream>
#include <filesystem>
#include <chrono>
#include <tinyxml2.h>

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

  std::lock_guard<std::mutex> lk(field_values_mutex_);
  field_values_ = rhs.field_values_;
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

uint64_t SignedDistanceField::getTotalMemoryUsageInBytes() const {
  return sizeof(voxel_size_) + sizeof(bounds_) + totalGridPointsCount() * 2 * sizeof(real_t);
}

uint64_t SignedDistanceField::gridPointId(const vec3i& coords) const {
  assertGridPointCoords(coords);

  const vec3i gridpoints_count = gridPointsCount();
  const uint32_t nx = static_cast<uint32_t>(gridpoints_count.x());
  const uint32_t ny = static_cast<uint32_t>(gridpoints_count.y());
  const uint32_t nz = static_cast<uint32_t>(gridpoints_count.z());
  return static_cast<uint32_t>(coords.z()) * nx * ny +
          static_cast<uint32_t>(coords.y()) * nx +
          static_cast<uint32_t>(coords.x());
}

vec3 SignedDistanceField::gridPointPosition(const vec3i& coords) const {
  assertGridPointCoords(coords);

  const vec3 position = bounds_.lower() +
                        vec3(static_cast<real_t>(coords.x()) * voxel_size_,
                             static_cast<real_t>(coords.y()) * voxel_size_,
                             static_cast<real_t>(coords.z()) * voxel_size_);
  return position;
}

vec4 SignedDistanceField::gridPointPositionAndFieldValue(const vec3i& coords) const {
  // gridPointPosition validates the coords
  const vec3 position = gridPointPosition(coords);

  real_t field_value = 0.0;
  {
    std::lock_guard<std::mutex> lk(field_values_mutex_);
    field_value = field_values_[gridPointId(coords)];
  }

  return vec4(position.x(), position.y(), position.z(), field_value);
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

  std::vector<real_t> magnitudes(totalGridPointsCount());
  std::vector<real_t> signs(totalGridPointsCount());

  // initialize the voxel grid by settings all values to +infinity
  std::fill(magnitudes.begin(), magnitudes.end(), std::numeric_limits<real_t>::max());
  std::fill(signs.begin(), signs.end(), 0.0);

  // process all faces in the mesh
  const uint32_t count_faces = in_mesh.countFaces();
  const vec3i gridpoints_count = gridPointsCount();

  const vec3i voxels_count = voxelsCount();

  // print some debug info
  SPDLOG_DEBUG("Mesh faces = [{}], vertices = [{}]", in_mesh.countFaces(), in_mesh.countVertices());

  SPDLOG_DEBUG("Number of voxels along x, y, and z axes are [{}, {}, {}], voxel size = [{}]",
               voxels_count.x(),
               voxels_count.y(),
               voxels_count.z(),
               voxel_size_);
  SPDLOG_DEBUG("Total grid points count = [{}]", totalGridPointsCount());
  SPDLOG_DEBUG("Total memory usage by SDF in bytes = [{}]", getTotalMemoryUsageInBytes());

  static const int kProgressReportPeriodSeconds = 5;

  // start timer
  auto t1 = std::chrono::high_resolution_clock::now();

  uint64_t last_report_timestamp_seconds = 0;

  for(uint32_t iface = 0; iface < count_faces; iface++) {
    auto tc = std::chrono::high_resolution_clock::now();
    auto current_duration_seconds = std::chrono::duration_cast<std::chrono::seconds>(tc - t1);
    if (current_duration_seconds.count() % kProgressReportPeriodSeconds == 0 &&
        last_report_timestamp_seconds != current_duration_seconds.count()) {
      const double progress = (static_cast<double>(iface + 1) * 100.0) / static_cast<double>(count_faces);
      SPDLOG_INFO("progress [{} of {}], {:.2f} %", iface + 1, count_faces, progress);
      last_report_timestamp_seconds = current_duration_seconds.count();
    }

    const HalfFaceIndex hface_id = HalfFaceIndex::create(iface);

    const TriangleMesh::HalfFaceType hface = in_mesh.halfFace(hface_id);
    const auto hface_vertices = in_mesh.halfFaceVertices(hface_id);

    // compute face bounding box and extend it by the width of the transition region
    AABB face_aabb = ComputeTriangleAABB(hface_vertices);
    face_aabb.expand(vec3(2.0 * voxel_size_, 2.0 * voxel_size_, 2.0 * voxel_size_));

    const vec3 face_aabb_lower_local = (face_aabb.lower() - bounds_.lower()) / voxel_size_;

    const int start_x = std::max(0, static_cast<int>(std::floor(face_aabb_lower_local.x())));
    const int start_y = std::max(0, static_cast<int>(std::floor(face_aabb_lower_local.y())));
    const int start_z = std::max(0, static_cast<int>(std::floor(face_aabb_lower_local.z())));

    const vec3 face_aabb_voxels_extent = face_aabb.extent() / voxel_size_;
    const int stop_x = std::min(gridpoints_count.x(), start_x + static_cast<int>(std::ceil(face_aabb_voxels_extent.x())));
    const int stop_y = std::min(gridpoints_count.y(), start_y + static_cast<int>(std::ceil(face_aabb_voxels_extent.y())));
    const int stop_z = std::min(gridpoints_count.z(), start_z + static_cast<int>(std::ceil(face_aabb_voxels_extent.z())));

    std::array<uint32_t, 3> face_vertex_ids;
    for(uint32_t i=0; i < 3; i++) {
      face_vertex_ids[i] = in_mesh.halfEdge(hface.halfEdgeIndex(i)).start();
    }

    // loop over all voxels and compute the distance to all voxel points
    for(int z = start_z; z < stop_z; z++) {
      for(int y = start_y; y < stop_y; y++) {
        for(int x = start_x; x < stop_x; x++) {

          const vec3i coords = vec3i(x, y, z);
          const vec3 p = gridPointPosition(coords);
          const real_t prev_dist = magnitudes[gridPointId(coords)];

          vec3 q(0.0, 0.0, 0.0);

          ClosestTriangleFeature closest_feature;

          const real_t dist = PointTriangleDistance(p,
                                                    hface_vertices[0],
                                                    hface_vertices[1],
                                                    hface_vertices[2],
                                                    q,
                                                    closest_feature);

          if ((dist < prev_dist) || FuzzyCompare(dist, prev_dist) == true) {
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

            if (dist < prev_dist) {
              magnitudes[gridpoint_id] = dist;

              // replace sign
              signs[gridpoint_id] = dir_vec_qp.dot(pseudo_normal);
            } else if (FuzzyCompare(dist, prev_dist) == true) {
              magnitudes[gridpoint_id] = dist;

              // accumulate sign
              signs[gridpoint_id] += dir_vec_qp.dot(pseudo_normal);
            }
          } // if dist
        }
      }
    }
  } // end for face

  // save the final field values
  {
    std::lock_guard<std::mutex> lk(field_values_mutex_);
    field_values_.resize(totalGridPointsCount());

    for (size_t i=0; i < field_values_.size(); ++i) {
      field_values_[i] = (signs[i] >= 0) ? magnitudes[i] : - magnitudes[i];
    }
  }

  auto t2 = std::chrono::high_resolution_clock::now();

  auto duration_milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1);
  SPDLOG_INFO("Total time spent in generating SDF = [{}.{}] seconds.",
               duration_milliseconds.count() / 1000,
               duration_milliseconds.count() % 1000);

  return true;
}

real_t SignedDistanceField::fieldValue(const vec3& p) const {
  // field is zero outside the bounding box
  if (bounds_.contains(p) == false) {
    return 0.0;
  }

  // find voxel coordinates

  // map p to the local coordinate system of the voxel grid
  vec3 q = (p - bounds_.lower()) / voxel_size_;

  // voxel coords
  const int ix = static_cast<int>(std::floor(q.x()));
  const int iy = static_cast<int>(std::floor(q.y()));
  const int iz = static_cast<int>(std::floor(q.z()));

  // setup interpolators
  const real_t tx = q.x() - static_cast<real_t>(ix);
  const real_t ty = q.y() - static_cast<real_t>(iy);
  const real_t tz = q.z() - static_cast<real_t>(iz);

  //vXYZ
  const real_t v000 = fieldValue(vec3i(ix, iy, iz));
  const real_t v001 = fieldValue(vec3i(ix, iy, iz + 1));
  const real_t v010 = fieldValue(vec3i(ix, iy + 1, iz));
  const real_t v011 = fieldValue(vec3i(ix, iy + 1, iz + 1));
  const real_t v100 = fieldValue(vec3i(ix + 1, iy, iz));
  const real_t v101 = fieldValue(vec3i(ix + 1, iy, iz + 1));
  const real_t v110 = fieldValue(vec3i(ix + 1, iy + 1, iz));
  const real_t v111 = fieldValue(vec3i(ix + 1, iy + 1, iz + 1));

  real_t result = (1 - tz) * ((1 - ty) * (v000 * (1 - tx) + v100 * tx) +
                                   ty  * (v010 * (1 - tx) + v110 * tx)) +
                  tz * ((1 - ty) * (v001 * (1 - tx) + v101 * tx) +
                        ty  * (v011 * (1 - tx) + v111 * tx));

  return result;
}

real_t SignedDistanceField::fieldValue(const vec3i& coords) const {
  // gridPointId will assert coords
  const uint64_t gridpoint_id = gridPointId(coords);

  real_t field_value = 0.0;

  {
    std::lock_guard<std::mutex> lk(field_values_mutex_);
    field_value = field_values_[gridpoint_id];
  }

  return field_value;
}

bool SignedDistanceField::saveAsVTI(const std::string& filepath) const {
  if (totalGridPointsCount() == 0) {
    SPDLOG_ERROR("This instance is empty. Initialize before saving to disk");
    return false;
  }

  if (std::filesystem::exists(filepath)) {
    SPDLOG_WARN("Another file with the same name exists under [{}] and will be overwritten.", filepath.c_str());
  }

  std::ofstream file(filepath);
  if (!file.is_open()) {
    SPDLOG_ERROR("Failed to open file [{}] for writing", filepath.c_str());
    return false;
  }

  const vec3i gridpoints_count = gridPointsCount();

  // Write the VTI XML header
  file << "<?xml version=\"1.0\"?>\n";
  file << "<VTKFile type=\"ImageData\" version=\"0.1\" byte_order=\"LittleEndian\">\n";
  file << "  <ImageData WholeExtent=\"0 " << gridpoints_count.x() - 1 << " 0 " << gridpoints_count.y() - 1 << " 0 " << gridpoints_count.z() - 1 \
        << "\" Origin=\"" << bounds_.lower().x() << " " << bounds_.lower().y() << " " << bounds_.lower().z() \
        << "\" Spacing=\"" << voxel_size_ << " " << voxel_size_ << " " << voxel_size_ << "\">\n";
  file << "    <Piece Extent=\"0 " << gridpoints_count.x() - 1 << " 0 " << gridpoints_count.y() - 1 << " 0 " << gridpoints_count.z() - 1 << "\">\n";
  file << "      <PointData Scalars=\"SignedDistanceField\">\n";
  file << "        <DataArray type=\"Float32\" Name=\"SignedDistanceField\" format=\"ascii\">\n";

  // Write the SDF values
  for(int z = 0; z < gridpoints_count.z(); z++) {
    for(int y = 0; y < gridpoints_count.y(); y++) {
      for(int x = 0; x < gridpoints_count.x(); x++) {
        file << static_cast<float>(fieldValue(vec3i(x, y, z))) << " ";
        if ((x + 1) % gridpoints_count.x() == 0) {
          file << "\n";
        }
      }
    }
  }

  // Close the XML tags
  file << "        </DataArray>\n";
  file << "      </PointData>\n";
  file << "      <CellData>\n";
  file << "      </CellData>\n";
  file << "    </Piece>\n";
  file << "  </ImageData>\n";
  file << "</VTKFile>\n";

  file.close();

  return true;
}

bool SignedDistanceField::loadAsVTI(const std::string& filepath) {
  struct ImageData {
    vec3i gridpoints_count;
    float spacing[3];
    float origin[3];
    std::string scalar_type;
    std::string scalar_data;
  };

  ImageData image_data;
  tinyxml2::XMLDocument doc;

  // Load the .vti file
  tinyxml2::XMLError eResult = doc.LoadFile(filepath.c_str());
  if (eResult != tinyxml2::XML_SUCCESS) {
    SPDLOG_ERROR("Failed loading VTI file: {}", filepath.c_str());
    return false;
  }

  // Get the ImageData node
  tinyxml2::XMLElement* root = doc.FirstChildElement("VTKFile");
  if (!root) {
    SPDLOG_ERROR("No VTKFile element found.");
    return false;
  }

  tinyxml2::XMLElement* imageDataElement = root->FirstChildElement("ImageData");
  if (!imageDataElement) {
    SPDLOG_ERROR("No ImageData element found.");
    return false;
  }

  // Get the WholeExtent attribute (describes the dimensions of the volume)
  const char* extentStr = imageDataElement->Attribute("WholeExtent");
  if (extentStr) {
    int ext[6];
    std::sscanf(extentStr, "%d %d %d %d %d %d",
                &ext[0], &ext[1], &ext[2], &ext[3], &ext[4], &ext[5]);

    // Calculate the dimensions from the extents
    image_data.gridpoints_count.x() = ext[1] - ext[0] + 1;
    image_data.gridpoints_count.y() = ext[3] - ext[2] + 1;
    image_data.gridpoints_count.z() = ext[5] - ext[4] + 1;

    SPDLOG_DEBUG("grid points dimension = {} x {} x {}",
                  image_data.gridpoints_count.x(),
                  image_data.gridpoints_count.y(),
                  image_data.gridpoints_count.z());
  } else {
    SPDLOG_ERROR("WholeExtent attribute not found.");
    return false;
  }

  // Get the Origin attribute (defines the origin of the volume in world coordinates)
  const char* originStr = imageDataElement->Attribute("Origin");
  if (originStr) {
    std::sscanf(originStr, "%f %f %f",
                &image_data.origin[0],
                &image_data.origin[1],
                &image_data.origin[2]);

    SPDLOG_DEBUG("Origin: ({}, {}, {})",
                 image_data.origin[0],
                 image_data.origin[1],
                 image_data.origin[2]);
  } else {
    SPDLOG_ERROR("Origin attribute not found.");
    return false;
  }

  // Get the Spacing attribute (defines the voxel size)
  const char* spacingStr = imageDataElement->Attribute("Spacing");
  if (spacingStr) {
    std::sscanf(spacingStr, "%f %f %f",
                &image_data.spacing[0],
                &image_data.spacing[1],
                &image_data.spacing[2]);

    SPDLOG_DEBUG("Spacing: ({}, {}, {})",
                  image_data.spacing[0],
                  image_data.spacing[1],
                  image_data.spacing[2]);
  } else {
    SPDLOG_ERROR("Spacing attribute not found.");
    return false;
  }

  // Get PointData (which usually contains scalar data)
  tinyxml2::XMLElement* pointDataElement = imageDataElement->FirstChildElement("Piece")
                                                        ->FirstChildElement("PointData");
  if (!pointDataElement) {
    SPDLOG_ERROR("No PointData element found.");
    return false;
  }

  // Read the scalar data type
  tinyxml2::XMLElement* dataArrayElement = pointDataElement->FirstChildElement("DataArray");
  if (dataArrayElement) {
    image_data.scalar_type = dataArrayElement->Attribute("type");
    image_data.scalar_data = dataArrayElement->GetText();

    if (image_data.scalar_type != "Float32") {
      SPDLOG_ERROR("The scalar type {} is not supported.", image_data.scalar_type.c_str());
      return false;
    }
  } else {
    SPDLOG_ERROR("No scalar DataArray found.");
    return false;
  }

  // load the VTI data
  if (FuzzyCompare(image_data.spacing[0], image_data.spacing[1]) == false ||
      FuzzyCompare(image_data.spacing[0], image_data.spacing[2]) == false) {
    SPDLOG_ERROR("Non uniform voxel grid is not supported");
    return false;
  }

  voxel_size_ = image_data.spacing[0];

  const vec3 origin(image_data.origin[0], image_data.origin[1], image_data.origin[2]);
  const vec3 voxels_count(static_cast<real_t>(image_data.gridpoints_count[0] - 1),
                          static_cast<real_t>(image_data.gridpoints_count[1] - 1),
                          static_cast<real_t>(image_data.gridpoints_count[2] - 1));
  const vec3 aabb_extent = image_data.spacing[0] * voxels_count;
  bounds_ = AABB(origin, origin + aabb_extent);

  // load field values
  bool result = false;

  {
    std::lock_guard<std::mutex> lock(field_values_mutex_);
    field_values_.reserve(image_data.gridpoints_count[0] * image_data.gridpoints_count[1] * image_data.gridpoints_count[2]);
    result = parseAsciiValues(image_data.scalar_data, field_values_);
  }

  return result;
}

// Function to parse ASCII values, including special cases like infinity
bool SignedDistanceField::parseAsciiValues(const std::string& in_data_string,
                                           std::vector<real_t>& out_data_values) {
    std::stringstream ss(in_data_string);
    std::string token;

    // Split by whitespace and handle each token
    while (ss >> token) {
      float value;
      if (token == "inf" || token == "+inf" || token == "Inf") {
        value = std::numeric_limits<float>::max();
      } else if (token == "-inf" || token == "-Inf") {
        value = -std::numeric_limits<float>::max();
      } else {
        try {
          value = std::stof(token);  // Convert string to float
        } catch (const std::invalid_argument&) {
          SPDLOG_ERROR("Could not parse token [{}] as float.", token.c_str());
          continue;
        }
      }

      out_data_values.push_back(static_cast<real_t>(value));
    }

    return true;
}

SignedDistanceField& SignedDistanceField::operator=(const SignedDistanceField& rhs) {
  copyFrom(rhs);
  return *this;
}

bool SignedDistanceField::isValidGridPointCoords(const vec3i& coords, const vec3i& gridpoints_count) const {
  bool is_valid = coords.x() >= 0 && coords.x() < gridpoints_count.x() && \
                  coords.y() >= 0 && coords.y() < gridpoints_count.y() && \
                  coords.z() >= 0 && coords.z() < gridpoints_count.z();
  return is_valid;
}

void SignedDistanceField::assertGridPointCoords(const vec3i& coords) const {
  const vec3i gridpoints_count = gridPointsCount();

  if(isValidGridPointCoords(coords, gridpoints_count) == false) {
    std::string message = fmt::format("The supplied grid point id [{}, {}, {}] is out of range, \
                                       Correct values must be in range x = [{}, {}], y = [{}, {}], z = [{}, {}].",
                                       coords.x(), coords.y(), coords.z(),
                                       0, gridpoints_count.x() - 1,
                                       0, gridpoints_count.y() - 1,
                                       0, gridpoints_count.z() - 1);
    throw std::out_of_range(message);
  }
}
