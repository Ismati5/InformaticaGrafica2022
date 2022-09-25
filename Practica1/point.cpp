/**
 * @file point.cpp
 * @author Adrián Yago & Ismael Tienda
 * @brief
 * @version 0.1
 * @date 2022-09-25
 *
 * @copyright Copyright (c) 2022
 *
 */
#include "point.hpp"
#include "direction.hpp"

/**
 * @brief Construct a new Point:: Point object
 *
 * @param x
 * @param y
 * @param z
 */
Point::Point(float x, float y, float z) : x(x), y(y), z(z) {}

/**
 * @brief Point = Point + Direction
 *
 * @param d
 * @return Point
 */
Point Point::operator+(const Direction &d) const
{
    Point p;
    p.x = this->x + d.x;
    p.y = this->y + d.y;
    p.z = this->z + d.z;
    return p;
};

/**
 * @brief Point = Point - Direction
 *
 * @param d
 * @return Point
 */
Point Point::operator-(const Direction &d) const
{
    Point p;
    p.x = this->x - d.x;
    p.y = this->y - d.y;
    p.z = this->z - d.z;
    return p;
};

/**
 * @brief Direction = Point - Point
 *
 * @param p
 * @return Direction
 */
Direction Point::operator-(const Point &p) const
{
    Direction d;
    d.x = this->x - p.x;
    d.y = this->y - p.y;
    d.z = this->z - p.z;
    return d;
};

/**
 * @brief operator<<
 *
 * @param os
 * @param p
 * @return ostream&
 */
ostream &operator<<(ostream &os, const Point &p)
{
    os << "{" << p.x << ", " << p.y << ", " << p.z << "}";

    return os;
};