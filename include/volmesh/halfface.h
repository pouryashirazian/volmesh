#pragma once

#include "volmesh/halfedge.h"
#include "volmesh/multikeyhash.h"

#include <array>

namespace volmesh {

  template <int NumEdges>
  class HalfFace {
  public:
    typedef std::array<HalfEdgeIndex, NumEdges> HalfEdgeIndexArray;

    HalfFace();
    HalfFace(const HalfFace& rhs);
    explicit HalfFace(const HalfEdgeIndexArray& in_hedges);
    ~HalfFace();

    void copyFrom(const HalfFace& rhs);

    const HalfEdgeIndex& halfEdgeIndex(const int i) const;

    static int numEdges();

    bool equals(const HalfFace& rhs) const;

    const HalfFace& operator=(const HalfFace& rhs);

  private:
    HalfEdgeIndexArray half_edge_indices_;
  };

  #include "volmesh/halfface.tpp"

}