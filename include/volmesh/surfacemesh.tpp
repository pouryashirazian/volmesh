template <int kNumEdgesPerFace, template <int NumEdgesPerFace> class LayoutPolicy>
SurfaceMesh<kNumEdgesPerFace, LayoutPolicy>::SurfaceMesh() {

}

template <int kNumEdgesPerFace, template <int NumEdgesPerFace> class LayoutPolicy>
SurfaceMesh<kNumEdgesPerFace, LayoutPolicy>::SurfaceMesh(const SurfaceMesh& rhs):SurfaceMesh() {
  copyFrom(rhs);
}

template <int kNumEdgesPerFace, template <int NumEdgesPerFace> class LayoutPolicy>
SurfaceMesh<kNumEdgesPerFace, LayoutPolicy>::~SurfaceMesh() {
  clear();
}

template <int kNumEdgesPerFace, template <int NumEdgesPerFace> class LayoutPolicy>
void SurfaceMesh<kNumEdgesPerFace, LayoutPolicy>::copyFrom(const SurfaceMesh& rhs) {
  clear();

}

template <int kNumEdgesPerFace, template <int NumEdgesPerFace> class LayoutPolicy>
void SurfaceMesh<kNumEdgesPerFace, LayoutPolicy>::clear() {
  //top-down clear
  hfaces_.clear();
  hedges_.clear();
  vertices_.clear();
}

template <int kNumEdgesPerFace, template <int NumEdgesPerFace> class LayoutPolicy>
uint32_t SurfaceMesh<kNumEdgesPerFace, LayoutPolicy>::countHalfFaces() const {
  return static_cast<uint32_t>(hfaces_.size());
}

template <int kNumEdgesPerFace, template <int NumEdgesPerFace> class LayoutPolicy>
uint32_t SurfaceMesh<kNumEdgesPerFace, LayoutPolicy>::countEdges() const {
  return static_cast<uint32_t>(hedges_.size() / 2);
}

template <int kNumEdgesPerFace, template <int NumEdgesPerFace> class LayoutPolicy>
uint32_t SurfaceMesh<kNumEdgesPerFace, LayoutPolicy>::countHalfEdges() const {
  return static_cast<uint32_t>(hedges_.size());
}

template <int kNumEdgesPerFace, template <int NumEdgesPerFace> class LayoutPolicy>
uint32_t SurfaceMesh<kNumEdgesPerFace, LayoutPolicy>::countVertices() const {
  return static_cast<uint32_t>(vertices_.size() / 3);
}

template <int kNumEdgesPerFace, template <int NumEdgesPerFace> class LayoutPolicy>
const typename SurfaceMesh<kNumEdgesPerFace, LayoutPolicy>::HalfFaceType&
SurfaceMesh<kNumEdgesPerFace, LayoutPolicy>::halfFace(const HalfFaceIndex& in_hface_id) const {
  if(in_hface_id.get() < countHalfFaces()) {
    std::lock_guard<std::mutex> lck(hfaces_mutex_);
    return hfaces_[in_hface_id.get()];
  } else {
    throw std::out_of_range("The supplied half-face id is out of range");
  }
}

template <int kNumEdgesPerFace, template <int NumEdgesPerFace> class LayoutPolicy>
const HalfEdge&
SurfaceMesh<kNumEdgesPerFace, LayoutPolicy>::halfEdge(const HalfEdgeIndex& in_hedge_id) const {
  if(in_hedge_id.get() < countHalfEdges()) {
    std::lock_guard<std::mutex> lck(hedges_mutex_);
    return hedges_[in_hedge_id.get()];
  } else {
    throw std::out_of_range("The supplied half-edge id is out of range");
  }
}

template <int kNumEdgesPerFace, template <int NumEdgesPerFace> class LayoutPolicy>
const vec3 SurfaceMesh<kNumEdgesPerFace, LayoutPolicy>::vertex(const VertexIndex& in_vertex_id) const {
  const uint64_t index = in_vertex_id.get();
  if(index < countVertices()) {
    std::lock_guard<std::mutex> lck(vertices_mutex_);
    return vec3({vertices_[index * 3],
                 vertices_[index * 3 + 1],
                 vertices_[index * 3 + 2]});
  } else {
    throw std::out_of_range("The supplied vertex id is out of range");
  }
}

template <int kNumEdgesPerFace, template <int NumEdgesPerFace> class LayoutPolicy>
bool SurfaceMesh<kNumEdgesPerFace, LayoutPolicy>::insertAllVertices(const std::vector<vec3>& in_vertices) {
  if(in_vertices.size() != 0) {
    {
      std::lock_guard<std::mutex> lck(vertices_mutex_);
      vertices_.resize(in_vertices.size() * 3);
      for(size_t i=0; i < in_vertices.size(); i++) {
        vec3 v = in_vertices[i];
        vertices_[i*3] = v.x();
        vertices_[i*3 + 1] = v.y();
        vertices_[i*3 + 2] = v.z();
      }
    }

    //first resize to zero to remove any previously set incident information
    {
      std::lock_guard<std::mutex> lck(hedges_mutex_);
      incident_hedges_per_vertex_.resize(0);
      incident_hedges_per_vertex_.resize(vertices_.size());
    }

    return true;
  } else {
    return false;
  }
}

template <int kNumEdgesPerFace, template <int NumEdgesPerFace> class LayoutPolicy>
HalfEdgeIndex SurfaceMesh<kNumEdgesPerFace, LayoutPolicy>::insertHalfEdgeIfNotExists(const HalfEdge& in_hedge) {
  HalfEdgeIndex hedge_index = HalfEdgeIndex::create(kSentinelIndex);
  bool result = findHalfEdge(in_hedge, hedge_index);
  if(result == false) {
    //add the half-edge
    {
      std::lock_guard<std::mutex> lck(hedges_mutex_);
      hedges_.emplace_back(in_hedge);
      hedge_index = HalfEdgeIndex::create(hedges_.size() - 1);
      incident_hedges_per_vertex_[in_hedge.start().get()].emplace_back(hedge_index.get());
    }

    //extend incidents vector
    {
      std::lock_guard<std::mutex> lck(hfaces_mutex_);
      incident_hfaces_per_hedge_.resize(hedges_.size());
    }
  }

  return hedge_index;
}

template <int kNumEdgesPerFace, template <int NumEdgesPerFace> class LayoutPolicy>
HalfFaceIndex SurfaceMesh<kNumEdgesPerFace, LayoutPolicy>::insertHalfFaceIfNotExists(const SurfaceMesh<kNumEdgesPerFace, LayoutPolicy>::HalfFaceType& in_hface) {
  HalfFaceIndex hface_index = HalfFaceIndex::create(kSentinelIndex);
  bool result = findHalfFace(in_hface, hface_index);
  if(result == false) {
    //add the half faces
    {
      std::lock_guard<std::mutex> lck(hfaces_mutex_);
      hfaces_.emplace_back(in_hface);
      hface_index = HalfFaceIndex::create(hfaces_.size() - 1);

      incident_hfaces_per_hedge_[in_hface.halfEdgeIndex(0).get()].emplace_back(hface_index.get());
      incident_hfaces_per_hedge_[in_hface.halfEdgeIndex(1).get()].emplace_back(hface_index.get());
      incident_hfaces_per_hedge_[in_hface.halfEdgeIndex(2).get()].emplace_back(hface_index.get());
    }

  }
  return hface_index;
}

template <int kNumEdgesPerFace, template <int NumEdgesPerFace> class LayoutPolicy>
bool SurfaceMesh<kNumEdgesPerFace, LayoutPolicy>::findHalfEdge(const HalfEdge& in_hedge, HalfEdgeIndex& out_hedge_id) const {
  out_hedge_id = HalfEdgeIndex::create(kSentinelIndex);

  VertexIndex start_id = in_hedge.start();
  if(start_id.get() < static_cast<uint32_t>(incident_hedges_per_vertex_.size())) {
    std::lock_guard<std::mutex> lck(hedges_mutex_);
    const auto incident_hedges = incident_hedges_per_vertex_[start_id.get()];
    for(auto it = incident_hedges.begin(); it != incident_hedges.end(); it++) {
      HalfEdge he = hedges_[*it];
      if(he.equals(in_hedge) == true) {
        out_hedge_id = HalfEdgeIndex::create(*it);
        return true;
      }
    }
  }

  return false;
}

template <int kNumEdgesPerFace, template <int NumEdgesPerFace> class LayoutPolicy>
bool SurfaceMesh<kNumEdgesPerFace, LayoutPolicy>::findHalfFace(const SurfaceMesh<kNumEdgesPerFace, LayoutPolicy>::HalfFaceType& in_hface, HalfFaceIndex& out_hface_id) const {
  out_hface_id = HalfFaceIndex::create(kSentinelIndex);

  HalfEdgeIndex start_hedge_id = in_hface.halfEdgeIndex(0);
  if(start_hedge_id.get() < static_cast<uint32_t>(incident_hfaces_per_hedge_.size())) {
    std::lock_guard<std::mutex> lck(hfaces_mutex_);
    const auto incident_hfaces = incident_hfaces_per_hedge_[start_hedge_id.get()];
    for(auto it = incident_hfaces.begin(); it != incident_hfaces.end(); it++) {
      HalfFaceType hf = hfaces_[*it];
      if(hf.equals(in_hface) == true) {
        out_hface_id = HalfFaceIndex::create(*it);
        return true;
      }
    }
  }

  return false;
}