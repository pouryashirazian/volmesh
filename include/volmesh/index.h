//-----------------------------------------------------------------------------
// Copyright (c) Pourya Shirazian
// All rights reserved.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.
//-----------------------------------------------------------------------------

#pragma once

#include <stdint.h>

namespace volmesh {

  static const uint32_t kSentinelIndex = -1;

  class BaseIndex {
    BaseIndex(): index_(kSentinelIndex) {}
  public:
    explicit BaseIndex(const uint32_t index): index_(index) {}
    BaseIndex(const BaseIndex& rhs);

    bool valid() const;
    uint32_t get() const;

    BaseIndex& operator=(const BaseIndex& rhs);
    bool operator==(const BaseIndex& rhs) const;
    bool operator<(const BaseIndex& rhs) const;
    bool operator>(const BaseIndex& rhs) const;

    inline operator uint32_t() const {
      return index_;
    }
  private:
    uint32_t index_;
  };

  class VertexIndex : public BaseIndex {
    explicit VertexIndex(const uint32_t index):BaseIndex(index) {}
  public:
    static VertexIndex create(const uint32_t index);
  };

  class HalfEdgeIndex : public BaseIndex {
    explicit HalfEdgeIndex(const uint32_t index):BaseIndex(index) {}
  public:
    static HalfEdgeIndex create(const uint32_t index);
  };

  class HalfFaceIndex : public BaseIndex {
    explicit HalfFaceIndex(const uint32_t index):BaseIndex(index) {}
  public:
    static HalfFaceIndex create(const uint32_t index);
  };

  class CellIndex : public BaseIndex {
    explicit CellIndex(const uint32_t index):BaseIndex(index) {}
  public:
    static CellIndex create(const uint32_t index);
  };

}