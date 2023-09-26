#pragma once

#include <stdint.h>

namespace volmesh {

  static const uint32_t kSentinelKey = -1;

  class BaseKey {
    BaseKey(): key_(kSentinelKey) {}
  public:
    explicit BaseKey(const uint32_t key): key_(key) {}

    bool valid() const;
    uint32_t get() const;

    BaseKey& operator=(const BaseKey& rhs);
    bool operator==(const BaseKey& rhs) const;
    bool operator<(const BaseKey& rhs) const;
    bool operator>(const BaseKey& rhs) const;

  private:
    uint32_t key_;
  };

  class VertexKey : public BaseKey {
    VertexKey(const uint32_t key):BaseKey(key) {}
  public:
    static VertexKey create(const uint32_t key);
  };

  class HalfEdgeKey : public BaseKey {
    HalfEdgeKey(const uint32_t key):BaseKey(key) {}
  public:
    static HalfEdgeKey create(const uint32_t key);
  };

  class HalfFaceKey : public BaseKey {
    HalfFaceKey(const uint32_t key):BaseKey(key) {}
  public:
    static HalfFaceKey create(const uint32_t key);
  };

  class CellKey : public BaseKey {
    CellKey(const uint32_t key):BaseKey(key) {}
  public:
    static CellKey create(const uint32_t key);
  };

}