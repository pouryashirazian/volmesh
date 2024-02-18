#pragma once

#include <stdint.h>

#include "volmesh/index.h"

namespace volmesh {

  class HalfEdge {
  public:
    HalfEdge();
    HalfEdge(const HalfEdge& rhs);
    explicit HalfEdge(const VertexIndex& start, const VertexIndex& end);
    ~HalfEdge();

    const VertexIndex& start() const;
    const VertexIndex& end() const;

    bool equals(const HalfEdge& rhs) const;

    HalfEdge sortedVertexIds() const;

    const HalfEdge& operator=(const HalfEdge& rhs);

  private:
    VertexIndex start_;
    VertexIndex end_;
  };

}