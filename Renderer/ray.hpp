/**
 * @file ray.hpp
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

class Ray
{

public:
    Direction d;
    Point p;

    Ray() {}
    Ray(Direction d, Point p) : d(d), p(p) {}
};