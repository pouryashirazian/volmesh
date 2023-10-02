#pragma once

#include "volmesh/volmesh.h"

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

    CellIndex insertNewCell(const vec4i& in_cell);

    bool readFromList(const std::vector<vec3>& in_vertices,
                      const std::vector<vec4i>& in_cells);

    bool writeToList(std::vector<vec3>& out_vertices,
                     std::vector<vec4i>& out_cells) const;


  };

}