#include "volmesh/tetmesh.h"
#include "volmesh/tetrahedra.h"

using namespace volmesh;

TetMesh::TetMesh():VolMesh<kTetMeshNumFacesPerCell, kTetMeshNumEdgesPerFace, TetMeshLayout>() {

}

TetMesh::TetMesh(const std::vector<vec3>& in_vertices,
                 const std::vector<vec4i>& in_tetra_cells_by_vertex_ids):VolMesh<kTetMeshNumFacesPerCell, kTetMeshNumEdgesPerFace, TetMeshLayout>() {

  bool result = readFromList(in_vertices, in_tetra_cells_by_vertex_ids);
  if(result == false) {
    throw std::runtime_error("Failed to initialize a VolMesh instance.");
  }
}

TetMesh::~TetMesh() {
  clear();
}

CellIndex TetMesh::insertTetrahedra(const vec4i& in_tetra_vertex_ids) {
  const int count_vertices = static_cast<int>(countVertices());
  for(int i=0; i < 4; i++) {
    const int vertex_id = in_tetra_vertex_ids.coeff(i);
    if(vertex_id < 0 || vertex_id >= count_vertices) {
      throw std::out_of_range("Some of the vertex indices for the supplied cell are out of range");
    }
  }

  //add half edges
  uint32_t half_edge_keys[Tetrahedra::kNumEdges * 2];
  for(int i=0; i < Tetrahedra::kNumEdges; i++) {
    const vec2i edge_vertices_lut = Tetrahedra::edgeVertexIdsLut(i);
    HalfEdge forward_hedge(VertexIndex::create(in_tetra_vertex_ids.coeff(edge_vertices_lut[0])),
                           VertexIndex::create(in_tetra_vertex_ids.coeff(edge_vertices_lut[1])));

    HalfEdge backward_hedge(VertexIndex::create(in_tetra_vertex_ids.coeff(edge_vertices_lut[1])),
                            VertexIndex::create(in_tetra_vertex_ids.coeff(edge_vertices_lut[0])));

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
  return insertCellIfNotExists(cell);
}

bool TetMesh::insertVoxel(const std::array<int, Voxel::kNumVerticesPerCell>& in_voxel_vertex_ids,
                          std::array<CellIndex, Voxel::kNumFittingTetrahedra>& out_tetra_cell_ids) {

  std::array<vec4i, Voxel::kNumFittingTetrahedra> tet_cells;
  tet_cells[0] = vec4i(in_voxel_vertex_ids[Voxel::VertexLocationId::LBN], in_voxel_vertex_ids[Voxel::VertexLocationId::LTN], in_voxel_vertex_ids[Voxel::VertexLocationId::RBN], in_voxel_vertex_ids[Voxel::VertexLocationId::LBF]);
  tet_cells[1] = vec4i(in_voxel_vertex_ids[Voxel::VertexLocationId::RTN], in_voxel_vertex_ids[Voxel::VertexLocationId::LTN], in_voxel_vertex_ids[Voxel::VertexLocationId::LBF], in_voxel_vertex_ids[Voxel::VertexLocationId::RBN]);
  tet_cells[2] = vec4i(in_voxel_vertex_ids[Voxel::VertexLocationId::RTN], in_voxel_vertex_ids[Voxel::VertexLocationId::LTN], in_voxel_vertex_ids[Voxel::VertexLocationId::LTF], in_voxel_vertex_ids[Voxel::VertexLocationId::LBF]);
  tet_cells[3] = vec4i(in_voxel_vertex_ids[Voxel::VertexLocationId::RTN], in_voxel_vertex_ids[Voxel::VertexLocationId::RBN], in_voxel_vertex_ids[Voxel::VertexLocationId::LBF], in_voxel_vertex_ids[Voxel::VertexLocationId::RBF]);
  tet_cells[4] = vec4i(in_voxel_vertex_ids[Voxel::VertexLocationId::RTN], in_voxel_vertex_ids[Voxel::VertexLocationId::LBF], in_voxel_vertex_ids[Voxel::VertexLocationId::LTF], in_voxel_vertex_ids[Voxel::VertexLocationId::RBF]);
  tet_cells[5] = vec4i(in_voxel_vertex_ids[Voxel::VertexLocationId::RTN], in_voxel_vertex_ids[Voxel::VertexLocationId::LTF], in_voxel_vertex_ids[Voxel::VertexLocationId::RTF], in_voxel_vertex_ids[Voxel::VertexLocationId::RBF]);

  bool result = true;
  for(int i=0; i < Voxel::kNumFittingTetrahedra; i++) {
    out_tetra_cell_ids[i] = insertTetrahedra(tet_cells[i]);

    result &= out_tetra_cell_ids[i].valid();
  }

  return result;
}

bool TetMesh::readFromList(const std::vector<vec3>& in_vertices,
                           const std::vector<vec4i>& in_tetra_cells_by_vertex_ids) {
  if((in_vertices.size() == 0)||(in_tetra_cells_by_vertex_ids.size() == 0)) {
    return false;
  }

  clear();

  bool result = insertAllVertices(in_vertices);
  for(auto it = in_tetra_cells_by_vertex_ids.begin(); it != in_tetra_cells_by_vertex_ids.end(); it++) {
    insertTetrahedra(*it);
  }

  result &= (countCells() == in_tetra_cells_by_vertex_ids.size());
  return result;
}

bool TetMesh::writeToList(std::vector<vec3>& out_vertices,
                          std::vector<vec4i>& out_tetra_cells_by_vertex_ids) const {
  return false;
}
