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

    // data members
    T x;
    T y;

    // operators
    Position& operator+=(const Position& p2)
    {
        x += p2.x;
        y += p2.y;
        return *this;
    }
};

// associated global operator definitions
template<typename T>
std::ostream& operator<<(std::ostream& os, const Position<T>& pos)
{
    os << pos.x << "," << pos.y;
    return os;
}

template<typename T>
const bool operator==( const Position<T>& lhs, const Position<T>& rhs ) {
   return ((lhs.x == rhs.x) && (lhs.y == rhs.y));
}

template<typename T>
const bool operator!=( const Position<T>& lhs, const Position<T>& rhs ) {
   return !(lhs==rhs);
}

template<typename T>
const Position<T> operator+( const Position<T>& lhs, const Position<T>& rhs ) {
   return Position<T>(lhs.x+rhs.x, lhs.y+rhs.y);
}

template<typename T>
const Position<T> operator-( const Position<T>& lhs, const Position<T>& rhs ) {
   return Position<T>(lhs.x-rhs.x, lhs.y-rhs.y);
}

#endif /* _POSITION_H */
