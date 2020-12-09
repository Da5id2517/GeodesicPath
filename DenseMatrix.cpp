#include "DenseMatrix.h"

DenseMatrix::DenseMatrix(std::vector<std::vector<int>> data)
{
    this->data = data;
    this->rows = data.size();
    if(rows)
    {
        this->columns = data[0].size();
    }
    else
    {
        this->columns = 0;
    }
}

int DenseMatrix::getColumns() const
{
    return this->columns;
}

int DenseMatrix::getRows() const
{
    return this->rows;
}

int & DenseMatrix::operator()(int row, int column)
{
    if(row > rows || column > columns)
    {
        throw std::invalid_argument("Index out of bounds");
    }
    return data[row][column];
}

int DenseMatrix::operator()(int row, int column) const
{
    if(row > rows || column > columns)
    {
        throw std::invalid_argument("Index out of bounds");
    }
    return data[row][column];
}

std::vector<std::vector<int>> & DenseMatrix::getData()
{
    return this->data;
}


