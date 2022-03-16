#include "face.h"

namespace gp {
Face::Face(std::vector<Point> &vertices) : vertices_(std::move(vertices)) {}

} // namespace gp