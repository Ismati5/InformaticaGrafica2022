/**
 * @file texture.hpp
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

class Texture
{

public:
    Vect3 kd;       // diffuse (color)
    Vect3 ks;       // specular
    Vect3 kt;       // reflection
    Vect3 ke;       // power
    Vect3 probs;    // type of material (pd, ps, pt)
    float ref_coef;

    Texture(){}

    Texture( Vect3 kd_, Vect3 ks_, Vect3 kt_, Vect3 ke_, Vect3 probs_, float ref_coef_) : 
        kd(kd_), ks(ks_), kt(kt_), ke(ke_), probs(probs_), ref_coef(ref_coef_) {}

    bool isLight() 
    {
        return ke.x > 0 || ke.y > 0 || ke.z > 0;
    }

    // return 0 = diffuse, return 1 = specular, return 2 = reflect, return 3 absortion
    int getMatType() 
    {
        float absortion = 1 - (probs.x + probs.y + probs.z);
        float pd = probs.x;
        float ps = probs.y + pd;
        float pt = probs.z + ps;

        int type = (float)(rand()) / (float)(RAND_MAX);

        if (type <= pd) return 0;
        else if (type <= ps) return 1;
        else if (type <= pt) return 2;
        else return 3;

    }
};