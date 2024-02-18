#include "volmesh/tetmesh.h"
#include "volmesh/tetrahedra.h"

#include <iostream>
#include <fstream>

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

    //face0 of each tet cell references half-edges {0, 2, 4} which correspond to edges {0, 1, 2}
    //edge 0 -> vertex {1, 2}
    //edge 1 -> vertex {2, 3}
    //edge 2 -> vertex {3, 1}

    //face2 of each tet cell references half-edges {9, 10, 5} which correspond to edges {4, 5, 2}
    //edge 4 -> vertex {0, 3}
    //edge 5 -> vertex {0, 1}
    //edge 2 -> vertex {3, 1}

    //each tet cell has 4 half-faces
    TetMesh::HalfFaceType face0 = halfFace(curr_cell.halfFaceIndex(0));

    //each half-face has 3 half-edges
    volmesh::HalfEdge face0_hedge0 = halfEdge(face0.halfEdgeIndex(0)).sortedVertexIds();
    volmesh::HalfEdge face0_hedge2 = halfEdge(face0.halfEdgeIndex(1)).sortedVertexIds();

    TetMesh::HalfFaceType face2 = halfFace(curr_cell.halfFaceIndex(2));
    volmesh::HalfEdge face2_hedge0 = halfEdge(face2.halfEdgeIndex(0)).sortedVertexIds();

    out_tet_cells_by_vertex_ids[i] = vec4i(face2_hedge0.start(),
                                           face0_hedge0.start(),
                                           face0_hedge0.end(),
                                           face0_hedge2.end());
  }

  return true;
}

template <typename T>
void SwapEndianness(T& var) {
  char* varArray = reinterpret_cast<char*>(&var);
  for(long i = 0; i < static_cast<long>(sizeof(var)/2); i++) {
    std::swap(varArray[sizeof(var) - 1 - i], varArray[i]);
  }
}

bool TetMesh::exportToVTK(const std::string& filepath, const bool is_binary) {
  //export to list of vertices and elements
  std::vector<vec3> vertices;
  std::vector<vec4i> cells;
  bool result = writeToList(vertices, cells);
  if(result == false) {
    std::cerr << "Failed to export to list of vertices and tetrahedral cells" << std::endl;
    return false;
  }

  std::ofstream file;
  if(is_binary) {
    file.open(filepath.c_str(), std::ios::out | std::ios::binary);
  } else {
    file.open(filepath.c_str());
  }

  file << "# vtk DataFile Version 3.0" << std::endl;
  file << "volmesh exported Tetrahedral Mesh" << std::endl;

  if(is_binary)
    file << "BINARY"<< std::endl;
  else
    file << "ASCII"<< std::endl;

  file << "DATASET UNSTRUCTURED_GRID" << std::endl;

  //write the points
  std::string datatype_str = "float";
  if(typeid(real_t) == typeid(double))
    datatype_str = "double";
  else
    datatype_str = "float";

  file << "POINTS " << vertices.size() << " " << datatype_str << std::endl;
  for (size_t i = 0; i < vertices.size(); i++) {
    vec3 p = vertices[i];

    if(is_binary) {
      real_t x = p.x();
      real_t y = p.y();
      real_t z = p.z();

      SwapEndianness(x);
      SwapEndianness(y);
      SwapEndianness(z);

      file.write(reinterpret_cast<char*>(&x), sizeof(real_t));
      file.write(reinterpret_cast<char*>(&y), sizeof(real_t));
      file.write(reinterpret_cast<char*>(&z), sizeof(real_t));
    } else {
      file << p.x() << " " << p.y() << " " << p.z() << std::endl;
    }
  }

  if(is_binary) {
    file << std::endl;
  }

  //write the cells
  const int total_cells_count = static_cast<int>(cells.size());

  //4 vertices per cell and 1 for the number of vertices in each cell
  int total_indices_count = total_cells_count * 5;

  file << "CELLS " << total_cells_count << " " << total_indices_count << std::endl;

  for (size_t i = 0; i < cells.size(); i++) {
    vec4i cell = cells[i];

    if(is_binary) {
      int nodes_per_cell = 4;
      int x = cell.x();
      int y = cell.y();
      int z = cell.z();
      int w = cell.w();

      SwapEndianness(nodes_per_cell);
      SwapEndianness(x);
      SwapEndianness(y);
      SwapEndianness(z);
      SwapEndianness(w);

      file.write(reinterpret_cast<char*>(&nodes_per_cell), sizeof(int));
      file.write(reinterpret_cast<char*>(&x), sizeof(int));
      file.write(reinterpret_cast<char*>(&y), sizeof(int));
      file.write(reinterpret_cast<char*>(&z), sizeof(int));
      file.write(reinterpret_cast<char*>(&w), sizeof(int));
    } else {
      file << "4 " << cell.x() << " " << cell.y() << " " << cell.z() << " " << cell.w() << std::endl;
    }
  }

  //Write the cell types
  file << "CELL_TYPES " << total_cells_count << std::endl;

  // VTK_TETRA
  int vtk_tetrahedra_celltype = 10;
  SwapEndianness(vtk_tetrahedra_celltype);
  for (size_t i = 0; i < total_cells_count; i++) {
    if(is_binary) {
      file.write(reinterpret_cast<const char*>(&vtk_tetrahedra_celltype), sizeof(int));
    } else {
      file << "10" << std::endl;
    }
  }

  file.close();
  return true;
}