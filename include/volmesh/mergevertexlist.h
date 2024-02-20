#pragma once

#include "volmesh/basetypes.h"

#include <vector>
#include <mutex>
#include <atomic>

namespace volmesh {

  class MergeList {
  private:
    MergeList(const MergeList& ) = delete;

  public:
    MergeList(const std::size_t capacity);
    virtual ~MergeList();

    std::size_t capacity() const;
    void reset();
    void addHash(const std::size_t hash);
    bool exists(const std::size_t hash, uint32_t* out_index = nullptr) const;
    std::size_t uniqueElements() const;

    std::vector<bool> masks() const;
    std::vector<uint32_t> masksPrefixSum() const;

  private:
    std::size_t capacity_ = 0;
    std::atomic<uint32_t> next_id_ = 0;
    mutable std::mutex hash_id_map_mutex_;
    std::unordered_map<std::size_t, uint32_t> hash_id_map_;
    std::vector<bool> masks_;
    std::vector<uint32_t> masks_prefix_sum_;
  };

  bool MergeVertexList(const std::vector<vec3>& in_vertices,
                       const std::vector<vec3>& in_per_face_normals,
                       std::vector<vec3>& out_vertices,
                       std::vector<vec3>& out_per_vertex_normals,
                       std::vector<vec3i>& out_triangles);

}