/**
 * @file direction.cpp
 * @author Adrián Yago & Ismael Tienda
 * @brief
 * @version 0.1
 * @date 2022-09-25
 *
 * @copyright Copyright (c) 2022
 *
 */
#include "direction.hpp"

/**
 * @brief Construct a new Direction:: Direction object
 *
 * @param x
 * @param y
 * @param z
 */
Direction::Direction(float x, float y,
                     float z) : x(x), y(y), z(z) {}

/**
 * @brief Direction = Direction + Direction
 *
 * @param d
 * @return Direction
 */
Direction Direction::operator+(const Direction &d) const
{
    return Direction(x + d.x,
                     y + d.y, z + d.z);
}

/**
 * @brief Direction = Direction + Point
 *
 * @param p
 * @return Direction
 */
Direction Direction::operator+(const Point &p) const
{
    return Direction(x + p.x,
                     y + p.y, z + p.z);
}

/**
 * @brief Direction = Direction - Direction
 *
 * @param d
 * @return Direction
 */
Direction Direction::operator-(const Direction &d) const
{
    return Direction(x - d.x,
                     y - d.y, z - d.z);
}

/**
 * @brief Direction = Direction - Point
 *
 * @param p
 * @return Direction
 */
Direction Direction::operator-(const Point &p) const
{
    return Direction(x - p.x,
                     y - p.y, z - p.z);
}

/**
 * @brief Direction = Direction * scalar
 *
 * @param s
 * @return Direction
 */
Direction Direction::operator*(const float s) const
{
    return Direction(x * s,
                     y * s, z * s);
}

/**
 * @brief Direction = Direction / scalar
 *
 * @param s
 * @return Direction
 */
Direction Direction::operator/(const float s) const
{
    return Direction(x / s,
                     y / s, z / s);
}

/**
 * @brief Returns Direction modulus
 *
 * @return float
 */
float Direction::modulus() const
{
    return sqrt(pow(x, 2) +
                pow(y, 2) + pow(z, 2));
}

/**
 * @brief Returns normalized Direction
 *
 * @return Direction
 */
Direction Direction::normalize() const
{
    float m = modulus();
    return Direction(x / m,
                     y / m, z / m);
}

/**
 * @brief Returns Direction ⋅ Direction
 *
 * @param d
 * @return float
 */
float Direction::dotProd(const Direction &d) const
{
    return x * d.x +
           y * d.y + z * d.z;
}

/**
 * @brief Returns Direction x Direction
 *
 * @param d
 * @return Direction
 */
Direction Direction::crossProd(const Direction &d) const
{
    return Direction(
        y * d.z - z * d.y,
        z * d.x - x * d.z,
        x * d.y - y * d.x);
}

/**
 * @brief operator<<
 *
 * @param os
 * @param d
 * @return std::ostream&
 */
ostream &operator<<(std::ostream &os, const Direction &d)
{
    os << "{" << d.x << ", " << d.y << ", " << d.z << "}";

    return os;
}