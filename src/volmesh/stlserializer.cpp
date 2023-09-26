//
// Project libunifiedmesh: Unified Anisotropic Volume and Surface Mesh
//
// Created on   : Aug 24, 2017
// Last Updated : Jan 11, 2018
// Author       : Pourya Shirazian <pourya.shirazian@intusurg.com>
//
//----------------------------------------------------------------------------------
//

#include "umf/serializestl.h"

#include <iostream>
#include <fstream>
#include <cstring>
#include <iomanip>
#include <boost/filesystem.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/foreach.hpp>
#include "plog/plog.h"

using namespace umf;
using namespace umf::linalg;
using namespace umf::serializer;

#define UMF_STL_FORMAT(num) std::setw(13) << std::setfill(' ') << num

int umf::serializer::ReadMultipleSTLFiles(std::vector<std::string>& input_stl_filepaths, Scene& output_scene)
{
  //sort file names
  sort(input_stl_filepaths.begin(), input_stl_filepaths.end());

  //print file names after sorting
  PLOG_TRACE("Order of stl files to be processed");
  int order = 0;
  for (auto s : input_stl_filepaths)
  {
    PSLOG_TRACE("[{}] {}", order, s.c_str());
    order++;
  }  

  //count all entities
  U64 count_faces = 0;
  U64 count_edges = 0;
  U64 count_nodes = 0;

  std::shared_ptr<UmfTriangleMeshModelGroup> input_triangle_modelgroup = output_scene.GetModelGroup<UmfTriangleMeshModelGroup>();
  input_triangle_modelgroup->Setup(input_stl_filepaths.size());
  int active_model = 0;
  for (auto stlfile : input_stl_filepaths)
  {
    input_triangle_modelgroup->SetActiveModel(active_model);
    std::string extension = boost::filesystem::path(stlfile).extension().string();
    boost::to_upper(extension);
    if (extension == std::string(".STL"))
    {
      if (ReadSTL(stlfile, *input_triangle_modelgroup) == false)
      {
        PSLOG_ERROR("I was unable to read the stl file [{}]", stlfile.c_str());
        return active_model;
      }
    }

    //Get stats
    count_faces += input_triangle_modelgroup->GetActiveModel()->GetTopologyStore()->CountFaces();
    count_edges += input_triangle_modelgroup->GetActiveModel()->GetTopologyStore()->CountEdges();
    count_nodes += input_triangle_modelgroup->GetActiveModel()->GetTopologyStore()->CountNodes();
    active_model++;
  }

  PSLOG_TRACE("UMF TriangleMesh model group now has: Face# {}, Edge# {}, Node# {}", count_faces, count_edges, count_nodes);
  return (int)input_stl_filepaths.size();
}

int umf::serializer::ReadMultipleSTLFilesFromDirPath(const std::string& input_stl_filepath_or_dirpath, Scene& output_scene)
{
  //if is a single file then find what format it is in
  if (boost::filesystem::is_regular_file(input_stl_filepath_or_dirpath))
  {
    boost::filesystem::path input_stl_filepath(input_stl_filepath_or_dirpath);
    std::string extension = "";
    if (input_stl_filepath.has_extension())
    {
      extension = input_stl_filepath.extension().string();
      boost::to_upper(extension);
    }
    else {
      return 0;
    }

    PSLOG_WARNING("Input path [{}] is not a directory.", input_stl_filepath_or_dirpath.c_str());
    if (extension == std::string(".STL"))
    {
      PSLOG_INFO("Attempt to read a single stl file from [{}]", input_stl_filepath_or_dirpath.c_str());
      std::shared_ptr<UmfTriangleMeshModelGroup> input_triangle_modelgroup = output_scene.GetModelGroup<UmfTriangleMeshModelGroup>();
      if (ReadSTL(input_stl_filepath_or_dirpath, *input_triangle_modelgroup)) {
        PSLOG_INFO("One mesh file read from the provided file-path: [{}]", input_stl_filepath_or_dirpath.c_str());
        return 1;
      }
      else {
        return 0;
      }
    }
    else {
      PSLOG_ERROR("No acceptable mesh files found in the supplied directory. [{}]", input_stl_filepath_or_dirpath.c_str());
      return 0;
    }
  }

  //Find all files in the provided directory
  std::vector<std::string> stl_filepaths_list;
  boost::filesystem::path path(input_stl_filepath_or_dirpath);
  if (boost::filesystem::is_directory(path) && boost::filesystem::exists(path))
  {
    for (auto &fp : boost::filesystem::directory_iterator(path))
    {
      boost::filesystem::path input_stl_filepath(fp);
      if (boost::filesystem::is_regular_file(input_stl_filepath))
      {
        if (input_stl_filepath.has_extension())
        {
          std::string extension = input_stl_filepath.extension().string();
          boost::to_upper(extension);
          if (extension == std::string(".STL"))
          {
            stl_filepaths_list.push_back(input_stl_filepath.string());
          }
        }
      }
    }
  }

  //use the version of the function that works with the array of file paths
  return umf::serializer::ReadMultipleSTLFiles(stl_filepaths_list, output_scene);
}


bool umf::serializer::ReadSTL(const std::string& input_stl_filepath, UmfTriangleMeshModelGroup& output_triangle_modelgroup)
{
  if (boost::filesystem::exists(input_stl_filepath) == false)
  {
    PSLOG_ERROR("Input input_stl_filepath [{}] does not exist.", input_stl_filepath.c_str());
    return false;
  }

  if (boost::filesystem::is_regular_file(input_stl_filepath) == false)
  {
    PSLOG_ERROR("Input input_stl_filepath [{}] is not a regular file.", input_stl_filepath.c_str());
    return false;
  }

  //open the file
  std::ifstream stlfile(input_stl_filepath, std::ios::binary);
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
    return ReadAsciiSTL(input_stl_filepath, output_triangle_modelgroup);
  }
  else
  {
    // Binary file
    stlfile.close();
    return ReadBinarySTL(input_stl_filepath, output_triangle_modelgroup);
  }
}

bool umf::serializer::ReadAsciiSTL(const std::string &input_stl_filepath, UmfTriangleMeshModelGroup &output_triangle_modelgroup)
{
  if (!boost::filesystem::exists(input_stl_filepath))
  {
    PSLOG_ERROR("Input input_stl_filepath [{}] does not exist.", input_stl_filepath.c_str());
    return false;
  }

  //open the file
  std::ifstream stlfile(input_stl_filepath, std::ios::in);
  if (!stlfile.is_open())
  {
    return false;
  }

  //read first 5
  char ascii_header[5];
  stlfile.read(ascii_header, 5);

  if (strncmp(ascii_header, "solid", 5) != 0)
  {
    PLOG_ERROR("Not an ASCII STL");
    stlfile.close();
    return false;
  }

  U32 count_triangles = 0;
  std::string line;
  std::string facet, normal, vertex;
  //local vars
  float pos[9];
  float norm[3];

  //pos and normals
  std::vector<float> positions;
  std::vector<float> normals;
  // ReadImageFile until reaching endsolid
  std::getline(stlfile, line); // skip the first line
  std::getline(stlfile, line); // facet normal n0 n1 n2
  std::istringstream iss(line);
  iss >> facet >> normal >> norm[0] >> norm[1] >> norm[2];
  while (facet == "facet")
  {
    std::getline(stlfile, line); // outer loop
    std::getline(stlfile, line); // vertex v0 v1 v2
    std::istringstream iss0(line);
    iss0 >> vertex >> pos[0] >> pos[1] >> pos[2];
    std::getline(stlfile, line); // vertex v0 v1 v2
    std::istringstream iss1(line);
    iss1 >> vertex >> pos[3] >> pos[4] >> pos[5];
    std::getline(stlfile, line); // vertex v0 v1 v2
    std::istringstream iss2(line);
    iss2 >> vertex >> pos[6] >> pos[7] >> pos[8];
    std::getline(stlfile, line); // endloop
    std::getline(stlfile, line); // endfacet
    for (int j = 0; j < 9; j++)
    {
      positions.push_back(pos[j]);
      normals.push_back(norm[j % 3]);
    }
    ++count_triangles;
    std::getline(stlfile, line); // facet normal n0 n1 n2
    std::istringstream iss(line);
    iss >> facet >> normal >> norm[0] >> norm[1] >> norm[2];
  }

  //adding triangles
  {
    FiberCompactionLut compaction_lut;
    std::string title = boost::filesystem::path(input_stl_filepath).stem().string();
    output_triangle_modelgroup.GetActiveModel()->SetName(title);
    output_triangle_modelgroup.GetVertexStore()->Add(positions, normals, 3, compaction_lut);
    output_triangle_modelgroup.GetTopologyStore()->AddFace(compaction_lut.GetIndexArray());
    output_triangle_modelgroup.PrintActiveModelStats();
  }
  stlfile.close();  

  //verify number of triangles from the header
  bool result = true;
  if(output_triangle_modelgroup.GetTopologyStore()->CountFaces() != count_triangles) {
    PSLOG_ERROR("The number of triangles read from [{}] does not match the expected number from the header.", 
                input_stl_filepath.c_str());
    result = false;
  }

  return result;
}

bool umf::serializer::ReadBinarySTL(const std::string& input_stl_filepath, UmfTriangleMeshModelGroup& output_triangle_modelgroup)
{
  if (boost::filesystem::exists(input_stl_filepath) == false)
  {
    PSLOG_ERROR("Input input_stl_filepath [{}] does not exist.", input_stl_filepath.c_str());
    return false;
  }

  //open the file
  std::ifstream stlfile(input_stl_filepath, std::ios::binary);
  if (stlfile.is_open() == false)
  {
    return false;
  }

  //read first 5
  char ascii_header[5];
  stlfile.read(ascii_header, 5);

  if (strncmp(ascii_header, "solid", 5) == 0)
  {
    PLOG_ERROR("This is not a binary STL. Retry with the Ascii version of this function");
    stlfile.close();
    return false;
  }

  //read rest in binary
  char restof_header[75];
  stlfile.read(restof_header, 75);

  U32 count_triangles = 0;
  stlfile.read((char *)&count_triangles, sizeof(count_triangles));

  //local vars
  float pos[9];
  float norm[3];
  U16 attr = 0;

  //pos and normals
  std::vector<float> positions;
  std::vector<float> normals;

  //each triangle has 3 nodes and each node is 3 floats
  positions.resize(count_triangles * 9);
  normals.resize(count_triangles * 9);
  PSLOG_TRACE("RAW STL file stats: Faces= {}, Nodes={}", count_triangles, count_triangles * 3);

  //reading all triangles
  for (U32 triangle_index = 0; triangle_index < count_triangles; triangle_index++)
  {
    //read normal
    stlfile.read((char *)norm, sizeof(float) * 3);
    stlfile.read((char *)pos, sizeof(float) * 9);
    stlfile.read((char *)&attr, sizeof(U16));

    U32 vertex_array_linear_offset = triangle_index * 9;
    for (int j = 0; j < 9; j++)
    {
      positions[vertex_array_linear_offset + j] = pos[j];
      normals[vertex_array_linear_offset + j] = norm[j % 3];
    }
  }

  //adding triangles
  {
    FiberCompactionLut compaction_lut;

    std::string title = boost::filesystem::path(input_stl_filepath).stem().string();
    output_triangle_modelgroup.GetActiveModel()->SetName(title);
    output_triangle_modelgroup.GetVertexStore()->Add(positions, normals, 3, compaction_lut);
    output_triangle_modelgroup.GetTopologyStore()->AddFace(compaction_lut.GetIndexArray());    
  }
  stlfile.close();  

  //verify number of triangles from the header
  bool result = true;
  if(output_triangle_modelgroup.GetTopologyStore()->CountFaces() != count_triangles) {
    PSLOG_ERROR("The number of triangles read from [{}] does not match the expected number from the header.", 
                input_stl_filepath.c_str());
    result = false;
  }

  return result;
}

bool umf::serializer::WriteSTL(const std::string& input_stl_dirpath, 
                               const UmfTriangleMeshModelGroup& input_triangle_modelgroup, 
                               const ExportFileNamingPolicy& input_naming_policy,
                               bool use_stl_binary)
{
  if (use_stl_binary)
    return WriteBinarySTL(input_stl_dirpath, input_triangle_modelgroup, input_naming_policy);
  else
    return WriteAsciiSTL(input_stl_dirpath, input_triangle_modelgroup, input_naming_policy);
}

bool umf::serializer::WriteAsciiSTL(const std::string& input_stl_dirpath, 
                                    const UmfTriangleMeshModelGroup& input_triangle_modelgroup,
                                    const ExportFileNamingPolicy& input_naming_policy)
{

  //Export one model at a time
  for (U32 i = 0; i < input_triangle_modelgroup.Count(); i++)
  {
    std::shared_ptr<UmfTriangleMeshModelGroup::UmfModelType> model;
    model = input_triangle_modelgroup.GetModel(i);    
    boost::filesystem::path filestem;

    switch(input_naming_policy) {
      case(kExportFileNameIsModelName): {
        std::string model_name = model->GetName();
        if (model_name.length() == 0) {
          std::stringstream ss;
          ss << "model_" << i + 1 << "_" << input_triangle_modelgroup.Count();
          model_name = ss.str();
        }
      
        //convert all spaces to underscore
        std::transform(model_name.begin(), model_name.end(), model_name.begin(), [](char ch) {
          return (ch == ' ' || ch == '-') ? '_' : ch;
        });

        filestem = model_name;
        filestem.replace_extension(".stl"); 
        break;
      }
      case(kExportFileNameIsModelId): {
        filestem = std::to_string(i);
        filestem.replace_extension(".stl"); 
        break;
      }
    }

    //join the input_stl_dirpath and the stem to make the combined file-path
    std::string combined_path = (boost::filesystem::path(input_stl_dirpath) / filestem).string();

    //write model
    bool result = WriteSingleModelToAsciiSTL(combined_path, *model);
    if (result)
    {
      PSLOG_INFO("Model [{} of {}] exported to [{}]", 
                 i + 1, 
                 input_triangle_modelgroup.Count(),
                 filestem.string().c_str());
    }
    else
    {
      PSLOG_ERROR("Failed to export model [{} of {}] to [{}]", 
                  i + 1, 
                  input_triangle_modelgroup.Count(),
                  filestem.string().c_str());
      return false;
    }
  }

  return true;
}

//write stl binary
bool umf::serializer::WriteBinarySTL(const std::string& input_stl_dirpath, 
                                     const UmfTriangleMeshModelGroup& input_triangle_modelgroup,
                                     const ExportFileNamingPolicy& input_naming_policy)
{
  //Export one input_triangle_modelgroup at a time
  for (U32 i = 0; i < input_triangle_modelgroup.Count(); i++)
  {
    std::shared_ptr<UmfTriangleMeshModelGroup::UmfModelType> model;
    model = input_triangle_modelgroup.GetModel(i);    
    boost::filesystem::path filestem;

    switch(input_naming_policy) {
      case(kExportFileNameIsModelName): {
        std::string model_name = model->GetName();
        if (model_name.length() == 0) {
          std::stringstream ss;
          ss << "model_" << i + 1 << "_" << input_triangle_modelgroup.Count();
          model_name = ss.str();
        }
      
        //convert all spaces to underscore
        std::transform(model_name.begin(), model_name.end(), model_name.begin(), [](char ch) {
          return (ch == ' ' || ch == '-') ? '_' : ch;
        });

        filestem = model_name;
        filestem.replace_extension(".stl"); 
        break;
      }
      case(kExportFileNameIsModelId): {
        filestem = std::to_string(i);
        filestem.replace_extension(".stl"); 
        break;
      }
    }

    //join the input_stl_dirpath and the stem to make the combined input_stl_filepath
    std::string combined_path = (boost::filesystem::path(input_stl_dirpath) / filestem).string();

    //write model
    bool result = WriteSingleModelToBinarySTL(combined_path, *model);
    if (result)
    {
      PSLOG_INFO("Model [{} of {}] exported to [{}]", 
                 i + 1, 
                 input_triangle_modelgroup.Count(),
                 filestem.string().c_str());
    }
    else
    {
      PSLOG_ERROR("Failed to export model [{} of {}] to [{}]", 
                  i + 1, 
                  input_triangle_modelgroup.Count(),
                  filestem.string().c_str());
      return false;
    }
  }

  return true;
}

bool umf::serializer::WriteSingleModelToAsciiSTL(const std::string& input_stl_filepath,
                                                 const UmfTriangleMeshModelGroup::UmfModelType& input_triangle_model)
{
  std::string solid_name = "umf";
  if (input_triangle_model.GetName().length() > 0)
  {
    solid_name = input_triangle_model.GetName();
  }

  //warn if the file will be overwritten
  if (boost::filesystem::exists(input_stl_filepath))
  {
    PSLOG_WARNING("Output stl file already exists [{}]", input_stl_filepath.c_str());
  }

  //open the file
  std::ofstream stlfile(input_stl_filepath, std::ios::out);
  if (!stlfile.is_open())
  {
    PSLOG_ERROR("Unable to open the file for writing [{}]", input_stl_filepath.c_str());
    return false;
  }

  // Set format to scientific
  stlfile << std::scientific;
  // write first 5
  stlfile << "solid " << solid_name << std::endl;

  // write triangles
  U32 count_triangles = static_cast<umf::linalg::U32>(input_triangle_model.GetTopologyStore()->CountFaces());
  U32 count_vertices = static_cast<umf::linalg::U32>(input_triangle_model.GetTopologyStore()->CountNodes());
  PSLOG_INFO("UMF triangle mesh stats: faces={}, nodes={}", count_triangles, count_vertices);

  //Get the face by nodes fiber
  using FaceByNodesFiber = UmfTriangleMeshModelGroup::TopologyDataStorageType::FaceByNodesFiberType;
  std::shared_ptr<FaceByNodesFiber> facebynodes(new FaceByNodesFiber());
  if (input_triangle_model.GetTopologyStore()->ComputeFaceByNodesFiber(facebynodes) != count_triangles)
  {
    PLOG_ERROR("Conversion from edge-based faces to node-based faces has failed!");
    return false;
  }

  //writing all triangles
  using TRIANGLE = FaceByNodesFiber::LAYOUT::PACKET_TYPE;
  using POSITION = UmfTriangleMeshModelGroup::VertexDataStorageType::RestPosFiberType::LAYOUT::PACKET_TYPE;
  using NORMAL = UmfTriangleMeshModelGroup::BasicShadingDataStorageType::NormalFiberType::LAYOUT::PACKET_TYPE;
  bool has_normals = (input_triangle_model.GetShadingStore()->GetNormals()->CountPackets() == count_vertices);

  TRIANGLE tri;
  POSITION p[3];

  NORMAL per_vertex_normals[3];
  float face_normal[3];
  float face_positions[9];

  for (U32 triangle_index = 0; triangle_index < count_triangles; ++triangle_index)
  {
    tri = facebynodes->GetPacket(triangle_index);

    p[0] = input_triangle_model.GetVertexStore()->GetRestPositions()->GetPacket(tri[0]);
    p[1] = input_triangle_model.GetVertexStore()->GetRestPositions()->GetPacket(tri[1]);
    p[2] = input_triangle_model.GetVertexStore()->GetRestPositions()->GetPacket(tri[2]);

    //store positions into the array
    p[0].Store(&face_positions[0]);
    p[1].Store(&face_positions[3]);
    p[2].Store(&face_positions[6]);

    //use per vertex normals
    if (has_normals)
    {
      per_vertex_normals[0] = input_triangle_model.GetShadingStore()->GetNormals()->GetPacket(tri[0]);
      per_vertex_normals[1] = input_triangle_model.GetShadingStore()->GetNormals()->GetPacket(tri[1]);
      per_vertex_normals[2] = input_triangle_model.GetShadingStore()->GetNormals()->GetPacket(tri[2]);
      NORMAL n = per_vertex_normals[0] + per_vertex_normals[1] + per_vertex_normals[2];
      n.Normalize();
      n.Store(face_normal);
    }
    else
    {
      NORMAL n = NORMAL::Cross(p[2] - p[0], p[1] - p[0]);
      n.Normalize();
      n.Store(face_normal);
    }

    //write to file
    stlfile << "  facet normal " << UMF_STL_FORMAT(face_normal[0]) << " " << UMF_STL_FORMAT(face_normal[1]) << " " << UMF_STL_FORMAT(face_normal[2]) << std::endl;
    stlfile << "    outer loop" << std::endl;
    stlfile << "      vertex  " << UMF_STL_FORMAT(face_positions[0]) << " " << UMF_STL_FORMAT(face_positions[1]) << " " << UMF_STL_FORMAT(face_positions[2]) << std::endl;
    stlfile << "      vertex  " << UMF_STL_FORMAT(face_positions[3]) << " " << UMF_STL_FORMAT(face_positions[4]) << " " << UMF_STL_FORMAT(face_positions[5]) << std::endl;
    stlfile << "      vertex  " << UMF_STL_FORMAT(face_positions[6]) << " " << UMF_STL_FORMAT(face_positions[7]) << " " << UMF_STL_FORMAT(face_positions[8]) << std::endl;
    stlfile << "    endloop" << std::endl;
    stlfile << "  endfacet" << std::endl;
  }

  stlfile << "endsolid " << solid_name << std::endl;
  stlfile.close();  
  return true;
}

bool umf::serializer::WriteSingleModelToBinarySTL(const std::string& input_stl_filepath,
                                                  const UmfTriangleMeshModelGroup::UmfModelType& input_triangle_model)
{
  //warn if the file will be overwritten
  if (boost::filesystem::exists(input_stl_filepath))
  {
    PSLOG_WARNING("Output stl file will be overwritten [{}]", input_stl_filepath.c_str());
  }
  //open the file
  std::ofstream stlfile(input_stl_filepath, std::ios::binary);
  if (!stlfile.is_open())
  {
    PSLOG_ERROR("Unable to open the file for writing [{}]", input_stl_filepath.c_str());
    return false;
  }

  //write first 5
  char ascii_header[] = "NBUMF";
  stlfile.write((const char *)ascii_header, 5);

  //write rest in binary
  char restof_header[75];
  stlfile.write((const char *)restof_header, 75);

  U32 count_triangles = static_cast<umf::linalg::U32>(input_triangle_model.GetTopologyStore()->CountFaces());
  U32 count_vertices = static_cast<umf::linalg::U32>(input_triangle_model.GetTopologyStore()->CountNodes());
  PSLOG_INFO("UMF triangle mesh stats: faces={}, nodes={}", count_triangles, count_vertices);  

  stlfile.write((const char *)&count_triangles, sizeof(count_triangles));

  //Get the face by nodes fiber
  using FaceByNodesFiber = UmfTriangleMeshModelGroup::TopologyDataStorageType::FaceByNodesFiberType;
  std::shared_ptr<FaceByNodesFiber> facebynodes(new FaceByNodesFiber());
  if (input_triangle_model.GetTopologyStore()->ComputeFaceByNodesFiber(facebynodes) != count_triangles)
  {
    PLOG_ERROR("Failed to convert faces from edge-based representation to node-based.");
    return false;
  }

  //writing all triangles
  using TRIANGLE = FaceByNodesFiber::LAYOUT::PACKET_TYPE;
  using POSITION = UmfTriangleMeshModelGroup::VertexDataStorageType::RestPosFiberType::LAYOUT::PACKET_TYPE;
  using NORMAL = UmfTriangleMeshModelGroup::BasicShadingDataStorageType::NormalFiberType::LAYOUT::PACKET_TYPE;
  using COLOR = UmfTriangleMeshModelGroup::BasicShadingDataStorageType::ColorFiberType::LAYOUT::PACKET_TYPE;
  bool has_normals = (input_triangle_model.GetShadingStore()->GetNormals()->CountPackets() == count_vertices);
  bool has_colors = (input_triangle_model.GetShadingStore()->GetColors()->CountPackets() == count_vertices);

  TRIANGLE tri;
  POSITION p[3];

  NORMAL per_vertex_normals[3];
  float face_normal[3];
  float face_positions[9];
  U16 face_attribute;

  for (U32 triangle_index = 0; triangle_index < count_triangles; triangle_index++)
  {
    tri = facebynodes->GetPacket(triangle_index);

    p[0] = input_triangle_model.GetVertexStore()->GetRestPositions()->GetPacket(tri[0]);
    p[1] = input_triangle_model.GetVertexStore()->GetRestPositions()->GetPacket(tri[1]);
    p[2] = input_triangle_model.GetVertexStore()->GetRestPositions()->GetPacket(tri[2]);

    //store positions into the array
    p[0].Store(&face_positions[0]);
    p[1].Store(&face_positions[3]);
    p[2].Store(&face_positions[6]);

    //use per vertex normals
    if (has_normals)
    {
      per_vertex_normals[0] = input_triangle_model.GetShadingStore()->GetNormals()->GetPacket(tri[0]);
      per_vertex_normals[1] = input_triangle_model.GetShadingStore()->GetNormals()->GetPacket(tri[1]);
      per_vertex_normals[2] = input_triangle_model.GetShadingStore()->GetNormals()->GetPacket(tri[2]);
      NORMAL n = per_vertex_normals[0] + per_vertex_normals[1] + per_vertex_normals[2];
      n.Normalize();
      n.Store(face_normal);
    }
    else
    {
      NORMAL n = NORMAL::Cross(p[2] - p[0], p[1] - p[0]);
      n.Normalize();
      n.Store(face_normal);
    }

    //
    if (has_colors)
    {
      COLOR color = input_triangle_model.GetShadingStore()->GetColors()->GetPacket(tri[0]);
      Vec3<U8> rgb((U8)(color.x * 32.0f), (U8)(color.y * 32.0f), (U8)(color.z * 32.0f));

      face_attribute = rgb.x;
      face_attribute |= (U16)((rgb.y) << 5);
      face_attribute |= (U16)((rgb.z) << 10);
      face_attribute |= (U16)(1 << 15);
    }
    else {
      face_attribute = 0;
    }

    //write normal
    stlfile.write((const char *)face_normal, sizeof(float) * 3);
    stlfile.write((const char *)face_positions, sizeof(float) * 9);
    stlfile.write((const char *)&face_attribute, sizeof(U16));
  }
  stlfile.close();
  return true;
}

