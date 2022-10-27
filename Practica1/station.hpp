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
#include "matrix4.hpp"
#include "vect4.hpp"
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

        // Base change to the local shpere coords
        Direction refVect = sp.reference - sp.center; // Direction from center to reference

        Direction w = sp.axis.normalize();
        Direction v = w.crossProd(refVect).normalize();
        Direction u = v.crossProd(w).normalize();

        Matrix4 T_change = TM_changeBase(u, v, w, sp.center);
        Vect4 localCoordsRef(sp.reference);
        // cout << "test " << sp.axis.crossProd(refVect) << " " << sp.axis.crossProd(refVect).modulus() << endl;
        // cout << "old base: " << localCoordsRef << endl;
        cout << "T mat: " << endl
             << T_change << endl;
        localCoordsRef = inverse(T_change) * localCoordsRef; // Reference point on local base
        // cout << "new base: " << localCoordsRef << endl;
        Matrix4 T = tm_rotation(inclination - asin(sp.axis.dotProd(refVect) / (sp.axis.modulus() * refVect.modulus())), 1);
        localCoordsRef = T * localCoordsRef;
        // cout << "new base and rotated in x: " << localCoordsRef << endl;
        // cout << inclination << " : " << asin(sp.axis.dotProd(refVect) / (sp.axis.modulus() * refVect.modulus())) << endl;
        T = tm_rotation(azimuth, 2);

        cout << "Rotate azimuth: " << T << endl;

        localCoordsRef = T * localCoordsRef;
        // cout << "new base and rotated in y: " << localCoordsRef << endl;
        T = TM_changeBase(u, v, w, sp.center);

        localCoordsRef = T * localCoordsRef;

        position = localCoordsRef.toPoint();

        normal = position - sp.center;

        long_tangent = normal.crossProd(sp.axis);
        lat_tangent = long_tangent.crossProd(normal);
    }
};