#pragma once

#include <stdint.h>

namespace volmesh {

  static const uint64_t kSentinelKey = -1;

  class BaseKey {
    BaseKey(): key_(kSentinelKey) {}
  public:
    explicit BaseKey(const uint64_t key): key_(key) {}
    explicit BaseKey(const BaseKey& rhs);

    bool valid() const;
    uint64_t get() const;

    BaseKey& operator=(const BaseKey& rhs);
    bool operator==(const BaseKey& rhs) const;
    bool operator<(const BaseKey& rhs) const;
    bool operator>(const BaseKey& rhs) const;

    inline operator uint64_t() const {
      return key_;
    }
  private:
    uint64_t key_;
  };

  class VertexKey : public BaseKey {
    VertexKey(const uint64_t key):BaseKey(key) {}
  public:
    static VertexKey create(const uint64_t key);
  };

  class HalfEdgeKey : public BaseKey {
    HalfEdgeKey(const uint64_t key):BaseKey(key) {}
  public:
    static HalfEdgeKey create(const uint64_t key);
  };

  class HalfFaceKey : public BaseKey {
    HalfFaceKey(const uint64_t key):BaseKey(key) {}
  public:
    static HalfFaceKey create(const uint64_t key);
  };

  class CellKey : public BaseKey {
    CellKey(const uint64_t key):BaseKey(key) {}
  public:
    static CellKey create(const uint64_t key);
  };

}