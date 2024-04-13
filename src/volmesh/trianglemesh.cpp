#include "volmesh/trianglemesh.h"
#include "volmesh/mergelist.h"
#include "volmesh/matrixhash.h"

#include <spdlog/spdlog.h>
#include <fmt/core.h>

using namespace volmesh;

TriangleMesh::TriangleMesh() {

}

TriangleMesh::~TriangleMesh() {

}

HalfFaceIndex TriangleMesh::insertTriangle(const uint32_t a, const uint32_t b, const uint32_t c) {
  return insertTriangle(vec3i(static_cast<int>(a), static_cast<int>(b), static_cast<int>(c)));
}

HalfFaceIndex TriangleMesh::insertTriangle(const vec3i& in_face_vertex_ids) {
  const int count_vertices = static_cast<int>(countVertices());
  if(validateFaceVertexIds(in_face_vertex_ids, count_vertices) == false) {
    const std::string error_message = fmt::format("Supplied face vertex ids is invalid [{}, {}, {}]",
                                                  in_face_vertex_ids[0], in_face_vertex_ids[1], in_face_vertex_ids[2]);
    SPDLOG_ERROR(error_message);
    throw std::invalid_argument(error_message);
  }

  //add half edges
  uint32_t half_edge_keys[kTriangleMeshNumEdgesPerFace * 2];
  for(int i=0; i < kTriangleMeshNumEdgesPerFace; i++) {
    const vec2i edge_vertices_lut = edgeVertexIdsLut(i);
    HalfEdge forward_hedge(VertexIndex::create(in_face_vertex_ids.coeff(edge_vertices_lut[0])),
                           VertexIndex::create(in_face_vertex_ids.coeff(edge_vertices_lut[1])));

    HalfEdge backward_hedge(VertexIndex::create(in_face_vertex_ids.coeff(edge_vertices_lut[1])),
                            VertexIndex::create(in_face_vertex_ids.coeff(edge_vertices_lut[0])));

    half_edge_keys[i * 2] = insertHalfEdgeIfNotExists(forward_hedge);
    half_edge_keys[i * 2 + 1] = insertHalfEdgeIfNotExists(backward_hedge);
  }

  //gather forward half edges
  HalfEdgeIndex he0 = HalfEdgeIndex::create(half_edge_keys[0]);
  HalfEdgeIndex he1 = HalfEdgeIndex::create(half_edge_keys[2]);
  HalfEdgeIndex he2 = HalfEdgeIndex::create(half_edge_keys[4]);

  TriangleMesh::Layout::HalfFaceType forward_hface(TriangleMesh::Layout::HalfFaceType::HalfEdgeIndexArray({he0, he1, he2}));
  return insertHalfFaceIfNotExists(forward_hface);
}

bool TriangleMesh::readFromList(const std::vector<vec3>& in_triangle_vertices,
                                const std::vector<vec3>& in_triangle_normals) {
  if(in_triangle_vertices.size() == 0) {
    SPDLOG_ERROR("Invalid input vertex list");
    return false;
  }

  MergeList merge_list(in_triangle_vertices.size());
  MatrixHash<vec3> hasher;
  for(size_t i=0; i < in_triangle_vertices.size(); i++) {
    merge_list.addHash(hasher(in_triangle_vertices[i]), i);
  }

  const auto masks = merge_list.masks();
  SPDLOG_DEBUG("Total unique elements in the vertex list [{} of {}]",
                merge_list.uniqueElements(),
                in_triangle_vertices.size());

  std::vector<vec3> unique_vertices(merge_list.uniqueElements());
  std::vector<uint32_t> indices = merge_list.indices();
  for(size_t i=0; i < in_triangle_vertices.size(); i++) {
    if(masks[i]) {
      unique_vertices[indices[i]] = in_triangle_vertices[i];
    }
  }

  //set triangle mesh data
  clear();
  bool result = insertAllVertices(unique_vertices);
  size_t count_triangles = in_triangle_vertices.size() / 3;
  for(size_t i=0; i < count_triangles; i++) {
    insertTriangle(indices[i * 3], indices[i * 3 + 1], indices[i * 3 + 2]);
  }

  result &= (countHalfFaces() == count_triangles);

  return result;
}

vec2i TriangleMesh::edgeVertexIdsLut(const int edge_id) {
  static const vec2i kEdgeVertexIdsLut[kTriangleMeshNumEdgesPerFace] = { {0, 1}, {1, 2}, {2, 0} };

  if(edge_id >=0 && edge_id < kTriangleMeshNumEdgesPerFace) {
    return kEdgeVertexIdsLut[edge_id];
  } else {
    throw std::out_of_range(fmt::format("The supplied edge id {} is out of range. Accepted edge id range is [{}, {}]",
                                        edge_id,
                                        0,
                                        kTriangleMeshNumEdgesPerFace - 1));
  }
}

bool TriangleMesh::validateFaceVertexIds(const vec3i& in_face_vertex_ids,
                                         const int in_total_vertex_count) {
  return ((in_face_vertex_ids.coeff(0) >= 0 && in_face_vertex_ids.coeff(0) < in_total_vertex_count)&&
          (in_face_vertex_ids.coeff(1) >= 0 && in_face_vertex_ids.coeff(1) < in_total_vertex_count)&&
          (in_face_vertex_ids.coeff(2) >= 0 && in_face_vertex_ids.coeff(2) < in_total_vertex_count)&&
          (in_face_vertex_ids.coeff(0) != in_face_vertex_ids.coeff(1))&&
          (in_face_vertex_ids.coeff(0) != in_face_vertex_ids.coeff(2))&&
          (in_face_vertex_ids.coeff(1) != in_face_vertex_ids.coeff(2)));
}
