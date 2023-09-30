#pragma once

#include <stdint.h>

#include "volmesh/keys.h"

namespace volmesh {

  class HalfEdge {
  public:
    HalfEdge();
    HalfEdge(const HalfEdge& rhs);
    explicit HalfEdge(const VertexKey& start, const VertexKey& end);
    ~HalfEdge();

    const VertexKey& start() const;
    const VertexKey& end() const;

    HalfEdgeKey key() const;

    bool equals(const HalfEdge& rhs) const;

    const HalfEdge& operator=(const HalfEdge& rhs);

  private:
    VertexKey start_;
    VertexKey end_;
  };

}