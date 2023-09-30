#include "volmesh/tetmesh.h"
#include "volmesh/tetrahedra.h"

using namespace volmesh;

TetMesh::TetMesh() {

}

TetMesh::TetMesh(const TetMesh& rhs):TetMesh() {
  copyFrom(rhs);
}

TetMesh::TetMesh(const std::vector<vec3>& in_vertices,
                 const std::vector<vec4i>& in_cells):TetMesh() {
  bool result = readFromList(in_vertices, in_cells);
  if(result == false) {
    throw std::runtime_error("Failed to initialize a TetMesh instance.");
  }
}

TetMesh::~TetMesh() {
  clear();
}

void TetMesh::copyFrom(const TetMesh& rhs) {
  clear();

}

void TetMesh::clear() {
  //top-down clear
  cells_map_.clear();
  hfaces_map_.clear();
  hedges_map_.clear();
  normals_.clear();
  vertices_.clear();
}

bool TetMesh::readFromList(const std::vector<vec3>& in_vertices,
                           const std::vector<vec4i>& in_cells) {
  if((in_vertices.size() == 0)||(in_cells.size() == 0)) {
    return false;
  }

  clear();

  vertices_.resize(in_vertices.size() * 3);
  for(size_t i=0; i < in_vertices.size(); i++) {
    vec3 v = in_vertices[i];
    vertices_[i*3] = v.x();
    vertices_[i*3 + 1] = v.y();
    vertices_[i*3 + 2] = v.z();
  }

  bool result = true;
  for(auto it = in_cells.begin(); it != in_cells.end(); it++) {
    result &= insertNewCell(*it);
  }
  return result;
}

bool TetMesh::writeToList(std::vector<vec3>& out_vertices,
                          std::vector<vec4i>& out_cells) const {
  return false;
}

uint64_t TetMesh::countCells() const {
  return cells_map_.size() + count_hashcollided_cells_.load();
}

uint64_t TetMesh::countHalfFaces() const {
  return hfaces_map_.size() + count_hashcollided_hfaces_.load();
}

uint64_t TetMesh::countEdges() const {
  return hedges_map_.size() / 2;
}

uint64_t TetMesh::countHalfEdges() const {
  return hedges_map_.size();
}

uint64_t TetMesh::countVertices() const {
  return vertices_.size() / 3;
}

const std::vector<TetMesh::TetCell> TetMesh::cell(const CellKey& in_cell_key) const {
  std::vector<TetMesh::TetCell> result;
  {
    std::lock_guard<std::mutex> lck(cells_map_mutex_);
    const auto it = cells_map_.find(in_cell_key);
    if(it != cells_map_.end()) {
      result = it->second;
    } else {
      throw std::invalid_argument("invalid cell key");
    }
  }

  return result;
}

const std::vector<TetMesh::TetHalfFace> TetMesh::hface(const HalfFaceKey& in_hface_key) const {
  std::vector<TetMesh::TetHalfFace> result;
  {
    std::lock_guard<std::mutex> lck(hfaces_map_mutex_);
    const auto it = hfaces_map_.find(in_hface_key);
    if(it != hfaces_map_.end()) {
      result = it->second;
    } else {
      throw std::invalid_argument("invalid half-face key");
    }
  }

  return result;
}

const HalfEdge TetMesh::hedge(const HalfEdgeKey& in_hedge_key) const {
  HalfEdge result;
  {
    std::lock_guard<std::mutex> lck(hedges_map_mutex_);
    const auto it = hedges_map_.find(in_hedge_key);
    if(it != hedges_map_.end()) {
      result = it->second;
    } else {
      throw std::invalid_argument("invalid half-edge key");
    }
  }

  return result;
}

const vec3 TetMesh::vertex(const VertexKey& in_vertex_key) const {
  vec3 result;

  const uint64_t index = in_vertex_key.get();
  if(index < countVertices()) {
    result[0] = vertices_[index * 3];
    result[1] = vertices_[index * 3 + 1];
    result[2] = vertices_[index * 3 + 2];
  }

  return result;
}

bool TetMesh::insertNewCell(const vec4i& in_cell) {
  const int count_vertices = static_cast<int>(countVertices());
  for(int i=0; i < 4; i++) {
    const int vertex_id = in_cell.coeff(i);
    if(vertex_id < 0 || vertex_id >= count_vertices) {
      return false;
    }
  }

  //add half edges
  uint64_t half_edge_keys[Tetrahedra::kNumEdges * 2];
  for(int i=0; i < Tetrahedra::kNumEdges; i++) {
    const vec2i edge_vertices_lut = Tetrahedra::edgeVertexIdsLut(i);
    HalfEdge forward_hedge(VertexKey::create(in_cell.coeff(edge_vertices_lut[0])),
                           VertexKey::create(in_cell.coeff(edge_vertices_lut[1])));

    HalfEdge backward_hedge(VertexKey::create(in_cell.coeff(edge_vertices_lut[1])),
                            VertexKey::create(in_cell.coeff(edge_vertices_lut[0])));

    insertHalfEdgeIfNotExists(forward_hedge);
    insertHalfEdgeIfNotExists(backward_hedge);

    half_edge_keys[i * 2] = forward_hedge.key().get();
    half_edge_keys[i * 2 + 1] = backward_hedge.key().get();
  }

  //add half faces
  uint64_t face_keys[Tetrahedra::kNumFaces];
  for(int i=0; i < Tetrahedra::kNumFaces; i++) {
    const vec3i face_halfedges_lut = Tetrahedra::faceHalfEdgeIdsLut(i);

    HalfEdgeKey he0 = HalfEdgeKey::create(half_edge_keys[face_halfedges_lut[0]]);
    HalfEdgeKey he1 = HalfEdgeKey::create(half_edge_keys[face_halfedges_lut[1]]);
    HalfEdgeKey he2 = HalfEdgeKey::create(half_edge_keys[face_halfedges_lut[2]]);

    TetHalfFace::HalfEdgeArray hedge_array({he0, he1, he2});
    TetHalfFace forward_hface(hedge_array);

    insertHalfFaceIfNotExists(forward_hface);

    face_keys[i] = forward_hface.key().get();
  }

  //add cells
  HalfFaceKey hf0 = HalfFaceKey::create(face_keys[0]);
  HalfFaceKey hf1 = HalfFaceKey::create(face_keys[1]);
  HalfFaceKey hf2 = HalfFaceKey::create(face_keys[2]);
  HalfFaceKey hf3 = HalfFaceKey::create(face_keys[3]);
  TetCell::HalfFaceArray hface_array({hf0, hf1, hf2, hf3});
  TetCell cell(hface_array);
  insertCellIfNotExists(cell);

  return true;
}

void TetMesh::insertHalfEdgeIfNotExists(const HalfEdge& in_hedge) {
  const size_t cur_hedge_key = in_hedge.key().get();

  std::lock_guard<std::mutex> lck(hedges_map_mutex_);
  const auto it = hedges_map_.find(cur_hedge_key);
  if(it == hedges_map_.end()) {
    hedges_map_.insert(std::make_pair(cur_hedge_key, in_hedge));
  }
}

void TetMesh::insertHalfFaceIfNotExists(const TetMesh::TetHalfFace& in_hface) {
  const size_t cur_hface_key = in_hface.key().get();

  std::lock_guard<std::mutex> lck(hfaces_map_mutex_);
  const auto it = hfaces_map_.find(cur_hface_key);

  if(it == hfaces_map_.end()) {
    //if there is no such bucket exists, then insert it as a new bucket
    std::vector<TetMesh::TetHalfFace> hfaces_bucket({in_hface});
    hfaces_map_.insert(std::make_pair(cur_hface_key, hfaces_bucket));
  } else {
    //if the bucket exists then search in it, otherwise append to this bucket
    for(auto itr = it->second.begin(); itr != it->second.end(); itr++) {
      if(itr->equals(in_hface) == true) {
        return;
      }
    }

    it->second.emplace_back(in_hface);
    count_hashcollided_hfaces_ ++;
  }
}

void TetMesh::insertCellIfNotExists(const TetCell& in_cell) {
  const size_t cur_cell_key = in_cell.key().get();

  std::lock_guard<std::mutex> lck(cells_map_mutex_);
  const auto it = cells_map_.find(cur_cell_key);

  if(it == cells_map_.end()) {
    //if there is no such bucket exists, then insert it as a new bucket
    std::vector<TetMesh::TetCell> cells_bucket({in_cell});
    cells_map_.insert(std::make_pair(cur_cell_key, cells_bucket));
  } else {
    //if the bucket exists then search in it, otherwise append to this bucket
    for(auto itr = it->second.begin(); itr != it->second.end(); itr++) {
      if(itr->equals(in_cell) == true) {
        return;
      }
    }

    it->second.emplace_back(in_cell);
    count_hashcollided_cells_ ++;
  }
}