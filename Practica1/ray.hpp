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

    Ray(){}
    Ray(Direction d, Point p) : d(d), p(p){}

};