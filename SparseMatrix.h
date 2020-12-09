#ifndef RSPROJECT_SPARSEMATRIX_H
#define RSPROJECT_SPARSEMATRIX_H
#include "DenseMatrix.h"


class SparseMatrix
{
public:
    SparseMatrix(DenseMatrix matrix);
    std::vector<std::vector<int>> getData();

private:
    std::vector<std::vector<int>> data;
};

#endif //RSPROJECT_SPARSEMATRIX_H
