#include "volmesh/stlserializer.h"
#include "volmesh/trianglemesh.h"
#include "volmesh/basetypes.h"

#include <iostream>
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
      std::cerr << "The supplied mesh file [" << in_mesh_filepath << "] is invalid." << std::endl;
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
      std::cerr << "The supplied mesh file [" << in_mesh_filepath << "] is invalid." << std::endl;
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
      std::cerr << "File [" << in_mesh_filepath << " it NOT an ASCII STL" << std::endl;
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
    return true;
  }

  bool ReadBinarySTL(const std::string& in_mesh_filepath,
                     TriangleMesh& out_triangle_mesh)
  {
    if(IsValidSTLFile(in_mesh_filepath) == false) {
      std::cerr << "The supplied mesh file [" << in_mesh_filepath << "] is invalid." << std::endl;
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
      std::cerr << "This is not a binary STL. Retry with the Ascii version of this function" << std::endl;
      stlfile.close();
      return false;
    }

    //read rest in binary
    char restof_header[75];
    stlfile.read(restof_header, 75);

    uint32_t triangles_count = 0;
    stlfile.read((char *)&triangles_count, sizeof(triangles_count));

    out_vertices.resize(triangles_count * 3);
    out_per_face_normals.resize(triangles_count);

    //reading all triangles
    float pos[9];
    float norm[3];
    uint16_t attr = 0;

    for(uint32_t i = 0; i < triangles_count; i++) {
      //read normal
      stlfile.read((char *)norm, sizeof(float) * 3);
      stlfile.read((char *)pos, sizeof(float) * 9);
      stlfile.read((char *)&attr, sizeof(uint16_t));

      out_vertices[i * 3] = vec3(pos[0], pos[1], pos[2]);
      out_vertices[i * 3 + 1] = vec3(pos[3], pos[4], pos[5]);
      out_vertices[i * 3 + 2] = vec3(pos[6], pos[7], pos[8]);

      out_per_face_normals[i] = vec3(norm[0], norm[1], norm[2]);
    }

    stlfile.close();

    return true;
  }
}