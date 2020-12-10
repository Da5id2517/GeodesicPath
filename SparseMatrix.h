#ifndef RSPROJECT_SPARSEMATRIX_H
#define RSPROJECT_SPARSEMATRIX_H
#include "DenseMatrix.h"


class SparseMatrix
{
public:
    SparseMatrix(DenseMatrix matrix);
    std::vector<std::tuple<int, int, int>> getData();

    int operator()(int i, int j);

private:
    // Values in data are (row, column, value)
    std::vector<std::tuple<int,int,int>> data;
    int rows, columns;
};

#endif //RSPROJECT_SPARSEMATRIX_H
