#pragma once

#include <iostream>
#include <cmath>
#include "point.hpp"

using namespace std;

class Direction{

public:
    //c[0] = Dx, c[1] = Dy, c[2] = Dz
    float c[3];     
    
    Direction(float x = 0.0, float y = 0.0, 
    float z = 0.0){
        c[0] = x; c[1] = y; c[2] = z;
    }

    //direction + direction
    Direction operator+(const Direction &d) const;

    //direction + point
    Direction operator+(const Point &p) const;

    //direction - direction
    Direction operator-(const Direction &d) const;
    
    //direction + point
    Direction operator-(const Point &p) const;
    
    //direction * scalar
    Direction operator*(const float s) const;
    
    //direction / scalar
    Direction operator/(const float s) const;

    float modulus() const;

    Direction normalize() const;

    float dotProd(const Direction &d) const;
    Direction crossProd(const Direction &d) const;

    friend std::ostream& operator<<(std::ostream& os, const Direction& d);
    
};

std::ostream& operator<<(std::ostream& os, const Direction& d);