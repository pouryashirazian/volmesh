#include "volmesh/tetmesh.h"

namespace volmesh {

  void createOneTetrahedra(TetMesh& inout_mesh);
  void createTwoTetrahedra(TetMesh& inout_mesh);

  bool createTruthCube(TetMesh& inout_mesh,
                       const int nx,
                       const int ny,
                       const int nz,
                       const double cellsize);

  bool createSphericalShell(TetMesh& inout_mesh,
                            const int horizontal_slices = 8,
                            const int vertical_slices = 8,
                            const double radius = 2.0,
                            const double shell_tickness = 0.3);
}