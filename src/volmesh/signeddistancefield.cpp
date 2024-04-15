#include "volmesh/signeddistancefield.h"

using namespace volmesh;

SignedDistanceField::SignedDistanceField() {

}

SignedDistanceField::SignedDistanceField(const SignedDistanceField& rhs) {
  copyFrom(rhs);
}

SignedDistanceField::~SignedDistanceField() {

}

AABB SignedDistanceField::bounds() const {
  return bounds_;
}

real_t SignedDistanceField::spacing() const {
  return spacing_;
}

bool SignedDistanceField::generate(const TriangleMesh& in_mesh,
                                   const vec3& expansion,
                                   real_t spacing) {
  //computes a bounding box that contains the triangle mesh
  bounds_ = in_mesh.bounds();
  bounds_.expand(expansion);
}

real_t SignedDistanceField::compute(const vec3& p) const {

}

SignedDistanceField& SignedDistanceField::operator=(const SignedDistanceField& rhs) {
  copyFrom(rhs);
  return *this;
}