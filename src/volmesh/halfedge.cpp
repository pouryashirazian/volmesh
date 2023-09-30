#include "volmesh/halfedge.h"

using namespace volmesh;

HalfEdge::HalfEdge():start_(VertexKey::create(kSentinelKey)), end_(VertexKey::create(kSentinelKey)) {

}

HalfEdge::HalfEdge(const HalfEdge& rhs):start_(rhs.start_), end_(rhs.end_) {

}

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

HalfEdgeKey HalfEdge::key() const {
  const uint64_t k = (start_.get() << 32) | end_.get();
  return HalfEdgeKey::create(k);
}

bool HalfEdge::equals(const HalfEdge& rhs) const {
  return (start_.get() == rhs.start_.get())&&(end_.get() == rhs.end_.get());
}

const HalfEdge& HalfEdge::operator=(const HalfEdge& rhs) {
  start_ = rhs.start_;
  end_ = rhs.end_;
  return *this;
}
