//-----------------------------------------------------------------------------
// Copyright (c) Pourya Shirazian
// All rights reserved.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.
//-----------------------------------------------------------------------------

#pragma once

#include "volmesh/volmesh.h"
#include "volmesh/voxel.h"
#include "volmesh/trianglemesh.h"

namespace volmesh {

  static const int kTetMeshNumFacesPerCell = 4;
  static const int kTetMeshNumEdgesPerFace = 3;

/**
 * @brief Layout struct for a tetrahedral mesh.
 *
 * Defines the basic layout for a tetrahedral mesh, specifying the types for
 * cells and half-faces, and the number of vertices per cell.
 *
 * @tparam kNumFacesPerCell Number of faces per cell in the mesh (default is 4).
 * @tparam kNumEdgesPerFace Number of edges per face in the mesh (default is 3).
 */
template <int kNumFacesPerCell = kTetMeshNumFacesPerCell,
          int kNumEdgesPerFace = kTetMeshNumEdgesPerFace>
struct TetMeshLayout {
  /// Type definition for a cell in the tetrahedral mesh.
  typedef Cell<kNumFacesPerCell> CellType;

  /// Type definition for a half-face in the tetrahedral mesh.
  typedef HalfFace<kNumEdgesPerFace> HalfFaceType;

  /// Number of vertices per cell in the tetrahedral mesh.
  static const int kNumVerticesPerCell = (kNumFacesPerCell * kNumEdgesPerFace) / 3;
};

/**
 * @brief Class representing a tetrahedral mesh.
 *
 * The TetMesh class represents a volumetric mesh consisting of tetrahedra.
 * It extends the VolMesh base class and provides methods for inserting
 * tetrahedra, extracting boundary triangle meshes, and reading/writing the
 * mesh from/to lists.
 */
class TetMesh: public VolMesh<kTetMeshNumFacesPerCell, kTetMeshNumEdgesPerFace, TetMeshLayout> {
public:
  /**
   * @brief Default constructor for TetMesh.
   *
   * Initializes an empty tetrahedral mesh.
   */
  TetMesh();

  /**
   * @brief Constructs a TetMesh from a list of vertices and tetrahedral cells.
   *
   * @param in_vertices List of vertex positions.
   * @param in_tet_cells_by_vertex_ids List of tetrahedral cells, each defined by four vertex indices.
   */
  explicit TetMesh(const std::vector<vec3>& in_vertices,
                   const std::vector<vec4i>& in_tet_cells_by_vertex_ids);

  /**
   * @brief Destructor for TetMesh.
   */
  virtual ~TetMesh();

  /**
   * @brief Inserts a tetrahedron into the mesh.
   *
   * @param in_tet_vertex_ids A vec4i containing the vertex IDs of the tetrahedron to insert.
   * @return The CellIndex of the newly inserted tetrahedron.
   */
  CellIndex insertTetrahedra(const vec4i& in_tet_vertex_ids);

  /**
   * @brief Inserts a voxel into the mesh.
   *
   * Inserts a voxel (a cubical or hexahedral cell) into the mesh and splits
   * it into tetrahedra. The vertex IDs of the voxel are provided, and the
   * resulting tetrahedral cell IDs are returned.
   *
   * @param in_voxel_vertex_ids An array containing the vertex IDs of the voxel.
   * @param out_tet_cell_ids An array to store the IDs of the resulting tetrahedra.
   * @return True if the voxel was successfully inserted, false otherwise.
   */
  bool insertVoxel(const std::array<int, Voxel::kNumVerticesPerCell>& in_voxel_vertex_ids,
                   std::array<CellIndex, Voxel::kNumFittingTetrahedra>& out_tet_cell_ids);

  /**
   * @brief Extracts the boundary triangle mesh from the tetrahedral mesh.
   *
   * @param out_triangle_mesh A reference to the output TriangleMesh where the boundary triangles will be stored.
   * @return True if the boundary triangle mesh was successfully extracted, false otherwise.
   */
  bool extractBoundaryTriangleMesh(volmesh::TriangleMesh& out_triangle_mesh) const;

  /**
   * @brief Reads a tetrahedral mesh from a list of vertices and tetrahedral cells.
   *
   * @param in_vertices List of vertex positions.
   * @param in_tet_cells_by_vertex_ids List of tetrahedral cells, each defined by four vertex indices.
   * @return True if the mesh was successfully read, false otherwise.
   */
  bool readFromList(const std::vector<vec3>& in_vertices,
                    const std::vector<vec4i>& in_tet_cells_by_vertex_ids);

  /**
   * @brief Writes the tetrahedral mesh to a list of vertices and tetrahedral cells.
   *
   * @param out_vertices List to store vertex positions.
   * @param out_tet_cells_by_vertex_ids List to store tetrahedral cells, each defined by four vertex indices.
   * @return True if the mesh was successfully written, false otherwise.
   */
  bool writeToList(std::vector<vec3>& out_vertices,
                   std::vector<vec4i>& out_tet_cells_by_vertex_ids) const;

  /**
   * @brief Exports the tetrahedral mesh to a VTK file.
   *
   * @param filepath The path where the VTK file will be saved.
   * @param is_binary Flag to determine whether the VTK file should be in binary format.
   * @return True if the mesh was successfully exported, false otherwise.
   */
  bool exportToVTK(const std::string& filepath, const bool is_binary);
};

}