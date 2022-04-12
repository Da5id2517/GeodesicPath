#ifndef GEODESIC_PATH_EDGE_FLIP_H
#define GEODESIC_PATH_EDGE_FLIP_H

#include "mesh.h"

namespace gp {
Mesh FlipOut(const Mesh &mesh, std::vector<int> &path);
}

#endif // GEODESIC_PATH_EDGE_FLIP_H
