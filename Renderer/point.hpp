/**
 * @file point.hpp
 * @author Adrián Yago & Ismael Tienda
 * @brief
 * @version 0.1
 * @date 2022-09-25
 *
 * @copyright Copyright (c) 2022
 *
 */
#pragma once

#include <iostream>

using namespace std;

class Direction;

/**
 * @brief Point represents a 3D Point in space
 *
 */
class Point
{

public:
    float x, y, z;

    /**
     * @brief Construct a new Point object
     *
     * @param x
     * @param y
     * @param z
     */
    Point(float x = 0.0, float y = 0.0, float z = 0.0);

    /**
     * @brief Point = Point + Direction
     *
     * @param d
     * @return Point
     */
    Point operator+(const Direction &d) const;

    /**
     * @brief Point = Point - Direction
     *
     * @param d
     * @return Point
     */
    Point operator-(const Direction &d) const;

    /**
     * @brief Direction = Point - Point
     *
     * @param p
     * @return Direction
     */
    Direction operator-(const Point &p) const;

    /**
     * @brief Point = Point * float
     *
     * @param p
     * @return Point
     */
    Point operator*(const float f) const;

    /**
     * @brief Point = Point / float
     *
     * @param p
     * @return Point
     */
    Point operator/(const float f) const;

    /**
     * @brief operator<<
     *
     * @param os
     * @param p
     * @return ostream&
     */
    friend ostream &operator<<(std::ostream &os, const Point &p);
};

ostream &operator<<(std::ostream &os, const Point &p);