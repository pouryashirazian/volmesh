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
uint32_t SurfaceMesh<kNumEdgesPerFace, LayoutPolicy>::countFaces() const {
  return static_cast<uint32_t>(hfaces_.size());
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
vec3 SurfaceMesh<kNumEdgesPerFace, LayoutPolicy>::halfFaceNormal(const HalfFaceIndex& in_hface_id) const {
  if(hasHalfFaceNormals() == false) {
    throw std::out_of_range("This triangle mesh does not have per face normals.");
  }

  if(in_hface_id.get() < hface_normals_.size()) {
    std::lock_guard<std::mutex> lck(hfaces_mutex_);
    return hface_normals_[in_hface_id.get()];
  } else {
    throw std::out_of_range("The supplied half-face id is out of range");
  }
}

template <int kNumEdgesPerFace, template <int NumEdgesPerFace> class LayoutPolicy>
std::array<vec3, kNumEdgesPerFace>
SurfaceMesh<kNumEdgesPerFace, LayoutPolicy>::halfFaceVertices(const HalfFaceIndex& in_hface_id) const {
  if(in_hface_id.get() < countHalfFaces()) {
    const HalfFaceType hface = halfFace(in_hface_id);

    std::array<vec3, kNumEdgesPerFace> face_vertices;
    for(uint32_t i=0; i < kNumEdgesPerFace; i++) {
      face_vertices[i] = vertex(halfEdge(hface.halfEdgeIndex(i)).start());
    }

    return face_vertices;
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
vec3 SurfaceMesh<kNumEdgesPerFace, LayoutPolicy>::halfEdgePseudoNormal(const HalfEdgeIndex& in_hedge_id) const {
  if(in_hedge_id.get() < hedge_pseudo_normals_.size()) {
    std::lock_guard<std::mutex> lck(hedges_mutex_);
    return hedge_pseudo_normals_[in_hedge_id.get()];
  } else {
    throw std::out_of_range("There is no pseudo normal associated with the supplied half-edge id");
  }
}

template <int kNumEdgesPerFace, template <int NumEdgesPerFace> class LayoutPolicy>
vec3 SurfaceMesh<kNumEdgesPerFace, LayoutPolicy>::vertex(const VertexIndex& in_vertex_id) const {
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
vec3 SurfaceMesh<kNumEdgesPerFace, LayoutPolicy>::vertexPseudoNormal(const VertexIndex& in_vertex_id) const {
  if(in_vertex_id.get() < vertex_pseudo_normals_.size()) {
    std::lock_guard<std::mutex> lck(vertices_mutex_);
    return vertex_pseudo_normals_[in_vertex_id.get()];
  } else {
    throw std::out_of_range("There is no pseudo normal associated with the supplied vertex id");
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

template <int kNumEdgesPerFace, template <int NumEdgesPerFace> class LayoutPolicy>
bool SurfaceMesh<kNumEdgesPerFace, LayoutPolicy>::getIncidentHalfFacesPerHalfEdge(const HalfEdgeIndex& in_hedge_id,
                                                                                  std::vector<HalfFaceIndex>& out_incident_hface_ids) const {

  if(in_hedge_id.get() < static_cast<uint32_t>(incident_hfaces_per_hedge_.size())) {
    std::lock_guard<std::mutex> lock(hfaces_mutex_);
    out_incident_hface_ids.reserve(incident_hfaces_per_hedge_[in_hedge_id.get()].size());
    for(const auto it: incident_hfaces_per_hedge_[in_hedge_id.get()]) {
      out_incident_hface_ids.emplace_back(HalfFaceIndex::create(it));
    }

    return true;
  }

  return false;
}

template <int kNumEdgesPerFace, template <int NumEdgesPerFace> class LayoutPolicy>
bool SurfaceMesh<kNumEdgesPerFace, LayoutPolicy>::getIncidentHalfEdgesPerVertex(const VertexIndex& in_vertex_id,
                                                                                std::vector<HalfEdgeIndex>& out_incident_hedge_ids) const {
  if(in_vertex_id.get() < static_cast<uint32_t>(incident_hedges_per_vertex_.size())) {
    std::lock_guard<std::mutex> lock(hedges_mutex_);
    out_incident_hedge_ids.reserve(incident_hedges_per_vertex_[in_vertex_id.get()].size());
    for(const auto it: incident_hedges_per_vertex_[in_vertex_id.get()]) {
      out_incident_hedge_ids.emplace_back(HalfEdgeIndex::create(it));
    }

    return true;
  }

  return false;
}

template <int kNumEdgesPerFace, template <int NumEdgesPerFace> class LayoutPolicy>
AABB SurfaceMesh<kNumEdgesPerFace, LayoutPolicy>::bounds() const {
  const size_t count_vertices = countVertices();
  if(count_vertices == 0) {
    return AABB();
  }

  real_t lx = std::numeric_limits<real_t>::max();
  real_t ly = std::numeric_limits<real_t>::max();
  real_t lz = std::numeric_limits<real_t>::max();

  real_t ux = std::numeric_limits<real_t>::min();
  real_t uy = std::numeric_limits<real_t>::min();
  real_t uz = std::numeric_limits<real_t>::min();

  for(size_t i=0; i < count_vertices; i++) {
    const vec3 v = vertex(VertexIndex::create(i));
    lx = std::min<real_t>(lx, v.x());
    ly = std::min<real_t>(ly, v.y());
    lz = std::min<real_t>(lz, v.z());

    ux = std::max<real_t>(ux, v.x());
    uy = std::max<real_t>(uy, v.y());
    uz = std::max<real_t>(uz, v.z());
  }

  return AABB(vec3(lx, ly, lz), vec3(ux, uy, uz));
}

template <int kNumEdgesPerFace, template <int NumEdgesPerFace> class LayoutPolicy>
bool SurfaceMesh<kNumEdgesPerFace, LayoutPolicy>::setAllHalfFaceNormals(const std::vector<vec3>& in_hface_normals) {
  if(in_hface_normals.size() == countHalfFaces()) {
    std::lock_guard<std::mutex> lock(hfaces_mutex_);
    hface_normals_.clear();
    hface_normals_.assign(in_hface_normals.begin(), in_hface_normals.end());

    return true;
  }

  return false;
}

template <int kNumEdgesPerFace, template <int NumEdgesPerFace> class LayoutPolicy>
bool SurfaceMesh<kNumEdgesPerFace, LayoutPolicy>::hasHalfFaceNormals() const {
  return (hface_normals_.size() == countHalfFaces()) && (countHalfFaces() != 0);
}

template <int kNumEdgesPerFace, template <int NumEdgesPerFace> class LayoutPolicy>
bool SurfaceMesh<kNumEdgesPerFace, LayoutPolicy>::hasHalfEdgePseudoNormals() const {
  return (hedge_pseudo_normals_.size() == countHalfEdges()) && (countHalfEdges() != 0);
}

template <int kNumEdgesPerFace, template <int NumEdgesPerFace> class LayoutPolicy>
bool SurfaceMesh<kNumEdgesPerFace, LayoutPolicy>::hasVertexPseudoNormals() const {
  return (vertex_pseudo_normals_.size() == countVertices()) && (countVertices() != 0);
}

template <int kNumEdgesPerFace, template <int NumEdgesPerFace> class LayoutPolicy>
void SurfaceMesh<kNumEdgesPerFace, LayoutPolicy>::computeHalfFaceNormals() {
  if(countHalfFaces() == 0) {
    return;
  }

  //clear the normals
  if(hface_normals_.size() != 0) {
    std::lock_guard<std::mutex> lock(hfaces_mutex_);
    hface_normals_.clear();
  }

  const uint32_t count_hfaces = countHalfFaces();

  std::lock_guard<std::mutex> lock(hfaces_mutex_);
  hface_normals_.resize(count_hfaces);
  for(uint32_t hf = 0; hf < count_hfaces; hf++) {
    const auto hface_vertices = halfFaceVertices(HalfFaceIndex::create(hf));
    vec3 normal = (hface_vertices[1] - hface_vertices[0]).cross(hface_vertices[2] - hface_vertices[0]).normalized();
    hface_normals_[hf] = normal;
  }
}

template <int kNumEdgesPerFace, template <int NumEdgesPerFace> class LayoutPolicy>
void SurfaceMesh<kNumEdgesPerFace, LayoutPolicy>::computeHalfEdgePseudoNormals() {
  const bool compute_face_normals = (hasHalfFaceNormals() == false);

  //resize pseudo normals to zero and reserve
  {
    std::lock_guard<std::mutex> lock(hedges_mutex_);
    hedge_pseudo_normals_.resize(0);
    hedge_pseudo_normals_.reserve(countHalfEdges());
  }

  //computes pseudo normals for all half-edges
  for(uint32_t he = 0; he < countHalfEdges(); he+=2) {
    std::vector<HalfFaceIndex> incident_hface_left;
    std::vector<HalfFaceIndex> incident_hface_right;
    getIncidentHalfFacesPerHalfEdge(HalfEdgeIndex::create(he), incident_hface_left);
    getIncidentHalfFacesPerHalfEdge(HalfEdgeIndex::create(he + 1), incident_hface_right);

    vec3 n1, n2;
    if(compute_face_normals) {
      const auto vertices_hface_left = halfFaceVertices(incident_hface_left[0]);
      const auto vertices_hface_right = halfFaceVertices(incident_hface_right[0]);

      n1 = (vertices_hface_left[1] - vertices_hface_left[0]).cross(vertices_hface_left[2] - vertices_hface_left[0]).normalized();
      n2 = (vertices_hface_right[1] - vertices_hface_right[0]).cross(vertices_hface_right[2] - vertices_hface_right[0]).normalized();
    } else {
      n1 = halfFaceNormal(incident_hface_left[0]);
      n2 = halfFaceNormal(incident_hface_right[0]);
    }

    const vec3 pseudo_normal = (M_PI * n1 + M_PI * n2).normalized();
    {
      std::lock_guard<std::mutex> lock(hedges_mutex_);
      hedge_pseudo_normals_.emplace_back(pseudo_normal);
      hedge_pseudo_normals_.emplace_back(pseudo_normal);
    }
  }
}

template <int kNumEdgesPerFace, template <int NumEdgesPerFace> class LayoutPolicy>
void SurfaceMesh<kNumEdgesPerFace, LayoutPolicy>::computePerVertexPseudoNormals() {
  const uint32_t count_vertices = countVertices();
  if(count_vertices == 0) {
    return;
  }

  //make sure we have per face normals
  if(hasHalfFaceNormals() == false) {
    computeHalfFaceNormals();
  }

  //delete all vertex normals
  {
    std::lock_guard<std::mutex> lk(vertices_mutex_);
    vertex_pseudo_normals_.resize(0);
  }

  //loop over all vertices and find all incident half-edges
  for(uint32_t vid=0; vid < count_vertices; vid++) {
    const VertexIndex vertex_id = VertexIndex::create(vid);

    //fetch all half edges incident to vertex
    std::vector<HalfEdgeIndex> incident_hedge_ids;
    getIncidentHalfEdgesPerVertex(vertex_id, incident_hedge_ids);

    //incident half-faces
    std::vector<HalfFaceIndex> incident_hface_ids;
    for(const auto hedge_id: incident_hedge_ids) {
      const HalfEdge he = halfEdge(hedge_id);

      std::cout << "vertex id: " << vertex_id << ", hedge: [" << he.start() << ", " << he.end() << "]" << std::endl;

      incident_hface_ids.clear();
      getIncidentHalfFacesPerHalfEdge(hedge_id, incident_hface_ids);

      for(const auto hface_id: incident_hface_ids) {
        const HalfFaceType hface = halfFace(hface_id);
        const vec3 hface_normal = halfFaceNormal(hface_id);

        std::array<uint32_t, 3> hface_vertex_ids;
        std::cout << "vertex id: " << vertex_id << ", hface: [";
        for(int i=0; i < kNumEdgesPerFace; i++) {
          hface_vertex_ids[i] = halfEdge(hface.halfEdgeIndex(i)).start();
          std::cout << hface_vertex_ids[i];
          if(i < kNumEdgesPerFace - 1) {
            std::cout << ", ";
          }
        }
        std::cout << "]" << std::endl;
      }
    }
  }
}
