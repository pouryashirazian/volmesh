#include "volmesh/halfedge.h"

using namespace volmesh;

HalfEdge::HalfEdge():start_(VertexIndex::create(kSentinelIndex)), end_(VertexIndex::create(kSentinelIndex)) {

}

HalfEdge::HalfEdge(const HalfEdge& rhs):start_(rhs.start_), end_(rhs.end_) {

}

HalfEdge::HalfEdge(const VertexIndex& start, const VertexIndex& end):start_(start), end_(end) {

}

HalfEdge::~HalfEdge() {

}

const VertexIndex& HalfEdge::start() const {
  return start_;
}

const VertexIndex& HalfEdge::end() const {
  return end_;
}

bool HalfEdge::equals(const HalfEdge& rhs) const {
  return (start_.get() == rhs.start_.get())&&(end_.get() == rhs.end_.get());
}

const HalfEdge& HalfEdge::operator=(const HalfEdge& rhs) {
  start_ = rhs.start_;
  end_ = rhs.end_;
  return *this;
}
