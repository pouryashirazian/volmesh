#pragma once

#include <Eigen/Dense>

namespace volmesh {
  #ifdef VOLMESH_USE_FLT64
    typedef double real_t;
    typedef Eigen::Vector2d vec2;
    typedef Eigen::Vector3d vec3;
    typedef Eigen::Vector4d vec4;
  #else
    typedef float real_t;
    typedef Eigen::Vector2f vec2;
    typedef Eigen::Vector3f vec3;
    typedef Eigen::Vector4f vec4;
  #endif

  typedef Eigen::Vector4i vec4i;
}