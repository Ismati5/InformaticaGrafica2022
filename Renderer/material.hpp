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

    // Textures
    Vect3 *texture;
    int texture_res[2] = {0, 0};
    Point ref_point;
    Direction e1, e2;
    Direction normal;

    Material() {}

    Material(Vect3 kd_, Vect3 ks_, Vect3 kt_, Vect3 ke_, float _absortion, Vect3 *texture, int _texture_res[2], Point ref_point, Direction e1, Direction e2, Direction normal) : kd(kd_), ks(ks_), kt(kt_), ke(ke_), ref_coef(1), pabs(_absortion), texture(texture), ref_point(ref_point), e1(e1), e2(e2), normal(normal)
    {
        texture_res[0] = _texture_res[0];
        texture_res[1] = _texture_res[1];
    }

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
     * @brief Material has a custom texture
     *
     * @return true
     * @return false
     */
    bool hasTexture()
    {
        return !(texture_res[0] == 0 && texture_res[1] == 0);
    }

    /**
     * @brief Get the Mat Type object
     *
     * @return materialType
     */
    materialType getMatType(float &p, bool noAbsortion)
    {

        float kd_max = kd.maxValue() / 255.0;
        float ks_max = ks.maxValue() / 255.0;
        float kt_max = kt.maxValue() / 255.0;

        float pabs_aux = pabs;

        float pd = kd_max / (kd_max + ks_max + kt_max + pabs_aux);
        float ps = ks_max / (kd_max + ks_max + kt_max + pabs_aux);
        float pt = kt_max / (kd_max + ks_max + kt_max + pabs_aux);

        if (noAbsortion)
            pabs_aux = 0;
        else
            pabs_aux = pabs / (kd_max + ks_max + kt_max + pabs_aux);

        float type = (float)(rand()) / (float)(RAND_MAX);

        if (type < pd)
        {
            p = pd + pabs_aux;
            return DIFFUSE;
        }
        else if (type < ps + pd)
        {
            p = ps + pabs_aux;
            return SPECULAR;
        }
        else if (type < pt + ps + pd)
        {
            p = pt + pabs_aux;
            return REFRACTION;
        }
        else
        {
            return ABSORTION;
        }
    }
};