//-----------------------------------------------------------------------------
// Copyright (c) Pourya Shirazian
// All rights reserved.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.
//-----------------------------------------------------------------------------

#include "volmesh/tetmesh.h"

namespace volmesh {

/**
 * @brief Creates a single tetrahedron and adds it to the input/output tetrahedral mesh.
 *
 * This function generates one tetrahedron and appends it to the given `TetMesh`.
 *
 * @param inout_mesh The tetrahedral mesh to which the tetrahedron will be added.
 */
void createOneTetrahedra(TetMesh& inout_mesh);

/**
 * @brief Creates two tetrahedra and adds them to the input/output tetrahedral mesh.
 *
 * This function generates two tetrahedra and appends them to the given `TetMesh`.
 *
 * @param inout_mesh The tetrahedral mesh to which the tetrahedra will be added.
 */
void createTwoTetrahedra(TetMesh& inout_mesh);

/**
 * @brief Creates a voxel grid of tetrahedra and adds it to the input/output tetrahedral mesh.
 *
 * This function generates a voxel grid of tetrahedra based on the specified grid dimensions
 * and cell size, and adds the grid to the given `TetMesh`.
 *
 * @param inout_mesh The tetrahedral mesh to which the voxel grid will be added.
 * @param nx Number of cells along the x-axis.
 * @param ny Number of cells along the y-axis.
 * @param nz Number of cells along the z-axis.
 * @param cellsize The size of each voxel cell.
 * @return True if the voxel grid was successfully created, otherwise false.
 */
bool createVoxelGrid(TetMesh& inout_mesh,
                     const int nx,
                     const int ny,
                     const int nz,
                     const real_t cellsize);

/**
 * @brief Creates a spherical shell of tetrahedra and adds it to the input/output tetrahedral mesh.
 *
 * This function generates a spherical shell of tetrahedra with the specified number of
 * horizontal and vertical slices, radius, and shell thickness, and adds it to the given `TetMesh`.
 *
 * @param inout_mesh The tetrahedral mesh to which the spherical shell will be added.
 * @param horizontal_slices The number of horizontal slices for the shell (default is 8).
 * @param vertical_slices The number of vertical slices for the shell (default is 8).
 * @param radius The radius of the spherical shell (default is 2.0).
 * @param shell_tickness The thickness of the spherical shell (default is 0.3).
 * @return True if the spherical shell was successfully created, otherwise false.
 */
bool createSphericalShell(TetMesh& inout_mesh,
                          const int horizontal_slices = 8,
                          const int vertical_slices = 8,
                          const real_t radius = 2.0,
                          const real_t shell_tickness = 0.3);

}