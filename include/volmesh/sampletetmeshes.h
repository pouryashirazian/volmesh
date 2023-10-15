#include "volmesh/tetmesh.h"

namespace volmesh {

  void createOneTetrahedra(TetMesh& inout_mesh);
  void createTwoTetrahedra(TetMesh& inout_mesh);

  bool createVoxelGrid(TetMesh& inout_mesh,
                       const int nx,
                       const int ny,
                       const int nz,
                       const real_t cellsize);

  bool createSphericalShell(TetMesh& inout_mesh,
                            const int horizontal_slices = 8,
                            const int vertical_slices = 8,
                            const real_t radius = 2.0,
                            const real_t shell_tickness = 0.3);
}