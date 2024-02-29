#include "volmesh/mergelist.h"

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

}