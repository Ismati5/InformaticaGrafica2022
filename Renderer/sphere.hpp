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

#include "primitive.hpp"

using namespace std;

/**
 * @brief Sphere represents a 3D sphere
 *
 */
class Sphere : public Primitive
{

public:
    Direction axis;
    Point center, reference;
    float radius;

    /**
     * @brief Construct a new Sphere object
     *
     * @param center
     * @param radius
     * @param emi
     */
    Sphere(Point center, float radius, string id, Material mat)
        : center(center), radius(radius)
    {
        type = 1;
        name = id;
        setMaterial(mat);
    }

    /**
     * @brief Construct a new Sphere object
     *
     * @param axis
     * @param center
     * @param reference
     */
    Sphere(Direction axis, Point center, Point reference, Material mat)
        : axis(axis), center(center), reference(reference)
    {
        setMaterial(mat);
        Direction radius1 = axis / 2;
        Direction radius2 = reference - center;

        if (radius1.modulus() - radius2.modulus() > pow(10, -6))
        {
            throw std::invalid_argument("Radius is not the same");
        }
        else
        {
            radius = radius2.modulus();
        }
    }

    /**
     * @brief Returns sign
     *
     * @tparam T
     * @param val
     * @return int
     */
    template <typename T>
    int sign(T val);

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
