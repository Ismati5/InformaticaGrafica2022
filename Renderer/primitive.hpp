/**
 * @file primitive.hpp
 * @author Adrián Yago & Ismael Tienda
 * @brief
 * @version 0.1
 * @date 2022-10-29
 *
 * @copyright Copyright (c) 2022
 *
 */
#pragma once

#include "vect3.hpp"
#include "ray.hpp"
#include "material.hpp"

using namespace std;

/**
 * @brief Primitive class
 *
 */
class Primitive
{
public:
    // emission RGB tuple
    Direction normal;
    Material material;
    int type;

    string name = "none";

    /**
     * @brief intersect interface
     *
     * @param ray
     * @param t
     * @param sur_normal
     * @return true
     * @return false
     */
    virtual bool intersect(Ray ray, float &t, Direction &sur_normal, Point &x) = 0;

    void setMaterial(Material mat)
    {
        material = mat;
    }

    bool isLight()
    {
        return material.isLight();
    }

    Vect3 power()
    {
        return material.ke;
    }

    Vect3 color()
    {
        return material.kd;
    }
};
