#ifndef GEODESIC_PATH_POINT_H
#define GEODESIC_PATH_POINT_H

#include <cmath>

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

inline Point ToVector(const Point &a, const Point &b) {
  return {.x = b.x - a.x, .y = b.y - a.y, .z = b.z - a.z};
}

inline double SquaredLength(const Point &a) {
  return (a.x * a.x) + (a.y * a.y) + (a.z * a.z);
}

inline double Distance(const Point &a, const Point &b) {
  return std::sqrt(SquaredLength(ToVector(a, b)));
}

inline Point CrossProduct(const Point &a, const Point &b) {
  return {.x = a.y * b.z - a.z * b.y,
          .y = a.z * b.x - a.x * b.z,
          .z = a.x * b.y - a.y * b.x};
}

} // namespace gp

#endif // GEODESIC_PATH_POINT_H
