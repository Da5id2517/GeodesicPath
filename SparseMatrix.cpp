#include "SparseMatrix.h"

SparseMatrix::SparseMatrix(DenseMatrix matrix)
{
    // use rows and columns to check if index is out of bounds.
    this->rows = matrix.getRows();
    this->columns = matrix.getColumns();

    int non_null_values = 0;

    for(auto row : matrix.getData())
    {
        non_null_values += std::count_if(std::begin(row), std::end(row), [](auto x){return x != 0;});
    }

    data.resize(non_null_values);

    int k = 0;
    for(int i = 0; i < matrix.getRows(); i ++)
    {
        for(int j = 0; j < matrix.getColumns(); j++)
        {
            auto value = matrix(i,j);
            if(value != 0)
            {
                data[k] = std::make_tuple(i, j, value);
                k++;
            }
        }
    }
}

std::vector<std::tuple<int, int, int>> SparseMatrix::getData()
{
    return this->data;
}

int SparseMatrix::operator()(int i, int j)
{
    if(i >= rows || j >= columns)
    {
        throw std::invalid_argument("Index out of bounds.");
    }

    for(auto tuple : data)
    {
        if(std::get<0>(tuple) == i && std::get<1>(tuple) == j)
        {
            return std::get<2>(tuple);
        }
    }
    return 0;
}

//TODO: remove later
SparseMatrix::SparseMatrix()
{
    this->rows = 0;
    this->columns = 0;
    this->data = {};
}
