#pragma once

#include <stdint.h>

#include "volmesh/keys.h"

namespace volmesh {

  class HalfEdge {
  public:
    HalfEdge(const HalfEdge& rhs);
    explicit HalfEdge(const VertexKey& start, const VertexKey& end);
    ~HalfEdge();

    const VertexKey& start() const;
    const VertexKey& end() const;

    HalfEdgeKey key() const;

  private:
    VertexKey start_;
    VertexKey end_;
  };

}