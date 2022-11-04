#pragma once
/**
 * @file triangle.hpp
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
#include "object.hpp"

using namespace std;

/**
 * @brief Sphere represents a 3D sphere
 *
 */
class Triangle : public Object
{

public:
    Point p1, p2, p3;
    float c;
    /**
     * @brief Construct a new Sphere object
     *
     * @param p1
     * @param p2
     * @param p3
     */
    Triangle(Point p1, Point p2, Point p3, Vect3 emi)
        : p1(p1), p2(p2), p3(p3)
        {
            Direction A = p2 - p1;
            Direction B = p3 - p1;
            normal = A.crossProd(B);
            normal.normalize();

            c = -(normal.x * p1.x + normal.y * p1.y + normal.z * p1.z);

            emission = emi;
        }

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
    bool intersect(Ray ray, float &t, Direction &sur_normal, Point &x);
};
