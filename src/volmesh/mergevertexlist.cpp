#include "volmesh/mergevertexlist.h"
#include "volmesh/matrixhash.h"

#include <iostream>
#include <fmt/core.h>
#include <algorithm>

namespace volmesh {

  MergeList::MergeList(const std::size_t capacity):capacity_(capacity) {
    masks_.resize(capacity_);
    std::fill(masks_.begin(), masks_.end(), false);

    masks_prefix_sum_.resize(capacity_);
    std::fill(masks_prefix_sum_.begin(), masks_prefix_sum_.end(), 0);
  }

  MergeList::~MergeList() {

  }

  std::size_t MergeList::capacity() const {
    return capacity_;
  }

  void MergeList::reset() {
    next_id_ = 0;

    masks_.resize(capacity_);
    std::fill(masks_.begin(), masks_.end(), false);

    masks_prefix_sum_.resize(capacity_);
    std::fill(masks_prefix_sum_.begin(), masks_prefix_sum_.end(), 0);

    {
      std::lock_guard<std::mutex> lk(hash_id_map_mutex_);
      hash_id_map_.clear();
    }
  }

  void MergeList::addHash(const std::size_t hash) {
    if(next_id_ == capacity_) {
      throw std::out_of_range(fmt::format("No space left in the merge list with capacity = [{}]", capacity_));
    }

    const uint32_t id = next_id_.load();

    if(exists(hash) == true) {
      masks_[id] = false;
    } else {
      std::lock_guard<std::mutex> lk(hash_id_map_mutex_);
      hash_id_map_.insert(std::make_pair(hash, id));
      masks_[id] = true;
    }

    if(id > 0) {
      masks_prefix_sum_[id] = masks_prefix_sum_[id - 1] + masks_[id - 1];
    } else {
      masks_prefix_sum_[id] = 0;
    }

    next_id_ ++;
  }

  bool MergeList::exists(const std::size_t hash, uint32_t* out_index) const {
    std::lock_guard<std::mutex> lk(hash_id_map_mutex_);
    const auto it = hash_id_map_.find(hash);
    if(it == hash_id_map_.end()) {
      return false;
    } else {
      if(out_index != nullptr) {
        *out_index = it->second;
      }
      return true;
    }
  }

  std::size_t MergeList::uniqueElements() const {
    const uint32_t last_id = next_id_.load() - 1;
    return masks_prefix_sum_[last_id] + masks_[last_id];
  }

  std::vector<bool> MergeList::masks() const {
    return masks_;
  }

  std::vector<uint32_t> MergeList::masksPrefixSum() const {
    return masks_prefix_sum_;
  }

  bool MergeVertexList(const std::vector<vec3>& in_vertices,
                       const std::vector<vec3>& in_per_face_normals,
                       std::vector<vec3>& out_vertices,
                       std::vector<vec3>& out_per_vertex_normals,
                       std::vector<vec3i>& out_triangles) {
    if(in_vertices.size() == 0) {
      std::cerr << "Invalid input vertex list" << std::endl;
      return false;
    }

    MergeList merge_list(in_vertices.size());
    MatrixHash<vec3> hasher;

    for(size_t i=0; i < in_vertices.size(); i++) {
      size_t h = hasher(in_vertices[i]);
      merge_list.addHash(h);
    }

    auto masks = merge_list.masks();
    auto indices = merge_list.masksPrefixSum();
    size_t count_triangles = in_vertices.size() / 3;

    out_vertices.resize(merge_list.uniqueElements());
    for(size_t i=0; i < in_vertices.size(); i++) {
      if(masks[i]) {
        out_vertices[indices[i]] = in_vertices[i];
      }
    }

    out_triangles.resize(count_triangles);
    for(size_t i=0; i < count_triangles; i++) {
      out_triangles[i] = vec3i(indices[i * 3], indices[i * 3 + 1], indices[i * 3 + 2]);
    }

    std::cout << fmt::format("Total unique elements in the vertex list [{} / {}]", merge_list.uniqueElements(), in_vertices.size()) << std::endl;

    return true;
  }


}