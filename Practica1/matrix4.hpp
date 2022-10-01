#pragma once
/**
 * @file matrix4.hpp
 * @author Adrián Yago & Ismael Tienda
 * @brief
 * @version 0.1
 * @date 2022-10-01
 *
 * @copyright Copyright (c) 2022
 *
 */
#include <iostream>
#include "direction.hpp"
#include "vect4.hpp"

using namespace std;

class Matrix4
{

public:
    float m[4][4];

    /**
     * @brief Construct a new Matrix 4 object
     *
     */
    Matrix4()
    {
        for (int i = 0; i < 4; i++)
        {
            for (int j = 0; j < 4; j++)
            {
                m[i][j] = 0.0;
            }
        }
    };

    /**
     * @brief Matrix * Matrix
     *
     * @param m
     * @return Matrix4
     */
    Matrix4 operator*(const Matrix4 &m) const;

    /**
     * @brief Matrix * Direction
     *
     * @param v
     * @return Vect4
     */
    Vect4 operator*(const Vect4 &v) const;

    /**
     * @brief Matrix / float
     *
     * @param n
     * @return Matrix4
     */
    Matrix4 operator/(const float &n) const;

    /**
     * @brief << operator
     *
     * @param os
     * @param m
     * @return std::ostream&
     */
    friend std::ostream &operator<<(std::ostream &os, const Matrix4 &m);
};

/**
 * @brief Returns inverse matrix of m1
 *
 * @param m1
 * @return Matrix4
 */
Matrix4 inverse(Matrix4 m1);

/**
 * @brief Returns determinant of a 4x4 matrix
 *
 * @param m1
 * @return float
 */
float determinant4x4(float m1[4][4]);

/**
 * @brief   Returns determinant of matrix eliminating the
 *          column "c" and the row "r"
 *
 * @param m1
 * @param r
 * @param c
 * @return float
 */
float determinant3x3(float m1[4][4], int r, int c);

/**
 * @brief Returns the transpose of m1
 *
 * @param m1
 * @return Matrix4
 */
Matrix4 transpose(Matrix4 m1);

/**
 * @brief Returns a translation transformation matrix
 *
 * @param x
 * @param y
 * @param z
 * @return Matrix4
 */
Matrix4 tm_translation(float x, float y, float z);

/**
 * @brief Returns a scale transformation matrix
 *
 * @param x
 * @param y
 * @param z
 * @return Matrix4
 */
Matrix4 tm_scale(float x, float y, float z);

/**
 * @brief   Returns a rotation transformation matrix given
 *          an axis "a"(0=x, 1=y, 2=z) and an angle "th"
 *
 * @param th
 * @param a
 * @return Matrix4
 */
Matrix4 tm_rotation(float th, int a);
// Returns a change of base transformation matrix
// given ¿?
// Matrix4 TM_changeBase(¿?);**************************************************************

/**
 * @brief << operator
 *
 * @param os
 * @param m
 * @return std::ostream&
 */
std::ostream &operator<<(std::ostream &os, const Matrix4 &m);