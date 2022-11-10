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
    Vect3 emission;
    bool is_light = false;
    Vect3 p;

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

    bool isLight(){
        return is_light;
    }

    void setPower(Vect3 p){
        p = p;
        is_light = true;
    }

};
