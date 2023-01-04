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

    float denominator = normal.dotProd(normal);
    float NdotRayDirection = normal.dotProd(d);

    if (fabs(NdotRayDirection) < 1e-8) // almost 0
        return false;                  // they are parallel so they don't intersect !

    // compute d parameter using equation 2
    float df = -normal.dotProd(Direction(p1.x, p1.y, p1.z));

    // compute t (equation 3)
    t = -(df + normal.dotProd(Direction(o.x, o.y, o.z))) / NdotRayDirection;

    // check if the triangle is in behind the ray
    if (t < 0)
        return false; // the triangle is behind

    x = o + d * t;

    Direction C;

    // edge 0
    Direction edge0 = p2 - p1;
    Direction v0 = x - p1;
    C = edge0.crossProd(v0);
    if (normal.dotProd(C) < 0)
        return false;

    // edge 1
    Direction edge1 = p3 - p2;
    Direction v1 = x - p2;
    C = edge1.crossProd(v1);
    if (normal.dotProd(C) < 0)
        return false;

    // edge 2
    Direction edge2 = p4 - p3;
    Direction v2 = x - p3;
    C = edge2.crossProd(v2);
    if (normal.dotProd(C) < 0)
        return false;

    // edge 3
    Direction edge3 = p1 - p4;
    Direction v3 = x - p4;
    C = edge3.crossProd(v3);
    if (normal.dotProd(C) < 0)
        return false;

    sur_normal = normal;

    cout << "INTERSECT" << endl;
    return true;
}