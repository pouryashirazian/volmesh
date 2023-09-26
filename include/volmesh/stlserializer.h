#pragma once

#include <string>

namespace volmesh {

    bool ReadSTL(const std::string& input_stl_filepath, UmfTriangleMeshModelGroup& output_triangle_modelgroup);
    bool ReadAsciiSTL(const std::string& input_stl_filepath, UmfTriangleMeshModelGroup& output_triangle_modelgroup);
    bool ReadBinarySTL(const std::string& input_stl_filepath, UmfTriangleMeshModelGroup& output_triangle_modelgroup);

    bool WriteSTL(const std::string& input_stl_dirpath,
                  const UmfTriangleMeshModelGroup& input_triangle_modelgroup,
                  const ExportFileNamingPolicy& input_naming_policy = kExportFileNameIsModelName,
                  bool use_stl_binary = true);

    bool WriteAsciiSTL(const std::string& input_stl_dirpath,
                       const UmfTriangleMeshModelGroup& input_triangle_modelgroup,
                       const ExportFileNamingPolicy& input_naming_policy = kExportFileNameIsModelName);

    bool WriteBinarySTL(const std::string& input_stl_dirpath,
                        const UmfTriangleMeshModelGroup& input_triangle_modelgroup,
                        const ExportFileNamingPolicy& input_naming_policy = kExportFileNameIsModelName);

}