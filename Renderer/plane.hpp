/**
 * @file plane.hpp
 * @author Adrián Yago & Ismael Tienda
 * @brief
 * @version 0.1
 * @date 2022-10-01
 *
 * @copyright Copyright (c) 2022
 *
 */
#pragma once

#include <iostream>

#include "primitive.hpp"

using namespace std;

/**
 * @brief Plane represents a 3D plane
 *
 */
class Plane : public Primitive
{

public:
    float c;

    /**
     * @brief Construct a new Plane object
     *
     * @param normal
     * @param c
     * @param emi
     */
    Plane(Direction _normal, float c, Vect3 emi) : c(c)
    {

        normal = _normal;
        emission = emi;
    }

    /**
     * @brief Returns distance from point to hit in plane
     *
     * @param ray
     * @param t1
     * @param sur_normal
     * @return true
     * @return false
     */
    bool intersect(Ray ray, float &t, Direction &sur_normal, Point &x);
};
