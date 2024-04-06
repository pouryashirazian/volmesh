#pragma once

#include "volmesh/basetypes.h"
#include "volmesh/index.h"
#include "volmesh/halfface.h"

#include <vector>
#include <unordered_map>
#include <mutex>

namespace volmesh {

  template <int kNumEdgesPerFace> struct SurfaceMeshLayout {
    typedef HalfFace<kNumEdgesPerFace> HalfFaceType;
  };

  template <int kNumEdgesPerFace,
            template <int NumEdgesPerFace> class LayoutPolicy = SurfaceMeshLayout>
  class SurfaceMesh {
  public:
    using Layout = SurfaceMeshLayout<kNumEdgesPerFace>;
    using HalfFaceType = typename Layout::HalfFaceType;

    SurfaceMesh();
    explicit SurfaceMesh(const SurfaceMesh& rhs);
    virtual ~SurfaceMesh();

    void copyFrom(const SurfaceMesh& rhs);
    void clear();

    uint32_t countHalfFaces() const;
    uint32_t countEdges() const;
    uint32_t countHalfEdges() const;
    uint32_t countVertices() const;

    const HalfFaceType& halfFace(const HalfFaceIndex& in_hface_id) const;
    const HalfEdge& halfEdge(const HalfEdgeIndex& in_hedge_id) const;
    const vec3 vertex(const VertexIndex& in_vertex_id) const;

    bool insertAllVertices(const std::vector<vec3>& in_vertices);

    HalfEdgeIndex insertHalfEdgeIfNotExists(const HalfEdge& in_hedge);
    HalfFaceIndex insertHalfFaceIfNotExists(const HalfFaceType& in_hface);

    bool findHalfEdge(const HalfEdge& in_hedge, HalfEdgeIndex& out_hedge_id) const;
    bool findHalfFace(const HalfFaceType& in_hface, HalfFaceIndex& out_hface_id) const;

  private:
    mutable std::mutex hfaces_mutex_;
    std::vector<HalfFaceType> hfaces_;
    std::vector<std::vector<uint32_t> > incident_hfaces_per_hedge_;

    mutable std::mutex hedges_mutex_;
    std::vector<HalfEdge> hedges_;
    std::vector<std::vector<uint32_t> > incident_hedges_per_vertex_;

    mutable std::mutex vertices_mutex_;
    std::vector<real_t> vertices_;
  };

  #include "volmesh/surfacemesh.tpp"

}