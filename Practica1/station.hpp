#pragma once
/**
 * @file station.hpp
 * @author Adrián Yago & Ismael Tienda
 * @brief
 * @version 0.1
 * @date 2022-10-01
 *
 * @copyright Copyright (c) 2022
 *
 */
#define _USE_MATH_DEFINES

#include <iostream>
#include "direction.hpp"
#include "point.hpp"
#include "sphere.hpp"
#include <cmath>

using namespace std;

/**
 * @brief Station represents a 3D planetary station
 *
 */
class Station
{

public:
    Sphere sp;
    float inclination, azimuth;

    Point position;
    Direction normal, long_tangent, lat_tangent;

    Point base;

    /**
     * @brief Construct a new Station object
     *
     * @param axis
     * @param center
     * @param reference
     */
    Station(Sphere sp, float inclination, float azimuth)
        : sp(sp), inclination(inclination), azimuth(azimuth)
    {
        position.x = sp.radius * sin(inclination) * cos(azimuth);
        position.y = sp.radius * sin(inclination) * sin(azimuth);
        position.z = sp.radius * cos(inclination);

        normal = position - sp.center;

        Point long_tang_p(sp.radius * sin(0) * cos(M_PI), sp.radius * sin(0) * sin(M_PI), sp.radius * cos(0));
        Point lat_tang_p(sp.radius * sin(M_PI) * cos(0), sp.radius * sin(M_PI) * sin(0), sp.radius * cos(M_PI));

        long_tangent = long_tang_p - sp.center;
        lat_tangent = lat_tang_p - sp.center;
    }
};