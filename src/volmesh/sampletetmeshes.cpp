#include "volmesh/sampletetmeshes.h"

namespace volmesh {

  void createOneTetrahedra(TetMesh& inout_mesh) {
	vector<double> vertices;
	vector<U32> elements;

	vec3d points[4];
	points[0] = vec3d(-1, 0, 0);
	points[1] = vec3d(0, 0, -2);
	points[2] = vec3d(1, 0, 0);
	points[3] = vec3d(0, 2, -1);

	vertices.resize(4 * 3);
	for (int i = 0; i < 4; i++) {
		points[i].store(&vertices[i * 3]);
	}

	elements.resize(4);
	elements[0] = 0;
	elements[1] = 1;
	elements[2] = 2;
	elements[3] = 3;

	VolMesh* tet = new VolMesh(vertices, elements);
	tet->setName("onetet");
	return tet;
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