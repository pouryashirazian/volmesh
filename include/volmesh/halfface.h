#pragma once

#include "volmesh/halfedge.h"

namespace volmesh {

  template <int NumEdges = 3>
  class HalfFace {
  public:
    HalfFace();
    HalfFace(const HalfFace& rhs);

    void copyFrom(const HalfFace& rhs);

    int numEdges() const;

    const HalfEdgeKey& edge(const int i) const;

    const HalfFace& operator=(const HalfFace& rhs);

  private:
    HalfEdgeKey edges_[NumEdges];
  };

  #include "volmesh/halfface.tpp"

}