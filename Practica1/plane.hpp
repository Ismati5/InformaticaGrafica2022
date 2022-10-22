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
    Point center;
    //emission RGB tuple
    float emission[3];


    Plane(Direction normal, Point center,  float emi[3]): 
        normal(normal), center(center){

            emission[0] = emi[0];
            emission[1] = emi[1];
            emission[2] = emi[2];
        }


    //Returns distance from point to hit in plane
    bool Intersect(Ray ray, float& t, Direction& sur_normal) {

        Direction d  = ray.d;
        Point p = ray.p;

        //t = ( (c - p) * n ) / (d * n)
        //assuming vectors are normalized
        float denominator = d.dotProd(normal);
        if (denominator > 1e-6) {

            Direction temp = center - p;
            t = temp.dotProd(normal) / denominator;

            Point intersection(t * d.x, t*d.y, t*d.z);
            // f(x,y,z) = Ax + By + Cz + D = 0
            float D = normal.x * center.x + 
                      normal.y * center.y + 
                      normal.z * center.z;

            //normal to surface that goes throw "intersection"
            sur_normal.x = normal.x * intersection.x;
            sur_normal.y = normal.y * intersection.y;
            sur_normal.z = normal.z * intersection.z;

            return true;
        }

        //no intersection
        return false;

    }

};
