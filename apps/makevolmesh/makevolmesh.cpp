#include "volmesh/tetmesh.h"
#include "volmesh/sampletetmeshes.h"

#include <iostream>
#include <filesystem>
#include <fmt/core.h>

using namespace volmesh;

namespace fs = std::filesystem;

int main(int argc, const char* argv[]) {
  TetMesh mesh;
  createOneTetrahedra(mesh);

  fs::path ascii_filepath = fs::temp_directory_path() / "onetet_ascii.vtk";
  fs::path binary_filepath = fs::temp_directory_path() / "onetet_binary.vtk";

  bool result = mesh.exportToVTK(ascii_filepath.string(), false);
  if(result) {
    std::cout << fmt::format("Wrote [{}]", ascii_filepath.c_str()) << std::endl;
  }

  result = mesh.exportToVTK(binary_filepath, true);
  if(result) {
    std::cout << fmt::format("Wrote [{}]", binary_filepath.c_str()) << std::endl;
  }

  return 0;
}