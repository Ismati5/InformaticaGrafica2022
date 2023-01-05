/**
 * @file square.cpp
 * @author Adrián Yago & Ismael Tienda
 * @brief
 * @version 0.1
 * @date 2022-10-01
 *
 * @copyright Copyright (c) 2022
 *
 */
#include "square.hpp"

/**
 * @brief   Returns the number of intersections with a ray.
 *          In case that the ray hits at least once, t1 will have
 *          the distance to the closest hit. If the ray hits twice
 *          t2 will have the distance to the further hit.
 *
 * @param ray
 * @param t
 * @param sur_normal
 * @return true
 * @return false
 */
bool Square::intersect(Ray ray, float &t, Direction &sur_normal, Point &x)
{

    Direction d = ray.d;
    Point o = ray.p;

    // assuming vectors are normalized
    float denominator = d.dotProd(normal);

    if (denominator != 0)
    {

        float numerator = (p - o).dotProd(normal);
        t = numerator / denominator;
        // The normal is equal to the surface normal
        sur_normal = normal;

        if (t > 0)
        {

            Direction intersection = d * t;
            x = ray.p + intersection;

            Direction v = x - p;

            float width = e1.modulus();
            float height = e2.modulus();

            float proj1 = v.dotProd(e1) / width;
            float proj2 = v.dotProd(e2) / height;

            if ((proj1 < width && proj1 > 0) && (proj2 < height && proj2 > 0))
                return true;
        }
    }

    return false;
}