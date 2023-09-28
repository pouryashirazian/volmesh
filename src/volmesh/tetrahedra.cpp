
#include "volmesh/tetrahedra.h"

using namespace volmesh;

Tetrahedra::Tetrahedra() {

}

Tetrahedra::Tetrahedra(const Tetrahedra& rhs): vertices_(rhs.vertices_) {

}

Tetrahedra::Tetrahedra(const Tetrahedra::VertexArray4& vertices): vertices_(vertices) {

}

Tetrahedra::~Tetrahedra() {

}

const Tetrahedra::VertexArray4& Tetrahedra::vertices() const {
  return vertices_;
}

vec3 Tetrahedra::vertex(const int vertex_id) const {
  return vertices_.col(vertex_id);
}

real_t Tetrahedra::surfaceArea() const {
  //The surface area of a Tetrahedra is the sum of the surface area of its four triangular faces.
  //Per each triangle with vertices a,b,c the area is calculated as: 0.5 * cross(ab, ac)
  real_t area = 0.0;

  for(int i=0; i < kNumFaces; i++) {
    vec3i face_indices = faceIndices(i);
    vec3 a = vertices_.col(face_indices.x());
    vec3 b = vertices_.col(face_indices.y());
    vec3 c = vertices_.col(face_indices.z());
    vec3 cross = (b - a).cross(c - a);

    area += 0.5 * cross.norm();
  }

  return area;
}

real_t Tetrahedra::inradius() const {
  return (3.0 * volume()) / surfaceArea();
}

real_t Tetrahedra::circumradius() const {
	vec4 one(1.0, 1.0, 1.0, 1.0);

  vec4 len2;
	for(int i=0;i < kNumVerticesPerCell; i++) {
		len2[i] = vertices_.col(i).squaredNorm();
  }

	//columns
	vec4 cx = vertices_.row(0);
	vec4 cy = vertices_.row(1);
	vec4 cz = vertices_.row(2);

	real_t dx = 0.0;
  real_t dy = 0.0;
  real_t dz = 0.0;
  real_t a = 0.0;
  real_t c = 0.0;

	//mdx
	{
		mat4 mdx;
		mdx.col(0) = len2;
		mdx.col(1) = cy;
		mdx.col(2) = cz;
		mdx.col(3) = one;
		dx = mdx.determinant();
	}

	//mdy
	{
		mat4 mdy;
		mdy.col(0) = len2;
		mdy.col(1) = cx;
		mdy.col(2) = cz;
		mdy.col(3) = one;
		dy = -1.0 * mdy.determinant();
	}

	//mdz
	{
		mat4 mdz;
		mdz.col(0) = len2;
		mdz.col(1) = cx;
		mdz.col(2) = cy;
		mdz.col(3) = one;
		dz = mdz.determinant();
	}

	//a
	{
		mat4 mda;
		mda.col(0) = cx;
		mda.col(1) = cy;
		mda.col(2) = cz;
		mda.col(3) = one;
		a = mda.determinant();
	}

	//c
	{
		mat4 mdc;
		mdc.col(0) = len2;
		mdc.col(1) = cx;
		mdc.col(2) = cy;
		mdc.col(3) = cz;
		c = mdc.determinant();
	}

	return sqrt(dx*dx + dy*dy + dz*dz - 4*a*c) / 2.0 * fabs(a);
}

real_t Tetrahedra::aspectRatio() const {
  //aspectRatio = cr / (3.0 x ir)
  return circumradius() / (3.0 * inradius());
}

vec3 Tetrahedra::centroid() const {
  static const real_t ratio = 1.0 / static_cast<real_t>(kNumVerticesPerCell);

  vec3 sum = vertices_.col(0);
  for(int i=1; i < kNumVerticesPerCell; i++) {
    sum = sum + vertices_.col(i);
  }
  return sum * ratio;
}

real_t Tetrahedra::determinant() const {
  const vec3 ab = vertices_.col(1) - vertices_.col(0);
  const vec3 ac = vertices_.col(2) - vertices_.col(0);
  const vec3 ad = vertices_.col(3) - vertices_.col(0);

  return ab.dot(ac.cross(ad));
}

real_t Tetrahedra::volume() const {
  return static_cast<real_t>(1.0 / 6.0) * fabs(determinant());
}

vec3i Tetrahedra::faceIndices(const int face_id) {
  static const vec3i faceIndicesLut[kNumFaces] = { {1, 2, 3}, {2, 0, 3}, {3, 0, 1}, {1, 0, 2} };

  if(face_id >= 0 && face_id < kNumFaces) {
    return faceIndicesLut[face_id];
  } else {
    throw std::out_of_range("The supplied face index is out of range");
  }
}

vec2i Tetrahedra::edgeIndices(const int edge_id) {
  static const vec2i edgeIndicesLut[kNumEdges] = { {1, 2}, {2, 3}, {3, 1}, {2, 0}, {0, 3}, {0, 1} };

  if(edge_id >=0 && edge_id < kNumEdges) {
    return edgeIndicesLut[edge_id];
  } else {
    throw std::out_of_range("The supplied edge index is out of range");
  }
}
