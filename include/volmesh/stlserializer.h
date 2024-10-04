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

/**
 * @brief Reads an STL (Stereolithography) file and populates a triangle mesh.
 *
 * This function reads an STL file from the specified file path and stores the resulting
 * triangle mesh data in the provided `TriangleMesh` object. It automatically detects
 * whether the file is in ASCII or binary format.
 *
 * @param in_mesh_filepath The file path of the input STL file.
 * @param out_triangle_mesh The `TriangleMesh` object to store the read mesh data.
 * @return True if the STL file was successfully read, false otherwise.
 */
bool ReadSTL(const std::string& in_mesh_filepath, TriangleMesh& out_triangle_mesh);

/**
 * @brief Reads an ASCII STL file and populates a triangle mesh.
 *
 * This function reads an STL file in ASCII format from the specified file path and
 * stores the resulting triangle mesh data in the provided `TriangleMesh` object.
 *
 * @param in_mesh_filepath The file path of the input ASCII STL file.
 * @param out_triangle_mesh The `TriangleMesh` object to store the read mesh data.
 * @return True if the ASCII STL file was successfully read, false otherwise.
 */
bool ReadAsciiSTL(const std::string& in_mesh_filepath, TriangleMesh& out_triangle_mesh);

/**
 * @brief Reads a binary STL file and populates a triangle mesh.
 *
 * This function reads an STL file in binary format from the specified file path and
 * stores the resulting triangle mesh data in the provided `TriangleMesh` object.
 *
 * @param in_mesh_filepath The file path of the input binary STL file.
 * @param out_triangle_mesh The `TriangleMesh` object to store the read mesh data.
 * @return True if the binary STL file was successfully read, false otherwise.
 */
bool ReadBinarySTL(const std::string& in_mesh_filepath, TriangleMesh& out_triangle_mesh);

/**
 * @brief Writes a triangle mesh to a binary STL file.
 *
 * This function writes the mesh data from the provided `TriangleMesh` object to a
 * binary STL file at the specified file path.
 *
 * @param in_mesh_filepath The file path where the output binary STL file will be written.
 * @param in_triangle_mesh The `TriangleMesh` object containing the mesh data to write.
 * @return True if the binary STL file was successfully written, false otherwise.
 */
bool WriteBinarySTL(const std::string& in_mesh_filepath, const TriangleMesh& in_triangle_mesh);

}