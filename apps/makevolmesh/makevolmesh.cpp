#include "volmesh/logger.h"
#include "volmesh/tetmesh.h"
#include "volmesh/sampletetmeshes.h"
#include "volmesh/stlserializer.h"

#include <iostream>
#include <filesystem>
#include <fmt/core.h>
#include <cxxopts.hpp>

using namespace volmesh;

namespace fs = std::filesystem;

int main(int argc, const char* argv[]) {
  SetLogFormat();

  cxxopts::Options options("makevolmesh", "Generate volumetric meshes from 3D surface meshes.");

  options.add_options()
    ("i,input", "Input surface mesh", cxxopts::value<std::string>())
    ("o,output", "Output volumetric mesh", cxxopts::value<std::string>())
    ("s,sdf", "Save intermediate SDF", cxxopts::value<bool>()->default_value("false"))
    ("h,help", "Print usage")
  ;

  auto result = options.parse(argc, argv);

  if (result.count("help")) {
    std::cout << options.help() << std::endl;
    exit(0);
  }

  std::string surface_mesh_filepath = result["input"].as<std::string>();
  SPDLOG_INFO("input surface mesh at [{}]", surface_mesh_filepath.c_str());

  TetMesh tet_mesh;
  createOneTetrahedra(tet_mesh);
  // createSphericalShell(tet_mesh);
  tet_mesh.exportToVTK("/tmp/shell.vtk", false);

  TriangleMesh tri_mesh;
  tet_mesh.extractBoundaryTriangleMesh(tri_mesh);

  WriteBinarySTL("/tmp/shell.stl", tri_mesh);

  tri_mesh.computePerEdgePseudoNormals();

  return 0;
}