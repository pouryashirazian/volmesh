#include "volmesh/tetmesh.h"

using namespace volmesh;

TetMesh::TetMesh() {

}

TetMesh::TetMesh(const TetMesh& rhs) {

}

TetMesh::~TetMesh() {

}

void TetMesh::copyFrom(const TetMesh& rhs) {

}

void TetMesh::clear() {
  //top-down clear
  cells_.clear();
  faces_.clear();
  edges_.clear();
  normals_.clear();
  vertices_.clear();
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
  return vertices_.size();
}
