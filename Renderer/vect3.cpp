/**
 * @file vect3.cpp
 * @author Adrián Yago & Ismael Tienda
 * @brief
 * @version 0.1
 * @date 2022-09-25
 *
 * @copyright Copyright (c) 2022
 *
 */
#include "vect3.hpp"

/**
 * @brief Construct a new vect3
 *
 * @param x
 * @param y
 * @param z
 */
Vect3::Vect3(float x, float y,
             float z) : x(x), y(y), z(z) {}

/**
 * @brief Vect3 = Vect3 + Vect3
 *
 * @param d
 * @return Vect3
 */
Vect3 Vect3::operator+(const Vect3 &d) const
{
    return Vect3(x + d.x,
                 y + d.y, z + d.z);
}
/**
 * @brief Vect3 = Vect3 - Vect3
 *
 * @param d
 * @return Vect3
 */
Vect3 Vect3::operator-(const Vect3 &d) const
{
    return Vect3(x - d.x,
                 y - d.y, z - d.z);
}

/**
 * @brief Vect3 = Vect3 * scalar
 *
 * @param s
 * @return Vect3
 */
Vect3 Vect3::operator*(const float s) const
{
    return Vect3(x * s,
                 y * s, z * s);
}
/**
 * @brief Vect3 = Vect3 * Vect3
 *
 * @param Vect3
 * @return Vect3
 */
Vect3 Vect3::operator*(const Vect3 &d) const
{
    return Vect3(x * d.x,
                 y * d.y, z * d.z);
}

/**
 * @brief Vect3 = Vect3 / scalar
 *
 * @param s
 * @return Vect3
 */
Vect3 Vect3::operator/(const float s) const
{
    return Vect3(x / s,
                 y / s, z / s);
}

/**
 * @brief Vect3 = Vect3
 *
 * @param d
 * @return Vect3
 */
const Vect3 &Vect3::operator=(const Vect3 &d)
{
    x = d.x;
    y = d.y;
    z = d.z;

    return (*this);
}

/**
 * @brief Vect3 = Vect3 * float
 *
 * @param d
 * @return Vect3
 */
const Vect3 &Vect3::operator*=(const float s)
{
    x *= s;
    y *= s;
    z *= s;

    return (*this);
}

/**
 * @brief Vect3 = Vect3 * Vect3
 *
 * @param d
 * @return Vect3
 */
const Vect3 &Vect3::operator*=(const Vect3 &d)
{
    x *= d.x;
    y *= d.y;
    z *= d.z;

    return (*this);
}

/**
 * @brief Vect3 = Vect3 + Vect3
 *
 * @param d
 * @return Vect3
 */
const Vect3 &Vect3::operator+=(const Vect3 &d)
{
    x += d.x;
    y += d.y;
    z += d.z;

    return (*this);
}

/**
 * @brief Vect3 = Vect3 / s
 *
 * @param d
 * @return Vect3
 */

const Vect3 &Vect3::operator/=(const float s)
{
    x /= s;
    y /= s;
    z /= s;

    return (*this);
}

const float& Vect3::operator[](size_t t) const
{
    if (t == 0)
        return x;
    else if (t == 1)
        return y;
    else // (t == 2)
        return z;
}

/**
 * @brief Returns max vector value
 *
 * @return const float
 */
const float Vect3::maxValue()
{
    return max(x, max(y, z));
}

/**
 * @brief operator<<
 *
 * @param os
 * @param d
 * @return std::ostream&
 */
ostream &operator<<(std::ostream &os, const Vect3 &d)
{
    os << "(" << d.x << ", " << d.y << ", " << d.z << ")";

    return os;
}
