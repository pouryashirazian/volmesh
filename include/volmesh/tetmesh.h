#pragma once

#include "volmesh/volmesh.h"
#include "volmesh/cube.h"

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
                     const std::vector<vec4i>& in_cells);
    virtual ~TetMesh();

    CellIndex insertTetrahedra(const vec4i& in_tetrahedra_cell);

    bool insertCube(const std::array<int, Cube::kNumVerticesPerCell>& in_cube_cell,
                    std::array<CellIndex, Cube::kNumFittingTetrahedra>& out_tet_cell_indices);

    bool readFromList(const std::vector<vec3>& in_vertices,
                      const std::vector<vec4i>& in_cells);

    bool writeToList(std::vector<vec3>& out_vertices,
                     std::vector<vec4i>& out_cells) const;


  };

}