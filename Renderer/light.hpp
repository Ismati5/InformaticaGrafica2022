/**
 * @file light.hpp
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

#include "point.hpp"
#include "direction.hpp"

using namespace std;

class Light
{

public:
    Point center;
    float power[3];

    Light() {}
    Light(Point c, float p[3]) : center(c) {
        power[0] = p[0];
        power[1] = p[1];
        power[2] = p[2];
    }
};