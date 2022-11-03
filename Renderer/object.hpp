/**
 * @file object.hpp
 * @author Adrián Yago & Ismael Tienda
 * @brief
 * @version 0.1
 * @date 2022-10-29
 *
 * @copyright Copyright (c) 2022
 *
 */
#pragma once

#include "ray.hpp"
#include "direction.hpp"

using namespace std;

/**
 * @brief Object class
 *
 */
class Object
{
public:
    // emission RGB tuple
    Direction normal;
    int emission[3];

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
};
