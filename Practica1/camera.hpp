#pragma once

#include <iostream>
#include "direction.hpp"
#include "point.hpp"
#include "ray.hpp"
#include <cmath>

using namespace std;

class Camera{

public:
    Point origin;
    Direction L, U, F; //L = x, U = z, F = y
    float size[2];

    Point topLeft, topRight, botLeft, botRight;
    Direction pixelSize_x, pixelSize_y;

    Camera(Direction _L, Direction _U, Direction _F, Point O, float _size[2]): 
        L(_L), U(_U), F(_F), origin(O){

        size[0] = _size[0];
        size[1] = _size[1];

        topLeft = origin + U + L + F;
        topRight = origin + U - L + F;
        botLeft = origin - U + L + F;
        botRight = origin - U - L + F;

        pixelSize_x = (topLeft - topRight) / size[0];
        pixelSize_y = (topLeft - botLeft) / size[1];

    }

    void render() {

        Ray McQueen;
        McQueen.p = origin;

        for (float i = 0; i < size[0]; i++){
           for (float j = 0; j < size[1]; j++){
                Point pixel = topLeft + pixelSize_x * i + pixelSize_y * j;
                McQueen.d = pixel - origin;
            } 
        }

    }


};