/**
 * @file
 *
 * Declaration of the Position class
 */

#ifndef _POSITION_H
#define _POSITION_H

#include <ostream>

template<typename T> class Position;

using fPos = Position<float>;
using iPos = Position<int>;

/**
 * The Position templated class is a generic class for providing
 * position information and associated methods.
 */
template<typename T>
class Position {
public:
    Position() {};

    Position(T _x, T _y) : 
        x(_x),
        y(_y)
    {}

    Position(const Position& p) :
        x(p.x),
        y(p.y)
    {}

    T x;
    T y;
};

template<typename T>
std::ostream& operator<<(std::ostream& os, const Position<T>& pos)
{
    os << pos.x << "," << pos.y;
    return os;
}

#endif /* _POSITION_H */
