/**
 * @file material.hpp
 * @author Adrián Yago & Ismael Tienda
 * @brief
 * @version 0.1
 * @date 2022-10-29
 *
 * @copyright Copyright (c) 2022
 *
 */
#pragma once

#include <iostream>

#include "vect3.hpp"

using namespace std;

enum materialType
{
    DIFFUSE,
    SPECULAR,
    REFRACTION,
    ABSORTION,
};

class Material
{

public:
    Vect3 kd; // diffuse (color)
    Vect3 ks; // specular
    Vect3 kt; // reflection
    Vect3 ke; // power
    float pabs;
    float ref_coef;

    Material() {}

    Material(Vect3 kd_, Vect3 ks_, Vect3 kt_, Vect3 ke_, float _absortion) : kd(kd_), ks(ks_), kt(kt_), ke(ke_), ref_coef(1), pabs(_absortion) {}

    Material(Vect3 kd_, Vect3 ks_, Vect3 kt_, Vect3 ke_, float ref_coef_, float _absortion) : kd(kd_), ks(ks_), kt(kt_), ke(ke_), ref_coef(ref_coef_), pabs(_absortion) {}

    /**
     * @brief Check if it's an area light
     *
     * @return true
     * @return false
     */
    bool isLight()
    {
        return ke.x > 0 || ke.y > 0 || ke.z > 0;
    }

    /**
     * @brief Get the Mat Type object
     *
     * @return materialType
     */
    materialType getMatType(float &p)
    {

        float kd_max = kd.maxValue() / 255.0;
        float ks_max = ks.maxValue() / 255.0;
        float kt_max = kt.maxValue() / 255.0;

        float pd = kd_max / (kd_max + ks_max + kt_max + pabs);
        float ps = ks_max / (kd_max + ks_max + kt_max + pabs);
        float pt = kt_max / (kd_max + ks_max + kt_max + pabs);

        pabs = 1 - (pd + ps + pt);

        float type = (float)(rand()) / (float)(RAND_MAX);

        /*cout << "kd_max: " << kd_max << endl;
        cout << "ks_max: " << ks_max << endl;
        cout << "kt_max: " << kt_max << endl;
        cout << "abs: " << pabs << endl;

        cout << endl;

        cout << "RR: " << type << endl;
        cout << "pd: " << pd << endl;
        cout << "ps: " << ps << endl;
        cout << "pt: " << pt << endl;
        cout << "abs: " << pabs << endl;

        cout << endl;

        */

        if (type <= pd)
        {
            p = pd;
            return DIFFUSE;
        }
        else if (type <= ps + pd)
        {
            p = ps;
            return SPECULAR;
        }
        else if (type <= pt + ps + pd)
        {
            p = pt;
            return REFRACTION;
        }
        else
        {
            return ABSORTION;
        }
    }
};