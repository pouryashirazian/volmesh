#pragma once

#include <stdint.h>

namespace volmesh {

  static const uint32_t kSentinelIndex = -1;

  class BaseIndex {
    BaseIndex(): index_(kSentinelIndex) {}
  public:
    explicit BaseIndex(const uint32_t index): index_(index) {}
    explicit BaseIndex(const BaseIndex& rhs);

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
    VertexIndex(const uint32_t index):BaseIndex(index) {}
  public:
    static VertexIndex create(const uint32_t index);
  };

  class HalfEdgeIndex : public BaseIndex {
    HalfEdgeIndex(const uint32_t index):BaseIndex(index) {}
  public:
    static HalfEdgeIndex create(const uint32_t index);
  };

  class HalfFaceIndex : public BaseIndex {
    HalfFaceIndex(const uint32_t index):BaseIndex(index) {}
  public:
    static HalfFaceIndex create(const uint32_t index);
  };

  class CellIndex : public BaseIndex {
    CellIndex(const uint32_t index):BaseIndex(index) {}
  public:
    static CellIndex create(const uint32_t index);
  };

}