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

  template <int kNumFacesPerCell = kTetMeshNumFacesPerCell,
            int kNumEdgesPerFace = kTetMeshNumEdgesPerFace>
  struct TetMeshLayout {
    typedef Cell<kNumFacesPerCell> CellType;
    typedef HalfFace<kNumEdgesPerFace> HalfFaceType;
    static const int kNumVerticesPerCell = (kNumFacesPerCell * kNumEdgesPerFace) / 3;
  };

  class TetMesh: public VolMesh<kTetMeshNumFacesPerCell, kTetMeshNumEdgesPerFace, TetMeshLayout> {
  public:
    TetMesh();
    explicit TetMesh(const std::vector<vec3>& in_vertices,
                     const std::vector<vec4i>& in_tet_cells_by_vertex_ids);
    virtual ~TetMesh();

    CellIndex insertTetrahedra(const vec4i& in_tet_vertex_ids);

    bool insertVoxel(const std::array<int, Voxel::kNumVerticesPerCell>& in_voxel_vertex_ids,
                     std::array<CellIndex, Voxel::kNumFittingTetrahedra>& out_tet_cell_ids);

    bool extractBoundaryTriangleMesh(volmesh::TriangleMesh& out_triangle_mesh) const;

    bool readFromList(const std::vector<vec3>& in_vertices,
                      const std::vector<vec4i>& in_tet_cells_by_vertex_ids);

    bool writeToList(std::vector<vec3>& out_vertices,
                     std::vector<vec4i>& out_tet_cells_by_vertex_ids) const;

    bool exportToVTK(const std::string& filepath, const bool is_binary);
  };

}