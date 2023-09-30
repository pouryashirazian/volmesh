#pragma once

#include "volmesh/cell.h"
#include "volmesh/basetypes.h"
#include "volmesh/keys.h"

#include <vector>
#include <unordered_map>
#include <mutex>
#include <atomic>

namespace volmesh {

  class TetMesh {
  public:
    static const int kNumFacesPerCell = 4;
    static const int kNumEdgesPerFace = 3;

    typedef Cell<kNumFacesPerCell> TetCell;
    typedef HalfFace<kNumEdgesPerFace> TetHalfFace;

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

    uint64_t countHalfFaces() const;

    uint64_t countEdges() const;
    uint64_t countHalfEdges() const;
    uint64_t countVertices() const;

    const std::vector<TetCell> cell(const CellKey& in_cell_key) const;
    const std::vector<TetHalfFace> hface(const HalfFaceKey& in_hface_key) const;
    const HalfEdge hedge(const HalfEdgeKey& in_hedge_key) const;
    const vec3 vertex(const VertexKey& in_vertex_key) const;

  private:
    bool insertNewCell(const vec4i& in_cell);
    void insertHalfEdgeIfNotExists(const HalfEdge& in_hedge);
    void insertHalfFaceIfNotExists(const TetHalfFace& in_hface);
    void insertCellIfNotExists(const TetCell& in_cell);

  private:
    std::unordered_map<uint64_t, std::vector<TetCell>> cells_map_;
    mutable std::mutex cells_map_mutex_;
    std::atomic<uint64_t> count_hashcollided_cells_ = {0};

    std::unordered_map<uint64_t, std::vector<TetHalfFace> > hfaces_map_;
    mutable std::mutex hfaces_map_mutex_;
    std::atomic<uint64_t> count_hashcollided_hfaces_ = {0};

    std::unordered_map<uint64_t, HalfEdge> hedges_map_;
    mutable std::mutex hedges_map_mutex_;

    std::vector<real_t> vertices_;
    std::vector<real_t> normals_;
  };

}