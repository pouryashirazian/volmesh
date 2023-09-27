#pragma once

#include <stdint.h>

#include "volmesh/keys.h"

namespace volmesh {

  class HalfEdge {
  public:
    explicit HalfEdge(const VertexKey& start, const VertexKey& end);
    ~HalfEdge();

    const VertexKey& start() const;
    const VertexKey& end() const;

  private:
    VertexKey start_;
    VertexKey end_;
  };

}