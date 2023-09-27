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
    explicit TetMesh(const std::vector<vec3>& in_vertices,
                     const std::vector<vec4i>& in_cells);
    ~TetMesh();

    void copyFrom(const TetMesh& rhs);
    void clear();

    bool readFromList(const std::vector<vec3>& in_vertices,
                      const std::vector<vec4i>& in_cells);

    bool writeToList(std::vector<vec3>& out_vertices,
                     std::vector<vec4i>& out_cells) const;


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