#include "gtest/gtest.h"

#include "geodesic_path/vertex.h"

namespace gp {
TEST(Point, EqualityOperator) {
  const Point p{0, 0, 0};
  const Point q{-1, -1, -1};
  const Point p_copy{0, 0, 0};

  EXPECT_EQ(p, p_copy);
  EXPECT_NE(p, q);
}

} // namespace gp