#include "volmesh/tetmesh.h"
#include "volmesh/sampletetmeshes.h"

#include <iostream>
#include <filesystem>
#include <fmt/core.h>
#include <cxxopts.hpp>
#include <spdlog/spdlog.h>

using namespace volmesh;

namespace fs = std::filesystem;

int main(int argc, const char* argv[]) {
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
  spdlog::info("input surface mesh at [{}]", surface_mesh_filepath.c_str());
}