#include "volmesh/tetmesh.h"

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
  cells_.clear();
  faces_.clear();
  edges_.clear();
  normals_.clear();
  vertices_.clear();
}

bool TetMesh::readFromList(const std::vector<vec3>& in_vertices,
                           const std::vector<vec4i>& in_cells) {
  if((in_vertices.size() == 0)||(in_cells.size() == 0)) {
    return false;
  }

  vertices_.resize(in_vertices.size() * 3);
  for(size_t i=0; i < in_vertices.size(); i++) {
    vec3 v = in_vertices[i];
    vertices_[i*3] = v.x();
    vertices_[i*3 + 1] = v.y();
    vertices_[i*3 + 2] = v.z();
  }


}

bool TetMesh::writeToList(std::vector<vec3>& out_vertices,
                          std::vector<vec4i>& out_cells) const {

}

uint64_t TetMesh::countCells() const {
  return cells_.size();
}

uint64_t TetMesh::countFaces() const {
  return faces_.size();
}

uint64_t TetMesh::countEdges() const {
  return edges_.size();
}

uint64_t TetMesh::countVertices() const {
  return vertices_.size() / 3;
}
