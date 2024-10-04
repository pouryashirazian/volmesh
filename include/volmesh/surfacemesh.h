//-----------------------------------------------------------------------------
// Copyright (c) Pourya Shirazian
// All rights reserved.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.
//-----------------------------------------------------------------------------

#pragma once

#include "volmesh/basetypes.h"
#include "volmesh/mathutils.h"
#include "volmesh/index.h"
#include "volmesh/halfface.h"
#include "volmesh/aabb.h"

#include <vector>
#include <array>
#include <unordered_map>
#include <mutex>
#include <iostream>

namespace volmesh {

template <int kNumEdgesPerFace> struct SurfaceMeshLayout {
  typedef HalfFace<kNumEdgesPerFace> HalfFaceType;
};

/**
 * @brief A class representing a surface mesh composed of half-edges and half-faces.
 *
 * This template class manages a mesh composed of half-edges and half-faces, allowing for operations
 * like insertion, querying, and normal computation on the mesh structure.
 *
 * @tparam kNumEdgesPerFace The number of edges per face.
 * @tparam LayoutPolicy A policy class defining the layout of the surface mesh (defaults to `SurfaceMeshLayout`).
 */
template <int kNumEdgesPerFace, template <int NumEdgesPerFace> class LayoutPolicy = SurfaceMeshLayout>
class SurfaceMesh {
public:
  /**
   * @brief Typedef for the layout of the surface mesh, based on `kNumEdgesPerFace`.
   */
  using Layout = SurfaceMeshLayout<kNumEdgesPerFace>;

  /**
   * @brief Typedef for the half-face type, which depends on the layout.
   */
  using HalfFaceType = typename Layout::HalfFaceType;

  /**
   * @brief Typedef for an array of vertex indices corresponding to each edge of the face.
   */
  using VertexIndexArray = std::array<VertexIndex, kNumEdgesPerFace>;

  /** @brief Default constructor. */
  SurfaceMesh();

  /**
   * @brief Copy constructor.
   * @param rhs The surface mesh to copy.
   */
  explicit SurfaceMesh(const SurfaceMesh& rhs);

  /** @brief Destructor. */
  virtual ~SurfaceMesh();

  /**
   * @brief Copies data from another surface mesh.
   * @param rhs The surface mesh to copy data from.
   */
  void copyFrom(const SurfaceMesh& rhs);

  /** @brief Clears all data in the surface mesh. */
  void clear();

  /** @brief Returns the total number of faces in the mesh. */
  uint32_t countFaces() const;

  /** @brief Returns the total number of half-faces in the mesh. */
  uint32_t countHalfFaces() const;

  /** @brief Returns the total number of edges in the mesh. */
  uint32_t countEdges() const;

  /** @brief Returns the total number of half-edges in the mesh. */
  uint32_t countHalfEdges() const;

  /** @brief Returns the total number of vertices in the mesh. */
  uint32_t countVertices() const;

  /**
   * @brief Retrieves a half-face by its index.
   * @param in_hface_id The index of the half-face.
   * @return The half-face corresponding to the given index.
   */
  const HalfFaceType& halfFace(const HalfFaceIndex& in_hface_id) const;

  /**
   * @brief Computes the normal of a half-face.
   * @param in_hface_id The index of the half-face.
   * @return The normal vector of the half-face.
   */
  vec3 halfFaceNormal(const HalfFaceIndex& in_hface_id) const;

  /**
   * @brief Retrieves the vertices of a half-face.
   * @param in_hface_id The index of the half-face.
   * @return An array of vertex positions corresponding to the half-face.
   */
  std::array<vec3, kNumEdgesPerFace> halfFaceVertices(const HalfFaceIndex& in_hface_id) const;

  /**
   * @brief Retrieves a half-edge by its index.
   * @param in_hedge_id The index of the half-edge.
   * @return The half-edge corresponding to the given index.
   */
  const HalfEdge& halfEdge(const HalfEdgeIndex& in_hedge_id) const;

  /**
   * @brief Computes the pseudo-normal of a half-edge.
   * @param in_hedge_id The index of the half-edge.
   * @return The pseudo-normal vector of the half-edge.
   */
  vec3 halfEdgePseudoNormal(const HalfEdgeIndex& in_hedge_id) const;

  /**
   * @brief Retrieves the position of a vertex by its index.
   * @param in_vertex_id The index of the vertex.
   * @return The position of the vertex.
   */
  vec3 vertex(const VertexIndex& in_vertex_id) const;

  /**
   * @brief Computes the pseudo-normal of a vertex.
   * @param in_vertex_id The index of the vertex.
   * @return The pseudo-normal vector of the vertex.
   */
  vec3 vertexPseudoNormal(const VertexIndex& in_vertex_id) const;

  /**
   * @brief Inserts all vertices into the mesh.
   * @param in_vertices A vector of vertex positions to insert.
   * @return True if successful, false otherwise.
   */
  bool insertAllVertices(const std::vector<vec3>& in_vertices);

  /**
   * @brief Inserts a half-edge into the mesh if it doesn't already exist.
   * @param in_hedge The half-edge to insert.
   * @return The index of the inserted or existing half-edge.
   */
  HalfEdgeIndex insertHalfEdgeIfNotExists(const HalfEdge& in_hedge);

  /**
   * @brief Inserts a half-face into the mesh if it doesn't already exist.
   * @param in_hface The half-face to insert.
   * @return The index of the inserted or existing half-face.
   */
  HalfFaceIndex insertHalfFaceIfNotExists(const HalfFaceType& in_hface);

  /**
   * @brief Finds a half-edge by its data.
   * @param in_hedge The half-edge to find.
   * @param out_hedge_id The index of the found half-edge.
   * @return True if the half-edge is found, false otherwise.
   */
  bool findHalfEdge(const HalfEdge& in_hedge, HalfEdgeIndex& out_hedge_id) const;

  /**
   * @brief Finds a half-face by its data.
   * @param in_hface The half-face to find.
   * @param out_hface_id The index of the found half-face.
   * @return True if the half-face is found, false otherwise.
   */
  bool findHalfFace(const HalfFaceType& in_hface, HalfFaceIndex& out_hface_id) const;

  /**
   * @brief Retrieves the incident half-faces for a half-edge.
   * @param in_hedge_id The index of the half-edge.
   * @param out_incident_hface_ids A vector to store the indices of the incident half-faces.
   * @return True if successful, false otherwise.
   */
  bool getIncidentHalfFacesPerHalfEdge(const HalfEdgeIndex& in_hedge_id,
                                        std::vector<HalfFaceIndex>& out_incident_hface_ids) const;

  /**
   * @brief Retrieves the incident half-edges for a vertex.
   * @param in_vertex_id The index of the vertex.
   * @param out_incident_hedge_ids A vector to store the indices of the incident half-edges.
   * @return True if successful, false otherwise.
   */
  bool getIncidentHalfEdgesPerVertex(const VertexIndex& in_vertex_id,
                                      std::vector<HalfEdgeIndex>& out_incident_hedge_ids) const;

  /** @brief Returns the axis-aligned bounding box (AABB) of the mesh. */
  AABB bounds() const;

  /**
   * @brief Sets the normals for all half-faces.
   * @param in_hface_normals A vector of normal vectors to set.
   * @return True if successful, false otherwise.
   */
  bool setAllHalfFaceNormals(const std::vector<vec3>& in_hface_normals);

  /** @brief Returns true if the mesh has half-face normals, false otherwise. */
  bool hasHalfFaceNormals() const;

  /** @brief Returns true if the mesh has half-edge pseudo-normals, false otherwise. */
  bool hasHalfEdgePseudoNormals() const;

  /** @brief Returns true if the mesh has vertex pseudo-normals, false otherwise. */
  bool hasVertexPseudoNormals() const;

  /** @brief Computes the normals for all half-faces in the mesh. */
  void computeHalfFaceNormals();

  /** @brief Computes the pseudo-normals for all half-edges in the mesh. */
  void computeHalfEdgePseudoNormals();

  /** @brief Computes the pseudo-normals for all vertices in the mesh. */
  void computeVertexPseudoNormals();

private:
  /**
   * @brief Stores the incident half-faces for each half-edge.
   *
   * This vector of vectors holds the indices of half-faces that are incident to each half-edge in the mesh.
   * Each element in the outer vector corresponds to a half-edge, and the inner vector contains the indices
   * of half-faces incident to that particular half-edge.
   */
  std::vector<std::vector<uint32_t>> incident_hfaces_per_hedge_;

  /**
   * @brief Mutex for synchronizing access to the half-edges data structure.
   *
   * This mutex ensures thread-safe access and modification of the half-edges data, preventing race conditions
   * in concurrent environments.
   */
  mutable std::mutex hedges_mutex_;

  /**
   * @brief Stores the half-edges in the mesh.
   *
   * This vector contains all the half-edges present in the mesh. Each half-edge represents a directed edge
   * in the mesh structure.
   */
  std::vector<HalfEdge> hedges_;

  /**
   * @brief Stores the pseudo-normal vectors of half-edges.
   *
   * This vector holds the precomputed pseudo-normal vectors for each half-edge in the mesh. Pseudo-normals
   * provide approximations to normal vectors for half-edges, which can be useful for shading or other geometric computations.
   */
  std::vector<vec3> hedge_pseudo_normals_;

  /**
   * @brief Stores the incident half-edges for each vertex.
   *
   * This vector of vectors holds the indices of half-edges that are incident to each vertex in the mesh.
   * Each element in the outer vector corresponds to a vertex, and the inner vector contains the indices
   * of half-edges incident to that particular vertex.
   */
  std::vector<std::vector<uint32_t>> incident_hedges_per_vertex_;

  /**
   * @brief Mutex for synchronizing access to the vertices data structure.
   *
   * This mutex ensures thread-safe access and modification of the vertices data, preventing race conditions
   * in concurrent environments.
   */
  mutable std::mutex vertices_mutex_;

  /**
   * @brief Stores the vertex positions in the mesh.
   *
   * This vector contains the positions (as real numbers) of each vertex in the mesh. Each entry in the vector
   * corresponds to the coordinates of a vertex.
   */
  std::vector<real_t> vertices_;

  /**
   * @brief Stores the pseudo-normal vectors of vertices.
   *
   * This vector holds the precomputed pseudo-normal vectors for each vertex in the mesh. Pseudo-normals
   * provide approximations to the normal vectors for vertices, which can be useful for shading or other geometric computations.
   */
  std::vector<vec3> vertex_pseudo_normals_;

};

#include "volmesh/surfacemesh.tpp"

}