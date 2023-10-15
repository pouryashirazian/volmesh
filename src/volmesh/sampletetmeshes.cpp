#include "volmesh/sampletetmeshes.h"
#include "volmesh/cube.h"

#include <vector>
#include <iostream>
#include <fmt/core.h>

namespace volmesh {

  void createOneTetrahedra(TetMesh& inout_mesh) {
    std::vector<vec3> vertices = {
      {-1, 0, 0},
      {0, 0, -2},
      {1, 0, 0},
      {0, 2, -1}
    };

    std::vector<vec4i> cells = {
      {0, 1, 2, 3}
    };

    inout_mesh.readFromList(vertices, cells);
  }


  void createTwoTetrahedra(TetMesh& inout_mesh) {
    std::vector<vec3> vertices = {
      {-1, 0, 0},
      {1, 0, 0},
      {0, 0, -1},
      {0, 0, 1},
      {0, 2, 0}
    };

    std::vector<vec4i> cells = {
      {0, 2, 3, 4},
      {1, 2, 3, 4}
    };

    inout_mesh.readFromList(vertices, cells);
  }

  bool createVoxelGrid(TetMesh& inout_mesh,
                       const int nx,
                       const int ny,
                       const int nz,
                       const real_t cellsize) {
    if(nx < 2 || ny < 2 || nz < 2) {
      std::cerr << fmt::format("Invalid grid dimension supplied [{} {} {}]", nx, ny, nz) << std::endl;
      return false;
    }

    std::vector<vec3> vertices;
    std::vector<vec4i> tet_cells;

	  vec3 start = vec3(- static_cast<real_t>(nx)/2.0f, 0.0f, - static_cast<real_t>(nz)/2.0f) * cellsize;

    //add all nodes
    for(int i=0; i < nx; i++) {
      for(int j=0; j < ny; j++) {
        for(int k=0; k < nz; k++) {
          vec3 v = start + vec3(i, j, k) * cellsize;
          vertices.push_back(v);
        }
      }
    }

    int cube_indices[8];

    //add all elements
    const int total_voxels = (nx - 1) * (ny - 1) * (nz - 1);
    tet_cells.resize(total_voxels * 6);

    int voxel_id = 0;
    for(int i=0; i < nx-1; i++) {
      for(int j=0; j < ny-1; j++) {
        for(int k=0; k < nz-1; k++) {
          //compute voxel elements
          cube_indices[Cube::VertexLocationId::LBN] = i * ny * nz + j * nz + k;
          cube_indices[Cube::VertexLocationId::LBF] = i * ny * nz + j * nz + k + 1;

          cube_indices[Cube::VertexLocationId::LTN] = i * ny * nz + (j+1) * nz + k;
          cube_indices[Cube::VertexLocationId::LTF] = i * ny * nz + (j+1) * nz + k + 1;

          cube_indices[Cube::VertexLocationId::RBN] = (i+1) * ny * nz + j * nz + k;
          cube_indices[Cube::VertexLocationId::RBF] = (i+1) * ny * nz + j * nz + k + 1;

          cube_indices[Cube::VertexLocationId::RTN] = (i+1) * ny * nz + (j+1) * nz + k;
          cube_indices[Cube::VertexLocationId::RTF] = (i+1) * ny * nz + (j+1) * nz + k + 1;

          tet_cells[voxel_id * 6]     = vec4i(cube_indices[Cube::VertexLocationId::LBN], cube_indices[Cube::VertexLocationId::LTN], cube_indices[Cube::VertexLocationId::RBN], cube_indices[Cube::VertexLocationId::LBF]);
          tet_cells[voxel_id * 6 + 1] = vec4i(cube_indices[Cube::VertexLocationId::RTN], cube_indices[Cube::VertexLocationId::LTN], cube_indices[Cube::VertexLocationId::LBF], cube_indices[Cube::VertexLocationId::RBN]);
          tet_cells[voxel_id * 6 + 2] = vec4i(cube_indices[Cube::VertexLocationId::RTN], cube_indices[Cube::VertexLocationId::LTN], cube_indices[Cube::VertexLocationId::LTF], cube_indices[Cube::VertexLocationId::LBF]);
          tet_cells[voxel_id * 6 + 3] = vec4i(cube_indices[Cube::VertexLocationId::RTN], cube_indices[Cube::VertexLocationId::RBN], cube_indices[Cube::VertexLocationId::LBF], cube_indices[Cube::VertexLocationId::RBF]);
          tet_cells[voxel_id * 6 + 4] = vec4i(cube_indices[Cube::VertexLocationId::RTN], cube_indices[Cube::VertexLocationId::LBF], cube_indices[Cube::VertexLocationId::LTF], cube_indices[Cube::VertexLocationId::RBF]);
          tet_cells[voxel_id * 6 + 5] = vec4i(cube_indices[Cube::VertexLocationId::RTN], cube_indices[Cube::VertexLocationId::LTF], cube_indices[Cube::VertexLocationId::RTF], cube_indices[Cube::VertexLocationId::RBF]);

          voxel_id ++;
        }
      }
    }

	  return inout_mesh.readFromList(vertices, tet_cells);
  }

  bool createSphericalShell(TetMesh& inout_mesh,
                            const int horizontal_slices /* = 8 */,
                            const int vertical_slices /* = 8 */,
                            const double radius /* = 2.0 */,
                            const double shell_tickness /*= 0.3 */) {

  }

}