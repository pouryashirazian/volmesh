#pragma once

#include "volmesh/basetypes.h"
#include "volmesh/index.h"
#include "volmesh/halfface.h"
#include "volmesh/aabb.h"

#include <vector>
#include <array>
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

    uint32_t countFaces() const;
    uint32_t countHalfFaces() const;
    uint32_t countEdges() const;
    uint32_t countHalfEdges() const;
    uint32_t countVertices() const;

    const HalfFaceType& halfFace(const HalfFaceIndex& in_hface_id) const;
    vec3 halfFaceNormal(const HalfFaceIndex& in_hface_id) const;
    std::array<vec3, kNumEdgesPerFace> halfFaceVertices(const HalfFaceIndex& in_hface_id) const;

    const HalfEdge& halfEdge(const HalfEdgeIndex& in_hedge_id) const;
    vec3 vertex(const VertexIndex& in_vertex_id) const;

    bool insertAllVertices(const std::vector<vec3>& in_vertices);

    HalfEdgeIndex insertHalfEdgeIfNotExists(const HalfEdge& in_hedge);
    HalfFaceIndex insertHalfFaceIfNotExists(const HalfFaceType& in_hface);

    bool findHalfEdge(const HalfEdge& in_hedge, HalfEdgeIndex& out_hedge_id) const;
    bool findHalfFace(const HalfFaceType& in_hface, HalfFaceIndex& out_hface_id) const;

    bool getIncidentHalfFacesPerHalfEdge(const HalfEdgeIndex& in_hedge_id,
                                         std::vector<HalfFaceIndex>& out_incident_hface_ids) const;

    bool getIncidentHalfEdgesPerVertex(const VertexIndex& in_vertex_id,
                                       std::vector<HalfEdgeIndex>& out_incident_hedge_ids) const;

    AABB bounds() const;

    bool setAllHalfFaceNormals(const std::vector<vec3>& in_hface_normals);
    bool hasHalfFaceNormals() const;

    void computePerEdgePseudoNormals();
    void computePerVertexPseudoNormals();

  private:
    mutable std::mutex hfaces_mutex_;
    std::vector<HalfFaceType> hfaces_;
    std::vector<vec3> hface_normals_;
    std::vector<std::vector<uint32_t> > incident_hfaces_per_hedge_;

    mutable std::mutex hedges_mutex_;
    std::vector<HalfEdge> hedges_;
    std::vector<vec3> hedge_pseudo_normals_;
    std::vector<std::vector<uint32_t> > incident_hedges_per_vertex_;

    mutable std::mutex vertices_mutex_;
    std::vector<real_t> vertices_;
    std::vector<vec3> vertex_pseudo_normals_;
  };

  #include "volmesh/surfacemesh.tpp"

}