//-----------------------------------------------------------------------------
// Copyright (c) Pourya Shirazian
// All rights reserved.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.
//-----------------------------------------------------------------------------

#include "volmesh/sampletetmeshes.h"
#include "volmesh/voxel.h"
#include "volmesh/mathutils.h"
#include "volmesh/tetrahedra.h"

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
      throw std::invalid_argument(fmt::format("Invalid grid dimension supplied [{} {} {}]", nx, ny, nz));
    }

    if(cellsize == (real_t)0.0) {
      throw std::invalid_argument(fmt::format("requested cellsize can not be zero [{}]", cellsize));
    }

    std::vector<vec3> vertex_list;
    std::vector<vec4i> tetcells_list;

    vec3 start = vec3(- static_cast<real_t>(nx)/2.0f, 0.0f, - static_cast<real_t>(nz)/2.0f) * cellsize;

    //add all nodes
    for(int i=0; i < nx; i++) {
      for(int j=0; j < ny; j++) {
        for(int k=0; k < nz; k++) {
          vec3 v = start + vec3(i, j, k) * cellsize;
          vertex_list.push_back(v);
        }
      }
    }

    int voxel_indices[8];

    //add all elements
    const int total_voxels = (nx - 1) * (ny - 1) * (nz - 1);
    tetcells_list.resize(total_voxels * 6);

    int voxel_id = 0;
    for(int i=0; i < nx-1; i++) {
      for(int j=0; j < ny-1; j++) {
        for(int k=0; k < nz-1; k++) {
          //compute voxel elements
          voxel_indices[Voxel::VertexLocationId::LBN] = i * ny * nz + j * nz + k;
          voxel_indices[Voxel::VertexLocationId::LBF] = i * ny * nz + j * nz + k + 1;

          voxel_indices[Voxel::VertexLocationId::LTN] = i * ny * nz + (j+1) * nz + k;
          voxel_indices[Voxel::VertexLocationId::LTF] = i * ny * nz + (j+1) * nz + k + 1;

          voxel_indices[Voxel::VertexLocationId::RBN] = (i+1) * ny * nz + j * nz + k;
          voxel_indices[Voxel::VertexLocationId::RBF] = (i+1) * ny * nz + j * nz + k + 1;

          voxel_indices[Voxel::VertexLocationId::RTN] = (i+1) * ny * nz + (j+1) * nz + k;
          voxel_indices[Voxel::VertexLocationId::RTF] = (i+1) * ny * nz + (j+1) * nz + k + 1;

          tetcells_list[voxel_id * 6]     = vec4i(voxel_indices[Voxel::VertexLocationId::LBN], voxel_indices[Voxel::VertexLocationId::LTN], voxel_indices[Voxel::VertexLocationId::RBN], voxel_indices[Voxel::VertexLocationId::LBF]);
          tetcells_list[voxel_id * 6 + 1] = vec4i(voxel_indices[Voxel::VertexLocationId::RTN], voxel_indices[Voxel::VertexLocationId::LTN], voxel_indices[Voxel::VertexLocationId::LBF], voxel_indices[Voxel::VertexLocationId::RBN]);
          tetcells_list[voxel_id * 6 + 2] = vec4i(voxel_indices[Voxel::VertexLocationId::RTN], voxel_indices[Voxel::VertexLocationId::LTN], voxel_indices[Voxel::VertexLocationId::LTF], voxel_indices[Voxel::VertexLocationId::LBF]);
          tetcells_list[voxel_id * 6 + 3] = vec4i(voxel_indices[Voxel::VertexLocationId::RTN], voxel_indices[Voxel::VertexLocationId::RBN], voxel_indices[Voxel::VertexLocationId::LBF], voxel_indices[Voxel::VertexLocationId::RBF]);
          tetcells_list[voxel_id * 6 + 4] = vec4i(voxel_indices[Voxel::VertexLocationId::RTN], voxel_indices[Voxel::VertexLocationId::LBF], voxel_indices[Voxel::VertexLocationId::LTF], voxel_indices[Voxel::VertexLocationId::RBF]);
          tetcells_list[voxel_id * 6 + 5] = vec4i(voxel_indices[Voxel::VertexLocationId::RTN], voxel_indices[Voxel::VertexLocationId::LTF], voxel_indices[Voxel::VertexLocationId::RTF], voxel_indices[Voxel::VertexLocationId::RBF]);

          voxel_id ++;
        }
      }
    }

    return inout_mesh.readFromList(vertex_list, tetcells_list);
  }

  bool createSphericalShell(TetMesh& inout_mesh,
                            const int horizontal_slices /* = 8 */,
                            const int vertical_slices /* = 8 */,
                            const real_t radius /* = 2.0 */,
                            const real_t shell_tickness /*= 0.3 */) {
    if(horizontal_slices <= 0 || vertical_slices <= 0) {
      throw std::invalid_argument(fmt::format("Either the horizontal or the vertical slice count is invalid. HS={}, VS={}",
                                  horizontal_slices,
                                  vertical_slices));
    }

    if(radius == (real_t)0.0 || shell_tickness == (real_t)0.0) {
      throw std::invalid_argument(fmt::format("Either the radius or the shell thickness is invalid. radius={}, shell_thickness={}",
                                  radius,
                                  shell_tickness));
    }

    const real_t inv_vslices = 1.0 / static_cast<real_t>(vertical_slices);
    const real_t inv_hslices = 1.0 / static_cast<real_t>(horizontal_slices);

    std::vector<vec3> vertex_list;
    vertex_list.reserve((vertical_slices + 1 ) * horizontal_slices * 2);

    //compute vertices on the outer surface of the shell
    for(int v=0; v < vertical_slices + 1; v++) {
      const real_t phi = volmesh::DegToRad(static_cast<real_t>(v) * inv_vslices * (real_t)180.0);

      for(int h=0; h < horizontal_slices; h++) {
        const real_t omega = volmesh::DegToRad(static_cast<real_t>(h) * inv_hslices * (real_t)360.0);

        //compute vertex compenents
        vec3 vertex(radius * sin(phi) * sin(omega),
                    radius * sin(phi) * cos(omega),
                    radius * cos(phi));

        vertex_list.push_back(vertex);
      }
    }

    const size_t outershell_start_vertex_id = vertex_list.size();

    //compute vertices on the inner surface of the shell
    const real_t inner_radius = radius - shell_tickness;
    for(int v=0; v < vertical_slices + 1; v++) {
      const real_t phi = volmesh::DegToRad(static_cast<real_t>(v) * inv_vslices * (real_t)180.0);

      for(int h=0; h < horizontal_slices; h++) {
        const real_t omega = volmesh::DegToRad(static_cast<real_t>(h) * inv_hslices * (real_t)360.0);

        //compute vertex compenents
        vec3 vertex(inner_radius * sin(phi) * sin(omega),
                    inner_radius * sin(phi) * cos(omega),
                    inner_radius * cos(phi));

        vertex_list.push_back(vertex);
      }
    }

    std::vector<vec4i> tetcells_list;
    tetcells_list.resize(vertical_slices * horizontal_slices * 6);

    int voxel_id = 0;

    //Indices
    int voxel_indices[8];

    //Add all elements
    for(int v=0; v < vertical_slices; v++) {
      for(int h=0; h < horizontal_slices; h++) {

        //top
        voxel_indices[Voxel::VertexLocationId::LTN] = h + v * horizontal_slices;
        voxel_indices[Voxel::VertexLocationId::LTF] = h + v * horizontal_slices + 1;
        voxel_indices[Voxel::VertexLocationId::RTN] = h + (v+1) * horizontal_slices;
        voxel_indices[Voxel::VertexLocationId::RTF] = h + (v+1) * horizontal_slices + 1;

        if(h == horizontal_slices - 1) {
          voxel_indices[Voxel::VertexLocationId::LTF] = v * horizontal_slices;
          voxel_indices[Voxel::VertexLocationId::RTF] = (v+1) * horizontal_slices;
        }

        //bottom
        voxel_indices[Voxel::VertexLocationId::LBN] = outershell_start_vertex_id + h + v * horizontal_slices;
        voxel_indices[Voxel::VertexLocationId::LBF] = outershell_start_vertex_id + voxel_indices[Voxel::VertexLocationId::LTN] + 1;
        voxel_indices[Voxel::VertexLocationId::RBN] = outershell_start_vertex_id + h + (v+1) * horizontal_slices;
        voxel_indices[Voxel::VertexLocationId::RBF] = outershell_start_vertex_id + voxel_indices[Voxel::VertexLocationId::RTN] + 1;

        if(h == horizontal_slices - 1) {
          voxel_indices[Voxel::VertexLocationId::LBF] = outershell_start_vertex_id + v * horizontal_slices;
          voxel_indices[Voxel::VertexLocationId::RBF] = outershell_start_vertex_id + (v+1) * horizontal_slices;
        }

        //add elements
        tetcells_list[voxel_id * 6]     = vec4i(voxel_indices[Voxel::VertexLocationId::LBN], voxel_indices[Voxel::VertexLocationId::LTN], voxel_indices[Voxel::VertexLocationId::RBN], voxel_indices[Voxel::VertexLocationId::LBF]);
        tetcells_list[voxel_id * 6 + 1] = vec4i(voxel_indices[Voxel::VertexLocationId::RTN], voxel_indices[Voxel::VertexLocationId::LTN], voxel_indices[Voxel::VertexLocationId::LBF], voxel_indices[Voxel::VertexLocationId::RBN]);
        tetcells_list[voxel_id * 6 + 2] = vec4i(voxel_indices[Voxel::VertexLocationId::RTN], voxel_indices[Voxel::VertexLocationId::LTN], voxel_indices[Voxel::VertexLocationId::LTF], voxel_indices[Voxel::VertexLocationId::LBF]);
        tetcells_list[voxel_id * 6 + 3] = vec4i(voxel_indices[Voxel::VertexLocationId::RTN], voxel_indices[Voxel::VertexLocationId::RBN], voxel_indices[Voxel::VertexLocationId::LBF], voxel_indices[Voxel::VertexLocationId::RBF]);
        tetcells_list[voxel_id * 6 + 4] = vec4i(voxel_indices[Voxel::VertexLocationId::RTN], voxel_indices[Voxel::VertexLocationId::LBF], voxel_indices[Voxel::VertexLocationId::LTF], voxel_indices[Voxel::VertexLocationId::RBF]);
        tetcells_list[voxel_id * 6 + 5] = vec4i(voxel_indices[Voxel::VertexLocationId::RTN], voxel_indices[Voxel::VertexLocationId::LTF], voxel_indices[Voxel::VertexLocationId::RTF], voxel_indices[Voxel::VertexLocationId::RBF]);

        voxel_id ++;
      }
    }

    //remove slivers or tetrahedral elements with volume less than a threshold
    static const real_t kMinVolumeThreshold = (real_t)0.0001;
    size_t i = 0;
    while(i < tetcells_list.size()) {
      vec4i vertex_ids = tetcells_list[i];
      Tetrahedra::TetraVertexArray tet_vertex_array4;
      tet_vertex_array4.col(0) = vertex_list[vertex_ids.coeff(0)];
      tet_vertex_array4.col(1) = vertex_list[vertex_ids.coeff(1)];
      tet_vertex_array4.col(2) = vertex_list[vertex_ids.coeff(2)];
      tet_vertex_array4.col(3) = vertex_list[vertex_ids.coeff(3)];

      Tetrahedra t(tet_vertex_array4);
      if(t.volume() < kMinVolumeThreshold) {
        tetcells_list.erase(tetcells_list.begin() + i);
      } else {
        i++;
      }
    }

    return inout_mesh.readFromList(vertex_list, tetcells_list);
  }

}