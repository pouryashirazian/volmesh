#pragma once

#include "volmesh/volmesh.h"
#include "volmesh/voxel.h"

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

    bool readFromList(const std::vector<vec3>& in_vertices,
                      const std::vector<vec4i>& in_tet_cells_by_vertex_ids);

    bool writeToList(std::vector<vec3>& out_vertices,
                     std::vector<vec4i>& out_tet_cells_by_vertex_ids) const;

    bool exportToVTK(const std::string& filepath, const bool is_binary);
  };

}