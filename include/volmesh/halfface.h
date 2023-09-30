#pragma once

#include "volmesh/halfedge.h"
#include "volmesh/multikeyhash.h"

#include <array>

namespace volmesh {

  template <int NumEdges = 3>
  class HalfFace {
  public:
    typedef std::array<HalfEdgeKey, NumEdges> HalfEdgeArray;

    HalfFace(const HalfFace& rhs);
    explicit HalfFace(const HalfEdgeArray& in_hedges);
    ~HalfFace();

    void copyFrom(const HalfFace& rhs);

    const HalfEdgeKey& edge(const int i) const;

    static int numEdges();

    HalfFaceKey key() const;

    bool equals(const HalfFace& rhs) const;

    const HalfFace& operator=(const HalfFace& rhs);

  private:
    HalfEdgeArray hedges_;
  };

  #include "volmesh/halfface.tpp"

}