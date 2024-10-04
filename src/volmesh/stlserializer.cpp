//-----------------------------------------------------------------------------
// Copyright (c) Pourya Shirazian
// All rights reserved.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.
//-----------------------------------------------------------------------------

#include "volmesh/stlserializer.h"
#include "volmesh/trianglemesh.h"
#include "volmesh/basetypes.h"
#include "volmesh/logger.h"

#include <fstream>
#include <filesystem>

namespace volmesh {

  std::string ToLowerCase(std::string s) {
    std::transform(s.begin(), s.end(), s.begin(),
                   [](unsigned char c){ return std::tolower(c); }
                  );
    return s;
  }

  bool IsValidSTLFile(const std::string& in_mesh_filepath) {
    std::filesystem::path filepath(in_mesh_filepath);
    std::string extension = ToLowerCase(filepath.extension());
    bool result = std::filesystem::exists(in_mesh_filepath) &&
                  std::filesystem::is_regular_file(in_mesh_filepath) &&
                  extension == std::string(".stl");
    return result;
  }

  bool ReadSTL(const std::string& in_mesh_filepath,
               TriangleMesh& out_triangle_mesh)
  {
    if(IsValidSTLFile(in_mesh_filepath) == false) {
      SPDLOG_ERROR("The supplied mesh file [{}] is invalid.", in_mesh_filepath.c_str());
      return false;
    }

    //open the file
    std::ifstream stlfile(in_mesh_filepath, std::ios::binary);
    if (stlfile.is_open() == false) {
      return false;
    }

    //read first 5
    char ascii_header[5];
    stlfile.read(ascii_header, 5);

    if (strncmp(ascii_header, "solid", 5) == 0)
    {
      // ASCII file
      stlfile.close();
      return ReadAsciiSTL(in_mesh_filepath, out_triangle_mesh);
    }
    else
    {
      // Binary file
      stlfile.close();
      return ReadBinarySTL(in_mesh_filepath, out_triangle_mesh);
    }
  }

  bool ReadAsciiSTL(const std::string& in_mesh_filepath,
                    TriangleMesh& out_triangle_mesh)
  {
    if(IsValidSTLFile(in_mesh_filepath) == false) {
      SPDLOG_ERROR("The supplied mesh file [{}] is invalid.", in_mesh_filepath.c_str());
      return false;
    }

    //open the file
    std::ifstream stlfile(in_mesh_filepath, std::ios::in);
    if (stlfile.is_open() == false) {
      return false;
    }

    //read first 5
    char ascii_header[5];
    stlfile.read(ascii_header, 5);

    if (strncmp(ascii_header, "solid", 5) != 0)
    {
      SPDLOG_ERROR("The supplied mesh file [{}] is not an ASCII STL.", in_mesh_filepath.c_str());
      stlfile.close();
      return false;
    }

    uint32_t triangles_count = 0;
    std::string line;
    std::string facet, normal, vertex;
    //local vars
    vec3 pos[3];
    vec3 norm;

    //count number of triangles (facets) appear in the file
    std::ifstream::pos_type filePos = stlfile.tellg();
    facet = "facet";
    std::string temp;
    while (stlfile >> temp) {
      if (temp == facet) {
        triangles_count ++;
      }
    }
    stlfile.seekg(filePos);

    std::vector<vec3> vertices;
    std::vector<vec3> per_face_normals;
    vertices.reserve(triangles_count * 3);
    per_face_normals.reserve(triangles_count);

    // Read until reaching endsolid
    std::getline(stlfile, line); // skip the first line
    std::getline(stlfile, line); // facet normal n0 n1 n2
    std::istringstream iss(line);
    iss >> facet >> normal >> norm.x() >> norm.y() >> norm.z();
    while (facet == "facet")
    {
      std::getline(stlfile, line); // outer loop
      std::getline(stlfile, line); // vertex v0 v1 v2
      std::istringstream iss0(line);
      iss0 >> vertex >> pos[0].x() >> pos[0].y() >> pos[0].z();

      std::getline(stlfile, line); // vertex v0 v1 v2
      std::istringstream iss1(line);
      iss1 >> vertex >> pos[1].x() >> pos[1].y() >> pos[1].z();

      std::getline(stlfile, line); // vertex v0 v1 v2
      std::istringstream iss2(line);
      iss2 >> vertex >> pos[2].x() >> pos[2].y() >> pos[2].z();

      std::getline(stlfile, line); // endloop
      std::getline(stlfile, line); // endfacet

      vertices.push_back(pos[0]);
      vertices.push_back(pos[1]);
      vertices.push_back(pos[2]);
      per_face_normals.push_back(norm);

      std::getline(stlfile, line); // facet normal n0 n1 n2

      std::istringstream iss(line);
      iss >> facet >> normal >> norm.x() >> norm.y() >> norm.z();
    }

    stlfile.close();

    return out_triangle_mesh.readFromList(vertices, per_face_normals);
  }

  bool ReadBinarySTL(const std::string& in_mesh_filepath,
                     TriangleMesh& out_triangle_mesh)
  {
    if(IsValidSTLFile(in_mesh_filepath) == false) {
      SPDLOG_ERROR("The supplied mesh file [{}] is invalid.", in_mesh_filepath.c_str());
      return false;
    }

    //open the file
    std::ifstream stlfile(in_mesh_filepath, std::ios::binary);
    if(stlfile.is_open() == false) {
      return false;
    }

    //read first 5
    char ascii_header[5];
    stlfile.read(ascii_header, 5);

    if (strncmp(ascii_header, "solid", 5) == 0) {
      SPDLOG_ERROR("The supplied file [{}] is not a binary STL. Use ReadAsciiSTL instead.", in_mesh_filepath.c_str());
      stlfile.close();
      return false;
    }

    //read rest in binary
    char restof_header[75];
    stlfile.read(restof_header, 75);

    uint32_t triangles_count = 0;
    stlfile.read((char *)&triangles_count, sizeof(triangles_count));

    std::vector<vec3> vertices(triangles_count * 3);
    std::vector<vec3> per_face_normals(triangles_count);

    //reading all triangles
    float pos[9];
    float norm[3];
    uint16_t attr = 0;

    for(uint32_t i = 0; i < triangles_count; i++) {
      //read normal
      stlfile.read((char *)norm, sizeof(float) * 3);
      stlfile.read((char *)pos, sizeof(float) * 9);
      stlfile.read((char *)&attr, sizeof(uint16_t));

      vertices[i * 3] = vec3(pos[0], pos[1], pos[2]);
      vertices[i * 3 + 1] = vec3(pos[3], pos[4], pos[5]);
      vertices[i * 3 + 2] = vec3(pos[6], pos[7], pos[8]);

      per_face_normals[i] = vec3(norm[0], norm[1], norm[2]);
    }

    stlfile.close();

    return out_triangle_mesh.readFromList(vertices, per_face_normals);
  }

  bool WriteBinarySTL(const std::string& in_mesh_filepath,
                      const TriangleMesh& in_triangle_mesh) {
    //check the triangle mesh
    if(in_triangle_mesh.countFaces() == 0) {
      SPDLOG_ERROR("The supplied triangle mesh has zero faces");
      return false;
    }

    //show a warning message if another file exists in the same location
    if(std::filesystem::exists(std::filesystem::path(in_mesh_filepath)) == true) {
      SPDLOG_WARN("Another file with the same name already exists under [{}]", in_mesh_filepath.c_str());
    }

    //open the file
    std::ofstream stlfile(in_mesh_filepath, std::ios::binary);
    if(stlfile.is_open() == false) {
      return false;
    }

    //write header
    char header[80];
    static const std::string header_msg = "volmesh";
    strncpy(header, header_msg.data(), header_msg.length());
    stlfile.write(header, 80);

    uint32_t triangles_count = in_triangle_mesh.countFaces();
    stlfile.write((char *)&triangles_count, sizeof(triangles_count));

    std::vector<vec3> vertices(triangles_count * 3);
    std::vector<vec3> per_face_normals(triangles_count);

    //reading all triangles
    float postions_float32[9];
    float normal_float32[3];
    uint16_t attr = 0;

    for(uint32_t i = 0; i < triangles_count; i++) {
      const HalfFaceIndex hface_id = HalfFaceIndex::create(i);

      std::array<vec3, 3> face_vertices = in_triangle_mesh.halfFaceVertices(hface_id);
      const vec3 a = face_vertices[0];
      const vec3 b = face_vertices[1];
      const vec3 c = face_vertices[2];

      vec3 normal({0.0, 0.0, 1.0});
      if(in_triangle_mesh.hasHalfFaceNormals() == true) {
        normal = in_triangle_mesh.halfFaceNormal(hface_id);
      } else {
        normal = (b - a).cross(c - a).normalized();
      }

      postions_float32[0] = static_cast<float>(a[0]);
      postions_float32[1] = static_cast<float>(a[1]);
      postions_float32[2] = static_cast<float>(a[2]);
      postions_float32[3] = static_cast<float>(b[0]);
      postions_float32[4] = static_cast<float>(b[1]);
      postions_float32[5] = static_cast<float>(b[2]);
      postions_float32[6] = static_cast<float>(c[0]);
      postions_float32[7] = static_cast<float>(c[1]);
      postions_float32[8] = static_cast<float>(c[2]);

      normal_float32[0] = static_cast<float>(normal[0]);
      normal_float32[1] = static_cast<float>(normal[1]);
      normal_float32[2] = static_cast<float>(normal[2]);

      //write attributes
      stlfile.write((char *)normal_float32, sizeof(float) * 3);
      stlfile.write((char *)postions_float32, sizeof(float) * 9);
      stlfile.write((char *)&attr, sizeof(uint16_t));
    }

    stlfile.close();

    return true;
  }

}

