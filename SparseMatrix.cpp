#include "SparseMatrix.h"

SparseMatrix::SparseMatrix(DenseMatrix matrix)
{
    int size = 0;
    data.resize(3);

    for(auto row : matrix.getData())
    {
        size += std::count_if(std::begin(row), std::end(row), [](auto x){return x != 0;});
    }

    for(auto & array : data)
    {
        array.resize(size);
    }

    int k = 0;
    for(int i = 0; i < matrix.getRows(); i ++)
    {
        for(int j = 0; j < matrix.getColumns(); j++)
        {
            if(matrix(i, j) != 0)
            {
                data[0][k]= i;
                data[1][k] = j;
                data[2][k] = 1;
                k++;
            }
        }
    }
}

std::vector<std::vector<int>> SparseMatrix::getData()
{
    return this->data;
}


