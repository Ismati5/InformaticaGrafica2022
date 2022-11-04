/**
 * @file triangle.cpp
 * @author Adrián Yago & Ismael Tienda
 * @brief
 * @version 0.1
 * @date 2022-10-01
 *
 * @copyright Copyright (c) 2022
 *
 */
#include "triangle.hpp"

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
bool Triangle::intersect(Ray ray, float &t, Direction &sur_normal, Point &x)
{

    Direction d = ray.d;
    Point o = ray.p;

    float denominator = normal.dotProd(d);
    if (denominator != 0) // ray is not parallel with the triangle
    { 
        float numerator = -((normal.x * o.x + normal.y * o.y + normal.z * o.z) + c);
        // t = (normal * o + c) / normal * d
        t = numerator / denominator;

        if (t < 0) return false; //Behind camera

        x = o + d * t;

        Direction C; 
        
        //edge 0
        Direction edge0 = p2 - p1;
        Direction v0 = x - p1;
        C = edge0.crossProd(v0);
        if (normal.dotProd(C) < 0) return false;

        //edge 1
        Direction edge1 = p3 - p2;
        Direction v1 = x - p2;
        C = edge1.crossProd(v1);
        if (normal.dotProd(C) < 0) return false;

        //edge 2
        Direction edge2 = p1 - p3;
        Direction v2 = x - p3;
        C = edge2.crossProd(v2);
        if (normal.dotProd(C) < 0) return false;

        sur_normal = normal;

        return true;

    }

    return false;

}