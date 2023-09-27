#include "volmesh/sampletetmeshes.h"

#include <vector>

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

  }

  bool createTruthCube(TetMesh& inout_mesh,
                       const int nx,
                       const int ny,
                       const int nz,
                       const double cellsize) {

  }

  bool createSphericalShell(TetMesh& inout_mesh,
                            const int horizontal_slices /* = 8 */,
                            const int vertical_slices /* = 8 */,
                            const double radius /* = 2.0 */,
                            const double shell_tickness /*= 0.3 */) {

  }

}