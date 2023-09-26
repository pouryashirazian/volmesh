#include "volmesh/halfedge.h"

using namespace volmesh;

HalfEdge::HalfEdge(const VertexKey& start, const VertexKey& end):start_(start), end_(end) {

}

HalfEdge::~HalfEdge() {

}

const VertexKey& HalfEdge::start() const {
  return start_;
}

const VertexKey& HalfEdge::end() const {
  return end_;
}
