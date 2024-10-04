//-----------------------------------------------------------------------------
// Copyright (c) Pourya Shirazian
// All rights reserved.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.
//-----------------------------------------------------------------------------

#include "volmesh/index.h"

using namespace volmesh;

BaseIndex::BaseIndex(const BaseIndex& rhs): index_(rhs.index_) {

}

bool BaseIndex::valid() const {
  return index_ != volmesh::kSentinelIndex;
}

uint32_t BaseIndex::get() const {
  return index_;
}

BaseIndex& BaseIndex::operator=(const BaseIndex& rhs) {
  index_ = rhs.index_;
  return *this;
}

bool BaseIndex::operator==(const BaseIndex& rhs) const {
  return index_ == rhs.index_;
}

bool BaseIndex::operator<(const BaseIndex& rhs) const {
  return index_ < rhs.index_;
}

bool BaseIndex::operator>(const BaseIndex& rhs) const {
  return index_ > rhs.index_;
}

VertexIndex VertexIndex::create(const uint32_t key) {
  return VertexIndex(key);
}

HalfEdgeIndex HalfEdgeIndex::create(const uint32_t key) {
  return HalfEdgeIndex(key);
}

HalfFaceIndex HalfFaceIndex::create(const uint32_t key) {
  return HalfFaceIndex(key);
}

CellIndex CellIndex::create(const uint32_t key) {
  return CellIndex(key);
}
