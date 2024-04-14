#include "volmesh/aabb.h"

using namespace volmesh;

AABB::AABB() {

}

AABB::AABB(const vec3& lower, const vec3& upper):lower_(lower), upper_(upper) {

}

AABB::AABB(const AABB& rhs):lower_(rhs.lower_), upper_(rhs.upper_) {

}

AABB::~AABB() {

}

vec3 AABB::lower() const {
  return lower_;
}

vec3 AABB::upper() const {
  return upper_;
}

vec3 AABB::extent() const {
  return upper_ - lower_;
}

void AABB::expand(const real_t dx, const real_t dy, const real_t dz) {
  lower_[0] -= dx * 0.5;
  lower_[1] -= dy * 0.5;
  lower_[2] -= dz * 0.5;

  upper_[0] += dx * 0.5;
  upper_[1] += dy * 0.5;
  upper_[2] += dz * 0.5;
}

void AABB::expand(const vec3& expansion) {
  expand(expansion.x(), expansion.y(), expansion.z());
}

bool AABB::intersects(const AABB& rhs) const {
  return ((lower_.x() >= rhs.lower_.x()) && (lower_.x() <= rhs.upper_.x()))||
         ((lower_.y() >= rhs.lower_.y()) && (lower_.y() <= rhs.upper_.y()))||
         ((lower_.z() >= rhs.lower_.z()) && (lower_.z() <= rhs.upper_.z()));
}

AABB& AABB::operator=(const AABB& rhs) {
  lower_ = rhs.lower_;
  upper_ = rhs.upper_;

  return *this;
}
