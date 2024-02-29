#include "volmesh/trianglemesh.h"
#include "volmesh/mergelist.h"
#include "volmesh/matrixhash.h"

using namespace volmesh;

TriangleMesh::TriangleMesh() {

}

TriangleMesh::~TriangleMesh() {

}

bool TriangleMesh::readFromList(const std::vector<vec3>& in_vertices,
                                const std::vector<vec3>& in_per_face_normals) {
  clear();

  if(in_vertices.size() == 0) {
    std::cerr << "Invalid input vertex list" << std::endl;
    return false;
  }

  MergeList merge_list(in_vertices.size());
  MatrixHash<vec3> hasher;

  for(size_t i=0; i < in_vertices.size(); i++) {
    size_t h = hasher(in_vertices[i]);
    merge_list.addHash(h);
  }

  auto masks = merge_list.masks();
  auto indices = merge_list.masksPrefixSum();
  size_t count_triangles = in_vertices.size() / 3;

  out_vertices.resize(merge_list.uniqueElements());
  for(size_t i=0; i < in_vertices.size(); i++) {
    if(masks[i]) {
      out_vertices[indices[i]] = in_vertices[i];
    }
  }

  out_triangles.resize(count_triangles);
  for(size_t i=0; i < count_triangles; i++) {
    out_triangles[i] = vec3i(indices[i * 3], indices[i * 3 + 1], indices[i * 3 + 2]);
  }

  std::cout << fmt::format("Total unique elements in the vertex list [{} / {}]", merge_list.uniqueElements(), in_vertices.size()) << std::endl;
}
