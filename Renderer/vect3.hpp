/**
 * @file vect3.hpp
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

using namespace std;

/**
 * @brief Direction represents a Vect3
 *
 */
class Vect3
{

public:
    float x, y, z;

    /**
     * @brief Construct a new Vect3 object
     *
     * @param x
     * @param y
     * @param z
     */
    Vect3(float x = 0.0, float y = 0.0,
          float z = 0.0);

    /**
     * @brief Vect3 = Vect3 + Vect3
     *
     * @param d
     * @return Vect3
     */
    Vect3 operator+(const Vect3 &d) const;

    /**
     * @brief Vect3 = Vect3 - Vect3
     *
     * @param d
     * @return Vect3
     */
    Vect3 operator-(const Vect3 &d) const;

    /**
     * @brief Vect3 = Vect3 * scalar
     *
     * @param s
     * @return Vect3
     */
    Vect3 operator*(const float s) const;

    /**
     * @brief Vect3 = Vect3 * Vect3
     *
     * @param Vect3
     * @return Vect3
     */
    Vect3 operator*(const Vect3 &d) const;

    /**
     * @brief Vect3 = Vect3 / scalar
     *
     * @param s
     * @return Vect3
     */
    Vect3 operator/(const float s) const;

    /**
     * @brief Vect3 = Vect3
     *
     * @param d
     * @return Vect3
     */
    const Vect3 &operator=(const Vect3 &d);

    /**
     * @brief Vect3 = Vect3 * float
     *
     * @param d
     * @return Vect3
     */
    const Vect3 &operator*=(const float s);

    /**
     * @brief Vect3 = Vect3 * Vect3
     *
     * @param d
     * @return Vect3
     */
    const Vect3 &operator*=(const Vect3 &d);

    /**
     * @brief Vect3 = Vect3 + Vect3
     *
     * @param d
     * @return Vect3
     */
    const Vect3 &operator+=(const Vect3 &d);

    /**
     * @brief Vect3 = Vect3 / s
     *
     * @param d
     * @return Vect3
     */
    const Vect3 &operator/=(const float s);

    const float& operator[](size_t t) const;

    /**
     * @brief Returns max vector value
     *
     * @return const float
     */
    const float maxValue();

    /**
     * @brief operator<<
     *
     * @param os
     * @param d
     * @return std::ostream&
     */
    friend std::ostream &operator<<(std::ostream &os, const Vect3 &d);
};

std::ostream &operator<<(std::ostream &os, const Vect3 &d);