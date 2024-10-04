//-----------------------------------------------------------------------------
// Copyright (c) Pourya Shirazian
// All rights reserved.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.
//-----------------------------------------------------------------------------

#pragma once

#include "volmesh/basetypes.h"

#include <array>
#include <stdint.h>

namespace volmesh {

  class Voxel {
  public:
    static const int kNumVerticesPerCell = 8;
    static const int kNumFaces = 6;
    static const int kNumEdges = 12;
    static const int kNumFittingTetrahedra = 6;

    typedef Eigen::Matrix<real_t, 3, kNumVerticesPerCell> VoxelVertexArray;

    //Vertex location Id
    enum VertexLocationId : int {
      LBN = 0, LBF = 1,
      LTN = 2, LTF = 3,
      RBN = 4, RBF = 5,
      RTN = 6, RTF = 7
    };

    enum VoxelAxis : int {
      X = 0,
      Y = 1,
      Z = 2,
    };

    Voxel();
    Voxel(const Voxel& rhs);
    explicit Voxel(const VoxelVertexArray& vertices);
    ~Voxel();

    const VoxelVertexArray& vertices() const;

    vec3 vertex(const int vertex_id) const;

    real_t axisLength(const VoxelAxis axis) const;

    real_t surfaceArea() const;

    vec3 centroid() const;

    real_t volume() const;

    static Voxel CreateVoxel(const vec3& center,
                             const vec3& axis_lengths);

  private:
    VoxelVertexArray vertices_;
  };

}