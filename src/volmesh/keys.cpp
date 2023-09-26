#include "volmesh/keys.h"

using namespace volmesh;

bool BaseKey::valid() const {
  return key_ != volmesh::kSentinelKey;
}

uint32_t BaseKey::get() const {
  return key_;
}

BaseKey& BaseKey::operator=(const BaseKey& rhs) {
  key_ = rhs.key_;
  return *this;
}

bool BaseKey::operator==(const BaseKey& rhs) const {
  return key_ == rhs.key_;
}

bool BaseKey::operator<(const BaseKey& rhs) const {
  return key_ < rhs.key_;
}

bool BaseKey::operator>(const BaseKey& rhs) const {
  return key_ > rhs.key_;
}

VertexKey VertexKey::create(const uint32_t key) {
  return VertexKey(key);
}

HalfEdgeKey HalfEdgeKey::create(const uint32_t key) {
  return HalfEdgeKey(key);
}

HalfFaceKey HalfFaceKey::create(const uint32_t key) {
  return HalfFaceKey(key);
}

CellKey CellKey::create(const uint32_t key) {
  return CellKey(key);
}
