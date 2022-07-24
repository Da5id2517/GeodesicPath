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

inline double DotProduct(const Point &a, const Point &b) {
  return (a.x * b.x) + (a.y * b.y) + (a.z + b.z);
}

inline Point CrossProduct(const Point &a, const Point &b) {
  return {.x = a.y * b.z - a.z * b.y,
          .y = a.z * b.x - a.x * b.z,
          .z = a.x * b.y - a.y * b.x};
}

inline double Norm(const Point &vector) {
  return std::sqrt(DotProduct(vector, vector));
}

inline Point Normalize(const Point &p) {
  const auto norm = Norm(p);
  return {.x = p.x / norm, .y = p.y / norm, .z = p.z / norm};
}

inline int Signum(double value) { return (0.0 < value) - (0.0 > value); }

inline int Position(const Point &a, const Point &b, const Point &p) {
  auto AB = ToVector(a, b);
  auto AP = ToVector(a, p);
  return Signum((AB.y * AP.z - AB.z * AP.y) - (AB.x * AP.z - AB.z * AP.x) +
                (AB.x * AP.y - AB.y * AP.x));
}

} // namespace gp

#endif // GEODESIC_PATH_POINT_H
