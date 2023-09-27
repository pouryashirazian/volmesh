#include "volmesh/tetrahedra.h"

#include <gtest/gtest.h>

using namespace volmesh;

TEST(Tetrahedra, Constructor) {
  Tetrahedra::VertexArray vertices{
    vec3({-1, 0, 0}),
    vec3({0, 0, -2}),
    vec3({1, 0, 0}),
    vec3({0, 2, -1})
  };

  Tetrahedra t(vertices);

  ASSERT_EQ(t.vertex(0), vertices[0]);
  ASSERT_EQ(t.vertex(1), vertices[1]);
  ASSERT_EQ(t.vertex(2), vertices[2]);
  ASSERT_EQ(t.vertex(3), vertices[3]);
}