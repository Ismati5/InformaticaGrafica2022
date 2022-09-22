#pragma once

#include <iostream>

using namespace std;

class Direction;

class Point{

public:
    //c[0] = Px, c[1] = Py, c[2] = Pz
    float c[3];

    Point(float x = 0.0, float y = 0.0, float z = 0.0){
        c[0] = x; c[1] = y; c[2] = z;
    }

    //point + direction
    Point operator+(const Direction& d) const;
    //direction + direction
    Point operator-(const Direction& d) const;
    //point + point
    Direction operator-(const Point& p) const;

    friend ostream &operator<<(std::ostream& os, const Point& p);
};

ostream &operator<<(std::ostream& os, const Point& p);