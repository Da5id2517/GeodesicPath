#ifndef RSPROJECT_DENSEMATRIX_H
#define RSPROJECT_DENSEMATRIX_H

#include <vector>

class DenseMatrix
{
public:
    DenseMatrix(std::vector<std::vector<int>> data = {});
    DenseMatrix(int row, int column);

    int getRows() const;
    int getColumns() const;

    int& operator()(int row, int column);
    int operator()(int row, int column) const;

    std::vector<std::vector<int>>& getData();



private:
    int rows, columns;
    std::vector<std::vector<int>> data;

};


#endif //RSPROJECT_DENSEMATRIX_H
