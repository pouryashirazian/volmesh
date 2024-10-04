//-----------------------------------------------------------------------------
// Copyright (c) Pourya Shirazian
// All rights reserved.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.
//-----------------------------------------------------------------------------

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

    void addHash(const std::size_t hash, const uint32_t element_id);
    bool exists(const std::size_t hash, uint32_t* out_index = nullptr) const;
    std::size_t uniqueElements() const;
    std::vector<bool> masks() const;
    std::vector<uint32_t> indices() const;

  private:
    std::size_t capacity_ = 0;
    std::atomic<uint32_t> next_unique_id_ = {0};

    mutable std::mutex hash_id_map_mutex_;
    std::unordered_map<std::size_t, uint32_t> hash_id_map_;
    std::vector<bool> masks_;
    std::vector<uint32_t> indices_;
  };

}