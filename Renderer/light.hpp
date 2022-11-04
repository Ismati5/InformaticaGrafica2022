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

#include "vect3.hpp"
#include "point.hpp"
#include "direction.hpp"

using namespace std;

class Light
{

public:
    Point center;
    Vect3 power;

    Light() {}
    Light(Point c, Vect3 p) : center(c)
    {
        power = p;
    }
};