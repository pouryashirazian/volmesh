#pragma once

#include "volmesh/basetypes.h"

#include <string>

namespace volmesh {

    bool ReadSTL(const std::string& in_mesh_filepath,
                 std::vector<vec3>& out_vertices,
                 std::vector<vec3>& out_per_face_normals);

    bool ReadAsciiSTL(const std::string& in_mesh_filepath,
                      std::vector<vec3>& out_vertices,
                      std::vector<vec3>& out_per_face_normals);

    bool ReadBinarySTL(const std::string& in_mesh_filepath,
                       std::vector<vec3>& out_vertices,
                       std::vector<vec3>& out_per_face_normals);

}