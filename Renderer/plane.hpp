#pragma once
/**
 * @file sphere.hpp
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
#include "point.hpp"
#include "ray.hpp"
#include <cmath>

using namespace std;

/**
 * @brief Sphere represents a 3D sphere
 *
 */
class Plane
{

public:
    Direction normal;
    float c;
    // emission RGB tuple
    int emission[3];

    Plane(Direction normal, float c, int emi[3]) : normal(normal), c(c)
    {

        emission[0] = emi[0];
        emission[1] = emi[1];
        emission[2] = emi[2];
    }

    // Returns distance from point to hit in plane
    bool Intersect(Ray ray, float &t1, Direction &sur_normal)
    {

        Direction d = ray.d;
        Point o = ray.p;

        // assuming vectors are normalized
        float denominator = d.dotProd(normal);

        if (denominator != 0)
        {

            float numerator = -(c + (o.x * normal.x + o.y * normal.y + o.z * normal.z));
            t1 = numerator / denominator;
            // The normal is equal to the surface normal
            sur_normal = normal;

            if (t1 > 0)
                return true; // the plane is not behind
        }

        // no intersection
        return false;
    }
};
