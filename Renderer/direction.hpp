/**
 * @file direction.hpp
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
#include <cmath>
#include "point.hpp"

using namespace std;

/**
 * @brief Direction represents a 3D Direction in space
 *
 */
class Direction
{

public:
    float x, y, z;

    /**
     * @brief Construct a new Direction object
     *
     * @param x
     * @param y
     * @param z
     */
    Direction(float x = 0.0, float y = 0.0,
              float z = 0.0);

    /**
     * @brief Direction = Direction + Direction
     *
     * @param d
     * @return Direction
     */
    Direction operator+(const Direction &d) const;

    /**
     * @brief Direction = Direction + Point
     *
     * @param p
     * @return Direction
     */
    Direction operator+(const Point &p) const;

    /**
     * @brief Direction = Direction - Direction
     *
     * @param d
     * @return Direction
     */
    Direction operator-(const Direction &d) const;

    /**
     * @brief Direction = Direction - Point
     *
     * @param p
     * @return Direction
     */
    Direction operator-(const Point &p) const;

    /**
     * @brief Direction = Direction * scalar
     *
     * @param s
     * @return Direction
     */
    Direction operator*(const float s) const;

    /**
     * @brief Direction = Direction / scalar
     *
     * @param s
     * @return Direction
     */
    Direction operator/(const float s) const;

    /**
     * @brief Returns Direction modulus
     *
     * @return float
     */
    float modulus() const;

    /**
     * @brief Returns normalized Direction
     *
     * @return Direction
     */
    Direction normalize() const;

    /**
     * @brief Returns Direction ⋅ Direction
     *
     * @param d
     * @return float
     */
    float dotProd(const Direction &d) const;

    /**
     * @brief Returns Direction x Direction
     *
     * @param d
     * @return Direction
     */
    Direction crossProd(const Direction &d) const;

    /**
     * @brief Direction = abs(Direction)
     *
     * @return Direction
     */
    Direction absolute() const;

    /**
     * @brief operator<<
     *
     * @param os
     * @param d
     * @return std::ostream&
     */
    friend std::ostream &
    operator<<(std::ostream &os, const Direction &d);
};

std::ostream &operator<<(std::ostream &os, const Direction &d);