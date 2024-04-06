#include "volmesh/mergelist.h"

#include <iostream>
#include <fmt/core.h>
#include <algorithm>

namespace volmesh {

  MergeList::MergeList(const std::size_t capacity):capacity_(capacity) {
    masks_.resize(capacity_);
    std::fill(masks_.begin(), masks_.end(), false);

    indices_.resize(capacity_);
    std::fill(indices_.begin(), indices_.end(), 0);
  }

  MergeList::~MergeList() {

  }

  std::size_t MergeList::capacity() const {
    return capacity_;
  }

  void MergeList::reset() {
    next_unique_id_ = 0;

    masks_.resize(capacity_);
    std::fill(masks_.begin(), masks_.end(), false);

    indices_.resize(capacity_);
    std::fill(indices_.begin(), indices_.end(), 0);

    {
      std::lock_guard<std::mutex> lk(hash_id_map_mutex_);
      hash_id_map_.clear();
    }
  }

  void MergeList::addHash(const std::size_t hash, const uint32_t element_id) {
    if(next_unique_id_.load() == capacity_) {
      throw std::out_of_range(fmt::format("No space left in the merge list with capacity = [{}]", capacity_));
    }

    uint32_t resolved_id = 0;
    if(exists(hash, &resolved_id) == true) {
      //already exists therefore mask out current entry
      masks_[element_id] = false;
      indices_[element_id] = resolved_id;
    } else {
      {
        //entry does not exist. store it for future with a unique id.
        std::lock_guard<std::mutex> lk(hash_id_map_mutex_);
        hash_id_map_.insert(std::make_pair(hash, next_unique_id_.load()));
      }

      masks_[element_id] = true;
      indices_[element_id] = next_unique_id_.load();
      next_unique_id_ ++;
    }
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
    return hash_id_map_.size();
  }

  std::vector<bool> MergeList::masks() const {
    return masks_;
  }

  std::vector<uint32_t> MergeList::indices() const {
    return indices_;
  }

}