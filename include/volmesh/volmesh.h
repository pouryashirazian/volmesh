//-----------------------------------------------------------------------------
// Copyright (c) Pourya Shirazian
// All rights reserved.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.
//-----------------------------------------------------------------------------

#pragma once

#include "volmesh/cell.h"
#include "volmesh/basetypes.h"
#include "volmesh/index.h"

#include <vector>
#include <unordered_map>
#include <mutex>

namespace volmesh {

/**
 * @brief Layout policy for volumetric meshes, defining basic structure of a cell.
 *
 * The VolMeshLayout template defines the types of cells and half-faces, as well as the number of vertices
 * per cell for volumetric meshes.
 */
template <int kNumFacesPerCell, int kNumEdgesPerFace>
struct VolMeshLayout {
    /// Defines the type of a cell with kNumFacesPerCell faces.
    typedef Cell<kNumFacesPerCell> CellType;

    /// Defines the type of a half-face with kNumEdgesPerFace edges.
    typedef HalfFace<kNumEdgesPerFace> HalfFaceType;

    /// The number of vertices per cell, derived from the number of faces and edges.
    static const int kNumVerticesPerCell = (kNumFacesPerCell * kNumEdgesPerFace) / 3;
};

/**
 * @brief Class representing a volumetric mesh with cells and faces.
 *
 * This class provides operations for inserting, accessing, and counting various elements
 * (cells, half-faces, edges, vertices) of a volumetric mesh.
 *
 * @tparam kNumFacesPerCell The number of faces per cell in the mesh.
 * @tparam kNumEdgesPerFace The number of edges per face in the mesh.
 * @tparam LayoutPolicy Policy that defines the layout of the mesh. Defaults to VolMeshLayout.
 */
template <int kNumFacesPerCell,
          int kNumEdgesPerFace,
          template <int NumFacesPerCell, int NumEdgesPerFace> class LayoutPolicy = VolMeshLayout>
class VolMesh {
public:
  /// Layout of the volumetric mesh.
  using Layout = VolMeshLayout<kNumFacesPerCell, kNumEdgesPerFace>;

  /// Type of the cell.
  using CellType = typename Layout::CellType;

  /// Type of the half-face.
  using HalfFaceType = typename Layout::HalfFaceType;

  /**
   * @brief Default constructor.
   *
   * Initializes an empty VolMesh object.
   */
  VolMesh();

  /**
   * @brief Copy constructor.
   *
   * Creates a new VolMesh object as a copy of another VolMesh object.
   * @param rhs The VolMesh object to copy from.
   */
  explicit VolMesh(const VolMesh& rhs);

  /**
   * @brief Destructor for VolMesh.
   */
  virtual ~VolMesh();

  /**
   * @brief Copies the contents of another VolMesh object into this one.
   *
   * @param rhs The VolMesh object to copy from.
   */
  void copyFrom(const VolMesh& rhs);

  /**
   * @brief Clears all the contents of the VolMesh.
   */
  void clear();

  /**
   * @brief Returns the number of cells in the mesh.
   *
   * @return The number of cells.
   */
  uint32_t countCells() const;

  /**
   * @brief Returns the number of half-faces in the mesh.
   *
   * @return The number of half-faces.
   */
  uint32_t countHalfFaces() const;

  /**
   * @brief Returns the number of edges in the mesh.
   *
   * @return The number of edges.
   */
  uint32_t countEdges() const;

  /**
   * @brief Returns the number of half-edges in the mesh.
   *
   * @return The number of half-edges.
   */
  uint32_t countHalfEdges() const;

  /**
   * @brief Returns the number of vertices in the mesh.
   *
   * @return The number of vertices.
   */
  uint32_t countVertices() const;

  /**
   * @brief Returns the cell with the given cell ID.
   *
   * @param in_cell_id The ID of the cell.
   * @return A reference to the requested cell.
   */
  const CellType& cell(const CellIndex& in_cell_id) const;

  /**
   * @brief Returns the half-face with the given half-face ID.
   *
   * @param in_hface_id The ID of the half-face.
   * @return A reference to the requested half-face.
   */
  const HalfFaceType& halfFace(const HalfFaceIndex& in_hface_id) const;

  /**
   * @brief Returns the number of cells incident on a given half-face.
   *
   * @param in_hface_id The ID of the half-face.
   * @return The number of incident cells.
   */
  uint32_t countIncidentCellsPerHalfFace(const HalfFaceIndex& in_hface_id) const;

  /**
   * @brief Retrieves the cells incident on a given half-face.
   *
   * @param in_hface_id The ID of the half-face.
   * @param out_incident_cells A vector to store the IDs of the incident cells.
   * @return The number of incident cells.
   */
  uint32_t getIncidentCellsPerHalfFace(const HalfFaceIndex& in_hface_id,
                                        std::vector<CellIndex>& out_incident_cells) const;

  /**
   * @brief Returns the half-edge with the given half-edge ID.
   *
   * @param in_hedge_id The ID of the half-edge.
   * @return A reference to the requested half-edge.
   */
  const HalfEdge& halfEdge(const HalfEdgeIndex& in_hedge_id) const;

  /**
   * @brief Returns the vertex at the given vertex ID.
   *
   * @param in_vertex_id The ID of the vertex.
   * @return A vector representing the vertex coordinates.
   */
  const vec3 vertex(const VertexIndex& in_vertex_id) const;

  /**
   * @brief Inserts all vertices into the mesh.
   *
   * @param in_vertices A vector of vertices to insert.
   * @return True if successful, false otherwise.
   */
  bool insertAllVertices(const std::vector<vec3>& in_vertices);

  /**
   * @brief Inserts a half-edge into the mesh if it does not already exist.
   *
   * @param in_hedge The half-edge to insert.
   * @return The index of the inserted half-edge.
   */
  HalfEdgeIndex insertHalfEdgeIfNotExists(const HalfEdge& in_hedge);

  /**
   * @brief Inserts a half-face into the mesh if it does not already exist.
   *
   * @param in_hface The half-face to insert.
   * @return The index of the inserted half-face.
   */
  HalfFaceIndex insertHalfFaceIfNotExists(const HalfFaceType& in_hface);

  /**
   * @brief Inserts a cell into the mesh if it does not already exist.
   *
   * @param in_cell The cell to insert.
   * @return The index of the inserted cell.
   */
  CellIndex insertCellIfNotExists(const CellType& in_cell);

  /**
   * @brief Finds the half-edge in the mesh.
   *
   * @param in_hedge The half-edge to find.
   * @param out_hedge_id The ID of the found half-edge.
   * @return True if found, false otherwise.
   */
  bool findHalfEdge(const HalfEdge& in_hedge, HalfEdgeIndex& out_hedge_id) const;

  /**
   * @brief Finds the half-face in the mesh.
   *
   * @param in_hface The half-face to find.
   * @param out_hface_id The ID of the found half-face.
   * @return True if found, false otherwise.
   */
  bool findHalfFace(const HalfFaceType& in_hface, HalfFaceIndex& out_hface_id) const;

  /**
   * @brief Finds the cell in the mesh.
   *
   * @param in_cell The cell to find.
   * @param out_cell_id The ID of the found cell.
   * @return True if found, false otherwise.
   */
  bool findCell(const CellType& in_cell, CellIndex& out_cell_id) const;

private:
  /// Mutex for protecting the cells.
  mutable std::mutex cells_mutex_;

  /// Vector of all cells in the mesh.
  std::vector<CellType> cells_;

  /// Incident cells per half-face.
  std::vector<std::vector<uint32_t>> incident_cells_per_hface_;

  /// Mutex for protecting the half-faces.
  mutable std::mutex hfaces_mutex_;

  /// Vector of all half-faces in the mesh.
  std::vector<HalfFaceType> hfaces_;

  /// Incident half-faces per half-edge.
  std::vector<std::vector<uint32_t>> incident_hfaces_per_hedge_;

  /// Mutex for protecting the half-edges.
  mutable std::mutex hedges_mutex_;

  /// Vector of all half-edges in the mesh.
  std::vector<HalfEdge> hedges_;

  /// Incident half-edges per vertex.
  std::vector<std::vector<uint32_t>> incident_hedges_per_vertex_;

  /// Mutex for protecting the vertices.
  mutable std::mutex vertices_mutex_;

  /// Vector of all vertices in the mesh.
  std::vector<real_t> vertices_;
};

#include "volmesh/volmesh.tpp"

}