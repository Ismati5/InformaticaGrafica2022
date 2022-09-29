#pragma once

#include <iostream>
#include "direction.hpp"
#include "vect4.hpp"

using namespace std;

class Matrix4{

public:
    float m[4][4];     
    
    Matrix4() {
        for(int i = 0; i < 4; i++){
            for(int j = 0; j < 4; j++){
                m[i][j] = 0.0;
            }
        }
    };

    Matrix4 inverse() const;

    //Matrix * Matrix
    Matrix4 operator*(const Matrix4 &m) const;
    //Matrix * Direction
    Vect4 operator*(const Vect4 &v) const;

    friend std::ostream &operator<<(std::ostream& os, const Matrix4& m);
    
};

float determinant(float m1[4][4]);

//Returns a transformation matrix 
Matrix4 tm_translation(float x, float y, float z);
Matrix4 tm_scale(float x, float y, float z);
Matrix4 tm_rotation(float th);
//Matrix4 TM_changeBase(¿?);

std::ostream &operator<<(std::ostream& os, const Matrix4& m);