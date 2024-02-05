#include "volmesh/tetmesh.h"
#include "volmesh/tetrahedra.h"

#include <iostream>

using namespace volmesh;

TetMesh::TetMesh():VolMesh<kTetMeshNumFacesPerCell, kTetMeshNumEdgesPerFace, TetMeshLayout>() {

}

TetMesh::TetMesh(const std::vector<vec3>& in_vertices,
                 const std::vector<vec4i>& in_tet_cells_by_vertex_ids):VolMesh<kTetMeshNumFacesPerCell, kTetMeshNumEdgesPerFace, TetMeshLayout>() {

  bool result = readFromList(in_vertices, in_tet_cells_by_vertex_ids);
  if(result == false) {
    throw std::runtime_error("Failed to initialize a VolMesh instance.");
  }
}

TetMesh::~TetMesh() {
  clear();
}

CellIndex TetMesh::insertTetrahedra(const vec4i& in_tet_vertex_ids) {
  const int count_vertices = static_cast<int>(countVertices());
  for(int i=0; i < 4; i++) {
    const int vertex_id = in_tet_vertex_ids.coeff(i);
    if(vertex_id < 0 || vertex_id >= count_vertices) {
      throw std::out_of_range("Some of the vertex indices for the supplied cell are out of range");
    }
  }

  //add half edges
  uint32_t half_edge_keys[Tetrahedra::kNumEdges * 2];
  for(int i=0; i < Tetrahedra::kNumEdges; i++) {
    const vec2i edge_vertices_lut = Tetrahedra::edgeVertexIdsLut(i);
    HalfEdge forward_hedge(VertexIndex::create(in_tet_vertex_ids.coeff(edge_vertices_lut[0])),
                           VertexIndex::create(in_tet_vertex_ids.coeff(edge_vertices_lut[1])));

    HalfEdge backward_hedge(VertexIndex::create(in_tet_vertex_ids.coeff(edge_vertices_lut[1])),
                            VertexIndex::create(in_tet_vertex_ids.coeff(edge_vertices_lut[0])));

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

  //add the cell by gathering all forward half-face ids
  HalfFaceIndex hf0 = HalfFaceIndex::create(face_keys[0]);
  HalfFaceIndex hf1 = HalfFaceIndex::create(face_keys[2]);
  HalfFaceIndex hf2 = HalfFaceIndex::create(face_keys[4]);
  HalfFaceIndex hf3 = HalfFaceIndex::create(face_keys[6]);
  TetMesh::Layout::CellType::HalfFaceIndexArray hface_array({hf0, hf1, hf2, hf3});
  TetMesh::Layout::CellType cell(hface_array);
  return insertCellIfNotExists(cell);
}

bool TetMesh::insertVoxel(const std::array<int, Voxel::kNumVerticesPerCell>& in_voxel_vertex_ids,
                          std::array<CellIndex, Voxel::kNumFittingTetrahedra>& out_tet_cell_ids) {

  std::array<vec4i, Voxel::kNumFittingTetrahedra> tet_cells;
  tet_cells[0] = vec4i(in_voxel_vertex_ids[Voxel::VertexLocationId::LBN], in_voxel_vertex_ids[Voxel::VertexLocationId::LTN], in_voxel_vertex_ids[Voxel::VertexLocationId::RBN], in_voxel_vertex_ids[Voxel::VertexLocationId::LBF]);
  tet_cells[1] = vec4i(in_voxel_vertex_ids[Voxel::VertexLocationId::RTN], in_voxel_vertex_ids[Voxel::VertexLocationId::LTN], in_voxel_vertex_ids[Voxel::VertexLocationId::LBF], in_voxel_vertex_ids[Voxel::VertexLocationId::RBN]);
  tet_cells[2] = vec4i(in_voxel_vertex_ids[Voxel::VertexLocationId::RTN], in_voxel_vertex_ids[Voxel::VertexLocationId::LTN], in_voxel_vertex_ids[Voxel::VertexLocationId::LTF], in_voxel_vertex_ids[Voxel::VertexLocationId::LBF]);
  tet_cells[3] = vec4i(in_voxel_vertex_ids[Voxel::VertexLocationId::RTN], in_voxel_vertex_ids[Voxel::VertexLocationId::RBN], in_voxel_vertex_ids[Voxel::VertexLocationId::LBF], in_voxel_vertex_ids[Voxel::VertexLocationId::RBF]);
  tet_cells[4] = vec4i(in_voxel_vertex_ids[Voxel::VertexLocationId::RTN], in_voxel_vertex_ids[Voxel::VertexLocationId::LBF], in_voxel_vertex_ids[Voxel::VertexLocationId::LTF], in_voxel_vertex_ids[Voxel::VertexLocationId::RBF]);
  tet_cells[5] = vec4i(in_voxel_vertex_ids[Voxel::VertexLocationId::RTN], in_voxel_vertex_ids[Voxel::VertexLocationId::LTF], in_voxel_vertex_ids[Voxel::VertexLocationId::RTF], in_voxel_vertex_ids[Voxel::VertexLocationId::RBF]);

  bool result = true;
  for(int i=0; i < Voxel::kNumFittingTetrahedra; i++) {
    out_tet_cell_ids[i] = insertTetrahedra(tet_cells[i]);

    result &= out_tet_cell_ids[i].valid();
  }

  return result;
}

bool TetMesh::readFromList(const std::vector<vec3>& in_vertices,
                           const std::vector<vec4i>& in_tet_cells_by_vertex_ids) {
  if((in_vertices.size() == 0)||(in_tet_cells_by_vertex_ids.size() == 0)) {
    throw std::invalid_argument("The supplied tetrahedra list is empty");
    return false;
  }

  clear();

  bool result = insertAllVertices(in_vertices);
  for(auto it = in_tet_cells_by_vertex_ids.begin(); it != in_tet_cells_by_vertex_ids.end(); it++) {
    insertTetrahedra(*it);
  }

  result &= (countCells() == in_tet_cells_by_vertex_ids.size());
  return result;
}

bool TetMesh::writeToList(std::vector<vec3>& out_vertices,
                          std::vector<vec4i>& out_tet_cells_by_vertex_ids) const {
  const uint32_t cells_count = countCells();
  if(cells_count == 0) {
    std::cerr << "There are no cells in this tetmesh" << std::endl;
    return false;
  }

  //copy all vertices
  const uint32_t vertices_count = countVertices();
  out_vertices.resize(vertices_count);
  for(uint32_t i=0; i < vertices_count; ++i) {
    out_vertices[i] = vertex(VertexIndex::create(i));
  }

  out_tet_cells_by_vertex_ids.resize(cells_count);
  for(uint32_t i=0; i < cells_count; ++i) {
    TetMesh::CellType curr_cell = cell(CellIndex::create(i));

    //face0 of our tet cell references edges {0, 2, 4}
    //edge 0 -> vertex {1, 2}
    //edge 2 -> vertex {3, 1}
    //edge 4 -> vertex {0, 3}

    //each tet cell has 4 half-faces
    TetMesh::HalfFaceType face0 = halfFace(curr_cell.halfFaceIndex(0));

    //each half-face has 3 half-edges
    volmesh::HalfEdge tet_edge0 = halfEdge(face0.halfEdgeIndex(0));
    volmesh::HalfEdge tet_edge2 = halfEdge(face0.halfEdgeIndex(1));
    volmesh::HalfEdge tet_edge4 = halfEdge(face0.halfEdgeIndex(2));

    out_tet_cells_by_vertex_ids[i] = vec4i(tet_edge4.start(),
                                           tet_edge0.start(),
                                           tet_edge0.end(),
                                           tet_edge2.start());
  }

  return true;
}
