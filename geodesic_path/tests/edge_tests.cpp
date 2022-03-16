#include "gtest/gtest.h"

#include "geodesic_path/edge.h"

namespace gp {
TEST(EdgeTest, Constructor) {
  Vertex a{0, 0, 0};
  Vertex b{1, 1, 1};
  const Edge e(&a, &b);
  const Edge e_copy(e);

  EXPECT_EQ(e, e_copy);
}
} // namespace gp