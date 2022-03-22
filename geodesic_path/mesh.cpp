#include "mesh.h"

namespace gp {

double Mesh::Area() const {
  double area = 0.0;
  for (const auto &triangle : triangles_) {
    area += gp::Area(triangle);
  }
  return area;
}
} // namespace gp