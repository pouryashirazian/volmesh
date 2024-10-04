//-----------------------------------------------------------------------------
// Copyright (c) Pourya Shirazian
// All rights reserved.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.
//-----------------------------------------------------------------------------

#pragma once

#include <string>
namespace volmesh {

    class TriangleMesh;

    bool ReadSTL(const std::string& in_mesh_filepath,
                 TriangleMesh& out_triangle_mesh);

    bool ReadAsciiSTL(const std::string& in_mesh_filepath,
                      TriangleMesh& out_triangle_mesh);

    bool ReadBinarySTL(const std::string& in_mesh_filepath,
                       TriangleMesh& out_triangle_mesh);

    bool WriteBinarySTL(const std::string& in_mesh_filepath,
                        const TriangleMesh& in_triangle_mesh);
}