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
#include "direction.hpp"
#include "point.hpp"
#include "ray.hpp"
#include "object.hpp"
#include <cmath>

using namespace std;

/**
 * @brief Plane represents a 3D plane
 *
 */
class Plane : public Object
{

public:
    Direction normal;
    float c;

    /**
     * @brief Construct a new Plane object
     *
     * @param normal
     * @param c
     * @param emi
     */
    Plane(Direction normal, float c, int emi[3]) : normal(normal), c(c)
    {

        emission[0] = emi[0];
        emission[1] = emi[1];
        emission[2] = emi[2];
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
    bool intersect(Ray ray, float &t1, Direction &sur_normal);
};
