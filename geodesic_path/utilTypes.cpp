#include "utilTypes.h"

bool operator == (const point_t &lhs, const point_t &rhs)
{
    return lhs.x == rhs.x && lhs.y == rhs.y && lhs.z == rhs.z;
}

bool operator == (const indexPair_t &lhs, const indexPair_t &rhs)
{
    if(lhs.first == rhs.first && lhs.second == rhs.second)
    {
        return true;
    }
    if(lhs.first == rhs.second && lhs.second == rhs.first)
    {
        return true;
    }
    return false;
}

bool operator == (const rowColumnValue_t &lhs, const rowColumnValue_t &rhs)
{
    return lhs.row == rhs.row && lhs.column == rhs.column && lhs.value == rhs.value;
}