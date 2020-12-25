#ifndef RSPROJECT_SPARSEMATRIX_H
#define RSPROJECT_SPARSEMATRIX_H
#include "DenseMatrix.h"
#include "utilTypes.h"

class SparseMatrix
{
public:
    SparseMatrix();
    SparseMatrix(DenseMatrix matrix);
    std::vector<rowColumnValue_t> getData();
    int getRows() const;
    int getColumns() const;


    int operator()(int i, int j);
    std::vector<int> getColumnIndicesWithinRow(int i);
    std::vector<int> getRowIndicesWithinColumn(int j);

private:
    // Values in data are (row, column, value)
    std::vector<rowColumnValue_t> data;
    int rows{}, columns{};
};


#endif //RSPROJECT_SPARSEMATRIX_H
