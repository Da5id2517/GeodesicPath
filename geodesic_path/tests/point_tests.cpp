#include "gtest/gtest.h"

#include "geodesic_path/point.h"

namespace gp {
TEST(Point, EqualityOperator) {
  const Point p{0, 0, 0};
  const Point q{-1, -1, -1};
  const Point p_copy{p};

  EXPECT_EQ(p, p_copy);
  EXPECT_NE(p, q);
}

TEST(Point, Distance) {
  const Point p{0, 0, 0};
  const Point p_copy{p};
  const Point q{5, 0, 0};

  EXPECT_EQ(Distance(p, p_copy), 0);
  EXPECT_EQ(Distance(p, q), 5);
}

TEST(Point, CrossProduct) {
  const Point p{5, 0, 0};
  const Point q{0, 4, 0};
  const Point zero{0, 0, 0};
  const Point expected_pq{0, 0, 20};

  EXPECT_EQ(CrossProduct(p, p), zero);
  EXPECT_EQ(CrossProduct(p, zero), zero);
  EXPECT_EQ(CrossProduct(p, q), expected_pq);
}

TEST(Point, Position) {
  const Point a{5, 0, 0};
  const Point b{0, 0, 0};
  const Point p{1, 2, 0};
  const Point q{1, -2, 0};
  const Point r{15, 1, 0};

  EXPECT_EQ(Position(a, b, a), 0);
  EXPECT_EQ(Position(a, b, b), 0);
  EXPECT_EQ(Position(a, b, r), Position(a, b, p));
  EXPECT_NE(Position(a, b, p), Position(a, b, q));
  EXPECT_NE(Position(a, b, r), Position(a, b, q));
}

} // namespace gp