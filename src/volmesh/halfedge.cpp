#include "volmesh/halfedge.h"

#include <algorithm>

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

HalfEdge HalfEdge::sortedVertexIds() const {
  uint32_t start_id = start_.get();
  uint32_t end_id = end_.get();
  if(start_id > end_id) {
    std::swap(start_id, end_id);
  }

  return HalfEdge(VertexIndex::create(start_id), VertexIndex::create(end_id));
}

const HalfEdge& HalfEdge::operator=(const HalfEdge& rhs) {
  start_ = rhs.start_;
  end_ = rhs.end_;
  return *this;
}
