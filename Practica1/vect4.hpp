#pragma once

#include <iostream>
#include "point.hpp"
#include "direction.hpp"

using namespace std;

class Vect4{

public:
    float v[4];     
    
    Vect4(Point p){
        v[0] = p.x;
        v[1] = p.y;
        v[2] = p.z;
        v[3] = 1;
    }
    Vect4(Direction d){
        v[0] = d.x;
        v[1] = d.y;
        v[2] = d.z;
        v[3] = 0;
    }

    friend std::ostream &operator<<(std::ostream& os, const Vect4& v);
    
};

std::ostream &operator<<(std::ostream& os, const Vect4& v);