#include "volmesh/tetmesh.h"
#include "volmesh/tetrahedra.h"

using namespace volmesh;

TetMesh::TetMesh():VolMesh<kTetMeshNumFacesPerCell, kTetMeshNumEdgesPerFace, TetMeshLayout>() {

}

TetMesh::TetMesh(const std::vector<vec3>& in_vertices,
                 const std::vector<vec4i>& in_cells):VolMesh<kTetMeshNumFacesPerCell, kTetMeshNumEdgesPerFace, TetMeshLayout>() {

  bool result = readFromList(in_vertices, in_cells);
  if(result == false) {
    throw std::runtime_error("Failed to initialize a VolMesh instance.");
  }
}

TetMesh::~TetMesh() {
  clear();
}

CellIndex TetMesh::insertNewCell(const vec4i& in_cell) {
  CellIndex cell_index = CellIndex::create(kSentinelIndex);
  const int count_vertices = static_cast<int>(countVertices());
  for(int i=0; i < 4; i++) {
    const int vertex_id = in_cell.coeff(i);
    if(vertex_id < 0 || vertex_id >= count_vertices) {
      throw std::out_of_range("Some of the vertex indices for the supplied cell are out of range");
    }
  }

  //add half edges
  uint32_t half_edge_keys[Tetrahedra::kNumEdges * 2];
  for(int i=0; i < Tetrahedra::kNumEdges; i++) {
    const vec2i edge_vertices_lut = Tetrahedra::edgeVertexIdsLut(i);
    HalfEdge forward_hedge(VertexIndex::create(in_cell.coeff(edge_vertices_lut[0])),
                           VertexIndex::create(in_cell.coeff(edge_vertices_lut[1])));

    HalfEdge backward_hedge(VertexIndex::create(in_cell.coeff(edge_vertices_lut[1])),
                            VertexIndex::create(in_cell.coeff(edge_vertices_lut[0])));

    half_edge_keys[i * 2] = insertHalfEdgeIfNotExists(forward_hedge);
    half_edge_keys[i * 2 + 1] = insertHalfEdgeIfNotExists(backward_hedge);
  }

  //add half faces
  uint32_t face_keys[Tetrahedra::kNumFaces * 2];
  for(int i=0; i < Tetrahedra::kNumFaces; i++) {
    const vec3i face_halfedges_lut = Tetrahedra::faceHalfEdgeIdsLut(i);

    HalfEdgeIndex he0 = HalfEdgeIndex::create(half_edge_keys[face_halfedges_lut[0]]);
    HalfEdgeIndex he1 = HalfEdgeIndex::create(half_edge_keys[face_halfedges_lut[1]]);
    HalfEdgeIndex he2 = HalfEdgeIndex::create(half_edge_keys[face_halfedges_lut[2]]);

    TetMesh::Layout::HalfFaceType forward_hface(TetMesh::Layout::HalfFaceType::HalfEdgeIndexArray({he0, he1, he2}));
    TetMesh::Layout::HalfFaceType backward_hface(TetMesh::Layout::HalfFaceType::HalfEdgeIndexArray({he2, he1, he0}));

    face_keys[i * 2] = insertHalfFaceIfNotExists(forward_hface);
    face_keys[i * 2 + 1] = insertHalfFaceIfNotExists(backward_hface);
  }

  //add cells
  HalfFaceIndex hf0 = HalfFaceIndex::create(face_keys[0]);
  HalfFaceIndex hf1 = HalfFaceIndex::create(face_keys[1]);
  HalfFaceIndex hf2 = HalfFaceIndex::create(face_keys[2]);
  HalfFaceIndex hf3 = HalfFaceIndex::create(face_keys[3]);
  TetMesh::Layout::CellType::HalfFaceIndexArray hface_array({hf0, hf1, hf2, hf3});
  TetMesh::Layout::CellType cell(hface_array);
  cell_index = insertCellIfNotExists(cell);
  return cell_index;
}

bool TetMesh::readFromList(const std::vector<vec3>& in_vertices,
                           const std::vector<vec4i>& in_cells) {
  if((in_vertices.size() == 0)||(in_cells.size() == 0)) {
    return false;
  }

  clear();

  bool result = insertAllVertices(in_vertices);
  for(auto it = in_cells.begin(); it != in_cells.end(); it++) {
    insertNewCell(*it);
  }

  result &= (countCells() == in_cells.size());
  return result;
}

bool TetMesh::writeToList(std::vector<vec3>& out_vertices,
                          std::vector<vec4i>& out_cells) const {
  return false;
}
