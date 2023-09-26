#pragma once

#include "volmesh/cell.h"
#include "volmesh/basetypes.h"

#include <vector>

namespace volmesh {

  class TetMesh {
  public:
    static const int kNumFacesPerCell = 4;
    static const int kNumEdgesPerFace = 3;

    TetMesh();
    explicit TetMesh(const TetMesh& rhs);
    ~TetMesh();

    void copyFrom(const TetMesh& rhs);
    void clear();

    uint64_t countCells() const;
    uint64_t countFaces() const;
    uint64_t countEdges() const;
    uint64_t countVertices() const;

  private:
    std::vector<Cell<kNumFacesPerCell>> cells_;
    std::vector<HalfFace<kNumEdgesPerFace>> faces_;
    std::vector<HalfEdge> edges_;
    std::vector<real_t> vertices_;
    std::vector<real_t> normals_;
  };

}