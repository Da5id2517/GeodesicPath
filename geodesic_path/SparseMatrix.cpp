#include "SparseMatrix.h"


SparseMatrix::SparseMatrix()
{
    this->data = {};
}

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
    for(int i = 0; i < rows; i ++)
    {
        for(int j = 0; j < columns; j++)
        {
            auto value = matrix(i,j);
            if(value != 0)
            {
                data[k] = {
                        i,      //row
                        j,      //column
                        value   //value
                };
                k++;
            }
        }
    }
}

int SparseMatrix::getRows() const
{
    return this->rows;
}

int SparseMatrix::getColumns() const
{
    return this->columns;
}

std::vector<rowColumnValue_t> SparseMatrix::getData()
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
        if(tuple.row == i && tuple.column == j)
        {
            return tuple.value;
        }
    }
    return 0;
}

std::vector<int> SparseMatrix::getColumnIndicesWithinRow(int i)
{
    std::vector<int> result;
    for(auto tuple : data)
    {
        if(tuple.row == i)
        {
            result.push_back(tuple.column);
        }
    }
    return result;
}

std::vector<int> SparseMatrix::getRowIndicesWithinColumn(int j)
{
    std::vector<int> result;
    for(auto tuple : data)
    {
        if(tuple.column == j)
        {
            result.push_back(tuple.row);
        }
    }
    return result;
}
