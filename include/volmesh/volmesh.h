#pragma once

#include "volmesh/cell.h"
#include "volmesh/basetypes.h"
#include "volmesh/index.h"

#include <vector>
#include <unordered_map>
#include <mutex>

namespace volmesh {

  template <int kNumFacesPerCell, int kNumEdgesPerFace> struct VolMeshLayout {
    typedef Cell<kNumFacesPerCell> CellType;
    typedef HalfFace<kNumEdgesPerFace> HalfFaceType;
    static const int kNumVerticesPerCell = (kNumFacesPerCell * kNumEdgesPerFace) / 3;
  };

  template <int kNumFacesPerCell,
            int kNumEdgesPerFace,
            template <int NumFacesPerCell, int NumEdgesPerFace> class LayoutPolicy = VolMeshLayout>
  class VolMesh {
  public:
    using Layout = VolMeshLayout<kNumFacesPerCell, kNumEdgesPerFace>;
    using CellType = typename Layout::CellType;
    using HalfFaceType = typename Layout::HalfFaceType;

    VolMesh();
    explicit VolMesh(const VolMesh& rhs);
    virtual ~VolMesh();

    void copyFrom(const VolMesh& rhs);
    void clear();


    uint32_t countCells() const;
    uint32_t countHalfFaces() const;
    uint32_t countEdges() const;
    uint32_t countHalfEdges() const;
    uint32_t countVertices() const;

    const CellType& cell(const CellIndex& in_cell_id) const;
    const HalfFaceType& halfFace(const HalfFaceIndex& in_hface_id) const;
    const HalfEdge& halfEdge(const HalfEdgeIndex& in_hedge_id) const;
    const vec3 vertex(const VertexIndex& in_vertex_id) const;

    bool insertAllVertices(const std::vector<vec3>& in_vertices);

    HalfEdgeIndex insertHalfEdgeIfNotExists(const HalfEdge& in_hedge);
    HalfFaceIndex insertHalfFaceIfNotExists(const HalfFaceType& in_hface);
    CellIndex insertCellIfNotExists(const CellType& in_cell);

    bool findHalfEdge(const HalfEdge& in_hedge, HalfEdgeIndex& out_hedge_id) const;
    bool findHalfFace(const HalfFaceType& in_hface, HalfFaceIndex& out_hface_id) const;
    bool findCell(const CellType& in_cell, CellIndex& out_cell_id) const;

  private:
    mutable std::mutex cells_mutex_;
    std::vector<CellType> cells_;
    std::vector<std::vector<uint32_t> > incident_cells_per_hface_;

    mutable std::mutex hfaces_mutex_;
    std::vector<HalfFaceType> hfaces_;
    std::vector<std::vector<uint32_t> > incident_hfaces_per_hedge_;

    mutable std::mutex hedges_mutex_;
    std::vector<HalfEdge> hedges_;
    std::vector<std::vector<uint32_t> > incident_hedges_per_vertex_;

    mutable std::mutex vertices_mutex_;
    std::vector<real_t> vertices_;
  };

  #include "volmesh/volmesh.tpp"

}