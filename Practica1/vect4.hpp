#pragma once
/**
 * @file vect4.hpp
 * @author Adrián Yago & Ismael Tienda
 * @brief
 * @version 0.1
 * @date 2022-10-01
 *
 * @copyright Copyright (c) 2022
 *
 */
#include <iostream>
#include "point.hpp"
#include "direction.hpp"

using namespace std;

/**
 * @brief Vect4 represents a 4 number tuple
 *
 */
class Vect4
{

public:
    float v[4];

    /**
     * @brief Construct a new Vect 4 object
     *
     * @param p
     */
    Vect4(Point p)
    {
        v[0] = p.x;
        v[1] = p.y;
        v[2] = p.z;
        v[3] = 1;
    }

    /**
     * @brief Construct a new Vect 4 object
     *
     * @param d
     */
    Vect4(Direction d)
    {
        v[0] = d.x;
        v[1] = d.y;
        v[2] = d.z;
        v[3] = 0;
    }

    /**
     * @brief From Vect4 to Point
     *
     * @return Point
     */
    Point toPoint()
    {
        Point p;
        p.x = v[0];
        p.y = v[1];
        p.z = v[2];
    }

    /**
     * @brief << operator
     *
     * @param os
     * @param v
     * @return std::ostream&
     */
    friend std::ostream &operator<<(std::ostream &os, const Vect4 &v);
};

/**
 * @brief << operator
 *
 * @param os
 * @param v
 * @return std::ostream&
 */
std::ostream &operator<<(std::ostream &os, const Vect4 &v);