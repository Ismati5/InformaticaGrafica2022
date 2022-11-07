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

// xt + n || yt + m || zt + b

// (x, y, z)
// (n, m, b)

//((xt + n) - c)^2

// (xt + n)^2 - 2*(xt + n)*c + c^2 + (yt + m)^2 - 2*(yt + m)*c + c^2 + (zt + b)^2 - 2*(zt + b)*c + c^2 - r^2

// (xt)^2 + n*n + 2xnt - 2xct + 2cn + c*c +
// (yt)^2 + m*m + 2ymt - 2yct + 2cm + c*c +
// (zt)^2 + b*b + 2zbt - 2zct + 2cb + c*c - r^2

// (x*x + y*y + z*z)t^2 + (2xn + 2ym + 2zb - 2xc - 2yc - 2zc)t + (n*n + m*m + b*b + 2cn + 2cm + 2cb + c*c + c*c + c*c - r*r)

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
 * @brief Point = Point * float
 *
 * @param p
 * @return Point
 */
Point Point::operator*(const float f) const
{
    return Point(x * f,
                 y * f, z * f);
};

/**
 * @brief Point = Point / float
 *
 * @param p
 * @return Point
 */
Point Point::operator/(const float f) const
{
    return Point(x / f,
                 y / f, z / f);
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