#ifndef RSPROJECT_UTILTYPES_H
#define RSPROJECT_UTILTYPES_H


//used in Vertex
struct point_t
{
    double x;
    double y;
    double z;
};

//used in Edge
struct indexPair_t
{
    int first;
    int second;
};

//used in Triangle
struct indexTriple_t
{
    int first;
    int second;
    int third;
};

//used in SparseMatrix
struct rowColumnValue_t {
    int row;
    int column;
    int value;
};

bool operator == (const point_t &lhs, const point_t &rhs);

//  (i, j) is equal to (i, j) but also to (j, i)
bool operator == (const indexPair_t &lhs, const indexPair_t &rhs);

bool operator == (const rowColumnValue_t &lhs, const rowColumnValue_t &rhs);

#endif //RSPROJECT_UTILTYPES_H
