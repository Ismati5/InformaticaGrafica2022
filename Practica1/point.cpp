#include <iostream>
#include "point.hpp"
#include "direction.hpp"



Point Point::operator+(const Direction& d) const{
    Point p;
    p.c[0] = this->c[0] + d.c[0];
    p.c[1] = this->c[1] + d.c[1];
    p.c[2] = this->c[2] + d.c[2];
    return p;
}
Point Point::operator-(const Direction& d) const{
    Point p;
    p.c[0] = this->c[0] - d.c[0];
    p.c[1] = this->c[1] - d.c[1];
    p.c[2] = this->c[2] - d.c[2];
    return p;
}
Direction Point::operator-(const Point& p) const{
    Direction d;
    d.c[0] = this->c[0] - p.c[0];
    d.c[1] = this->c[1] - p.c[1];
    d.c[2] = this->c[2] - p.c[2];
    return d;
}

std::ostream& operator<<(std::ostream& os, const Point& p)
{
 	os << "{" << p.c[0] << ", " << p.c[1] << ", " << p.c[2] << "}";

	return os;
}