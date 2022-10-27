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
#include "direction.hpp"
#include "point.hpp"
#include "ray.hpp"
#include <cmath>

using namespace std;

/**
 * @brief Sphere represents a 3D sphere
 *
 */
class Plane
{

public:

    Direction normal;
    float c;
    //emission RGB tuple
    float emission[3];


    Plane(Direction normal, float c,  float emi[3]): 
        normal(normal), c(c){

            emission[0] = emi[0];
            emission[1] = emi[1];
            emission[2] = emi[2];
        }


    //Returns distance from point to hit in plane
    bool Intersect(Ray ray, float& t1, Direction& sur_normal) {

        Direction d  = ray.d;
        Point p = ray.p;

        //assuming vectors are normalized
        float denominator = d.dotProd(normal);
        if (denominator > 1e-6) {

            //float numerator = c + p*normal;
            //The normal is equal to the surface normal
            sur_normal = normal;

            return true;
        }

        //no intersection
        return false;

    }

};
