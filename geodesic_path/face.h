#ifndef GEODESIC_PATH_FACE_H
#define GEODESIC_PATH_FACE_H

#include <vector>

#include "point.h"

namespace gp {

class Face {
public:
  Face();
  explicit Face(std::vector<Point> &vertices);

  const std::vector<Point> &Vertices() const { return vertices_; }

private:
  std::vector<Point> vertices_{};
};

bool operator==(const Face &lhs, const Face &rhs) {
  return lhs.Vertices() == rhs.Vertices();
}

} // namespace gp

#endif // GEODESIC_PATH_FACE_H
