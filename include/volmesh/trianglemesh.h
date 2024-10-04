//-----------------------------------------------------------------------------
// Copyright (c) Pourya Shirazian
// All rights reserved.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.
//-----------------------------------------------------------------------------

#pragma once

#include "volmesh/surfacemesh.h"

namespace volmesh {

  static const int kTriangleMeshNumEdgesPerFace = 3;

/**
 * @brief Class representing a triangular surface mesh.
 *
 * The TriangleMesh class inherits from the SurfaceMesh class and provides additional
 * methods for inserting triangles and reading from vertex and normal lists. It represents
 * a mesh made up of triangular faces.
 */
template <int kNumEdgesPerFace = kTriangleMeshNumEdgesPerFace>
struct TriangleMeshLayout {
    /// Type definition for a HalfFace with kNumEdgesPerFace edges.
    typedef HalfFace<kNumEdgesPerFace> HalfFaceType;
};

/**
 * @brief Class representing a triangular surface mesh with operations for managing triangles.
 *
 * This class extends the SurfaceMesh class, implementing specific functionality for triangular meshes.
 */
class TriangleMesh: public SurfaceMesh<kTriangleMeshNumEdgesPerFace, TriangleMeshLayout> {
public:
  /**
   * @brief Default constructor.
   *
   * Initializes an empty TriangleMesh object.
   */
  TriangleMesh();

  /**
   * @brief Destructor for TriangleMesh.
   */
  virtual ~TriangleMesh();

  /**
   * @brief Inserts a triangle into the mesh using three vertex indices.
   *
   * @param a Index of the first vertex.
   * @param b Index of the second vertex.
   * @param c Index of the third vertex.
   * @return The index of the inserted triangle (half-face).
   */
  HalfFaceIndex insertTriangle(const uint32_t a, const uint32_t b, const uint32_t c);

  /**
   * @brief Inserts a triangle into the mesh using a vec3i containing vertex IDs.
   *
   * @param in_face_vertex_ids A vec3i containing the indices of the three vertices.
   * @return The index of the inserted triangle (half-face).
   */
  HalfFaceIndex insertTriangle(const vec3i& in_face_vertex_ids);

  /**
   * @brief Reads vertex and normal data from a list and adds them to the mesh.
   *
   * @param in_triangle_vertices A vector containing the vertices of the triangles.
   * @param in_triangle_normals A vector containing the normals of the triangles.
   * @return True if the operation was successful, false otherwise.
   */
  bool readFromList(const std::vector<vec3>& in_triangle_vertices,
                    const std::vector<vec3>& in_triangle_normals);

  /**
   * @brief Lookup table for the vertex IDs of a specific edge.
   *
   * @param edge_id The ID of the edge (0-2 for triangular faces).
   * @return A vec2i containing the vertex IDs of the specified edge.
   */
  static vec2i edgeVertexIdsLut(const int edge_id);

  /**
   * @brief Validates whether the given face vertex IDs are valid for a mesh.
   *
   * @param in_face_vertex_ids A vec3i containing the vertex indices of a face.
   * @param in_total_vertex_count The total number of vertices in the mesh.
   * @return True if the face vertex IDs are valid, false otherwise.
   */
  static bool validateFaceVertexIds(const vec3i& in_face_vertex_ids,
                                    const int in_total_vertex_count);
};

}