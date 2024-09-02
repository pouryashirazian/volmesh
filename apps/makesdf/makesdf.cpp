#include "volmesh/logger.h"
#include "volmesh/tetmesh.h"
#include "volmesh/stlserializer.h"
#include "volmesh/signeddistancefield.h"

#include <iostream>
#include <filesystem>
#include <fmt/core.h>
#include <cxxopts.hpp>

using namespace volmesh;

namespace fs = std::filesystem;

int main(int argc, const char* argv[]) {
  SetLogFormat();

  cxxopts::Options options("makesdf", "Generate signed distance field from 3D surface meshes.");

  options.add_options()
    ("i,input", "Input surface mesh (STL format only)", cxxopts::value<std::string>())
    ("o,output", "Output SDF in the VTK Image Data (VTI format)", cxxopts::value<std::string>())
    ("h,help", "Print usage")
  ;

  auto args = options.parse(argc, argv);

  if (args.count("help")) {
    std::cout << options.help() << std::endl;
    exit(0);
  }

  std::string surface_mesh_filepath = args["input"].as<std::string>();
  SPDLOG_INFO("input surface mesh at [{}]", surface_mesh_filepath.c_str());

  std::string sdf_filepath = args["output"].as<std::string>();
  SPDLOG_INFO("output filepath at [{}]", sdf_filepath.c_str());

  TriangleMesh tri_mesh;
  if (volmesh::ReadSTL(surface_mesh_filepath, tri_mesh) == false) {
    SPDLOG_ERROR("Failed to load the surface mesh file [{}]", surface_mesh_filepath.c_str());
    return EXIT_FAILURE;
  }

  AABB bounds = tri_mesh.bounds();
  SPDLOG_INFO("Mesh lower bounds [{}, {}, {}]", bounds.lower().x(), bounds.lower().y(), bounds.lower().z());
  SPDLOG_INFO("Mesh upper bounds [{}, {}, {}]", bounds.upper().x(), bounds.upper().y(), bounds.upper().z());
  SPDLOG_INFO("Mesh AABB extent [{}, {}, {}]", bounds.extent().x(), bounds.extent().y(), bounds.extent().z());

  tri_mesh.computeHalfEdgePseudoNormals();
  tri_mesh.computeVertexPseudoNormals();

  const real_t voxel_size = 0.1;
  SignedDistanceField sdf;
  bool result = sdf.generate(tri_mesh, vec3(voxel_size, voxel_size, voxel_size), voxel_size);
  if (result == true) {
    if (sdf.save(sdf_filepath)) {
      SPDLOG_INFO("Saved SDF under [{}]", sdf_filepath.c_str());
    } else {
      SPDLOG_ERROR("Failed when saving the SDF under [{}].", sdf_filepath.c_str());
    }
  } else {
    SPDLOG_ERROR("Failed to generate SDF");
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}