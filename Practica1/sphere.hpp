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
class Sphere
{

public:
    Direction axis;
    Point center, reference;
    float radius;
    //emission RGB tuple
    float emission[3];

    /**
     * @brief Construct a new Sphere object
     *
     * @param axis
     * @param center
     * @param reference
     */
    Sphere(Direction axis, Point center, Point reference,  float emi[3])
        : axis(axis), center(center), reference(reference)
    {
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

        emission[0] = emi[0];
        emission[1] = emi[1];
        emission[2] = emi[2];
    }

    //Returns distance from point to hit in sphere
    float Intersect(Ray ray) {

        Direction d  = ray.d;
        Point p = ray.p;

        //xt + n || yt + m || zt + b

        // (x, y, z)
        // (n, m, b)

        //((xt + n) - c)^2

        // (xt + n)^2 - 2*(xt + n)*c + c^2 + (yt + m)^2 - 2*(yt + m)*c + c^2 + (zt + b)^2 - 2*(zt + b)*c + c^2 - r^2

        // (xt)^2 + n*n + 2xnt - 2xct + 2cn + c*c + 
        // (yt)^2 + m*m + 2ymt - 2yct + 2cm + c*c + 
        // (zt)^2 + b*b + 2zbt - 2zct + 2cb + c*c - r^2 

        // (x*x + y*y + z*z)t^2 + (2xn + 2ym + 2zb - 2xc - 2yc - 2zc)t + (n*n + m*m + b*b + 2cn + 2cm + 2cb + c*c + c*c + c*c - r*r) = 0

        float a = d.x*d.x + d.y*d.y + d.z*d.z;
        float b = 2*d.x*p.x + 2*d.y*p.y + 2*d.z+p.z - 2*d.x*center.x - 2*d.y*center.y - 2*d.z*center.z;
        float c = p.x*p.x + p.y*p.y + p.z*p.z + 2*center.x*p.x + 2*center.y*p.y + 
                  2*center.z*p.z + center.x*center.x + center.y*center.y + center.z*center.z - radius*radius;

        // 2c / (-b -+ sqrt(b^2 -4ac)) 
        float temp = -0.5 * (b + sign(b) * sqrt(b*b - 4*a*c));
        float x1 = temp / a;
        float x2 = c / temp;


    }

};

template <typename T> int sign(T val) {
    return (T(0) < val) - (val < T(0));
}