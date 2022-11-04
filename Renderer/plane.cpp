/**
 * @file plane.cpp
 * @author Adrián Yago & Ismael Tienda
 * @brief
 * @version 0.1
 * @date 2022-10-29
 *
 * @copyright Copyright (c) 2022
 *
 */
#include "plane.hpp"

/**
 * @brief Returns distance from point to hit in plane
 *
 * @param ray
 * @param t1
 * @param sur_normal
 * @return true
 * @return false
 */
bool Plane::intersect(Ray ray, float &t, Direction &sur_normal, Point &x)
{

    Direction d = ray.d;
    Point o = ray.p;

    // assuming vectors are normalized
    float denominator = d.dotProd(normal);

    if (denominator != 0)
    {

        float numerator = -(c + (o.x * normal.x + o.y * normal.y + o.z * normal.z));
        t = numerator / denominator;
        // The normal is equal to the surface normal
        sur_normal = normal;

        Direction intersection = d * t;
        x = ray.p + intersection;

        if (t > 0)
            return true; // the plane is not behind
    }

    // no intersection
    return false;
}