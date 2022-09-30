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

    //Matrix * Matrix
    Matrix4 operator*(const Matrix4 &m) const;
    //Matrix * Direction
    Vect4 operator*(const Vect4 &v) const;
    //Matrix / float
    Matrix4 operator/(const float &n) const;

    friend std::ostream &operator<<(std::ostream& os, const Matrix4& m);
    
};

//Returns inverse matrix of m1
Matrix4 inverse(Matrix4 m1);
//Returns determinant of a 4x4 matrix
float determinant4x4(float m1[4][4]);
//Returns determinant of matrix eliminating the 
//column "c" and the row "r"
float determinant3x3(float m1[4][4], int r, int c);
//Returns the transpose of m1
Matrix4 transpose(Matrix4 m1);

//Returns a translation transformation matrix 
Matrix4 tm_translation(float x, float y, float z);
//Returns a scale transformation matrix 
Matrix4 tm_scale(float x, float y, float z);
//Returns a rotation transformation matrix given 
//an axis "a"(0=x, 1=y, 2=z) and an angle "th"
Matrix4 tm_rotation(float th, int a);
//Returns a change of base transformation matrix 
//given ¿?
//Matrix4 TM_changeBase(¿?);**************************************************************

std::ostream &operator<<(std::ostream& os, const Matrix4& m);