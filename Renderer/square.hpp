#pragma once
/**
 * @file square.hpp
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
#include "primitive.hpp"

using namespace std;

/**
 * @brief Sphere represents a 3D sphere
 *
 */
class Square : public Primitive
{

public:
    Point p;
    Direction e1, e2;

    /**
     * @brief Construct a new Square object
     *
     * @param p1
     * @param p2
     * @param p3
     * @param p4
     */
    Square(Point p1, Direction n, Direction width, Direction height, Material mat)
        : p(p1), e1(width), e2(height)
    {
        setMaterial(mat);
        normal = n;
        normal.normalize();
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
