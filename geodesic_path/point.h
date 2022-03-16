#ifndef GEODESIC_PATH_POINT_H
#define GEODESIC_PATH_POINT_H

namespace gp {

struct Point {
  double x{-1.0};
  double y{-1.0};
  double z{-1.0};
};

inline bool operator==(const Point &a, const Point &b) {
  return a.x == b.x && a.y == b.y && a.z == b.z;
}

inline bool operator!=(const Point &a, const Point &b) { return !(a == b); }

} // namespace gp

#endif // GEODESIC_PATH_POINT_H
