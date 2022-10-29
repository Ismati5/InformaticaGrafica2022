/**
 * @file sphere.cpp
 * @author Adrián Yago & Ismael Tienda
 * @brief
 * @version 0.1
 * @date 2022-10-01
 *
 * @copyright Copyright (c) 2022
 *
 */
#include "sphere.hpp"

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
bool Sphere::intersect(Ray ray, float &t, Direction &sur_normal)
{

    float t1, t2;

    Direction d = ray.d;
    Point o = ray.p;

    Direction aux = o - center;
    float a = 1;                                         // a = d^2 = 1 because d is normalized
    float b = 2 * d.dotProd(aux);                        // b = 2d(o-c)
    float c = abs(aux.dotProd(aux)) - (radius * radius); // c = |o-c|^2 - r^2

    float discriminant = b * b - 4 * a * c;
    if (discriminant < 0)
        return false; // No solutions
    if (discriminant == 0)
        t1 = t2 = -0.5 * b / a; // One solution

    float q = (b > 0) ? -0.5 * (b + sqrt(discriminant)) : -0.5 * (b - sqrt(discriminant));
    t1 = q / a;
    t2 = c / q;

    if (t1 > t2)
        swap(t1, t2); // Get the closest distance
    if (t1 < 0)
    {
        t1 = t2;
        if (t1 < 0)
            return false; // Sphere is behind camera
    }

    t = t1;

    Point intersection(t1 * d.x, t1 * d.y, t1 * d.z);
    // Gradient f(x,y,z) = (2(c-1)^2 * x, 2(c-1)^2 * y,  2(c-1)^2 * z)
    sur_normal.x = 2 * (c - 1) * (c - 1) * intersection.x;
    sur_normal.y = 2 * (c - 1) * (c - 1) * intersection.y;
    sur_normal.z = 2 * (c - 1) * (c - 1) * intersection.z;
    sur_normal.normalize();

    return true;
}