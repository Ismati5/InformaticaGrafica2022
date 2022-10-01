/**
 * @file matrix4.cpp
 * @author Adrián Yago & Ismael Tienda
 * @brief
 * @version 0.1
 * @date 2022-09-25
 *
 * @copyright Copyright (c) 2022
 *
 */

#include <iomanip>
#include "matrix4.hpp"

Matrix4 Matrix4::operator*(const Matrix4 &m) const {
    Matrix4 m1;

    for(int i = 0; i < 4; i++) {
        for(int j = 0; j < 4; j++) {
            for(int k = 0; k < 4; k++){
                m1.m[i][j] += this->m[i][k] * m.m[k][j];
            }
        }
    }

    return m1;
}

Vect4 Matrix4::operator*(const Vect4 &v) const {
    Point p1;
    Vect4 m1(p1);
    m1.v[3] = 0;
    float k;

    for(int i = 0; i < 4; i++) {
        for(int j = 0; j < 4; j++) {
            m1.v[i] += m[i][j] * v.v[j];
        }
    }

    return m1;
}

Matrix4 Matrix4::operator/(const float &n) const {
    Matrix4 m1;

    for(int i = 0; i < 4; i++)
        for(int j = 0; j < 4; j++)
            m1.m[i][j] = this->m[i][j] / n;

    return m1;

}

Matrix4 inverse(Matrix4 m1){
    float det = determinant4x4(m1.m);
    Matrix4 adj;

    for(int i = 0; i < 4; i++)
        for(int j = 0; j < 4; j++){
            if ((j % 2 != 0 && i % 2 == 0) || 
                (j % 2 == 0 && i % 2 != 0))
                adj.m[i][j] = -determinant3x3(m1.m,i,j);
            else 
                adj.m[i][j] = determinant3x3(m1.m,i,j);
            
        }
    return transpose(adj) / det;
}

Matrix4 transpose(Matrix4 m1){
    Matrix4 m2;

    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++)
            m2.m[i][j] = m1.m[j][i];

    return m2; 

}

float determinant4x4(float m1[4][4]){
    double c, r=1;

    for(int i = 0; i < 4; i++) {
        for(int k = i+1; k < 4; k++) {
            c = m1[k][i] / m1[i][i];
            for(int j = i; j < 4; j++)
                m1[k][j] = m1[k][j] - c*m1[i][j];
        }
    }
    for (int i = 0; i < 4; i++)
        r *=m1[i][i];
    return r;

}

float determinant3x3(float m1[4][4], int r, int c){
    float mAux[3][3];

    int i2 = 0, j2 = 0;

    for(int i = 0; i < 4; i++, i2++) {
        if (i == r) i++;
        if (i == 4) break;
        for(int j = 0; j < 4; j++, j2++) {
            if (j == c) j++;
            if (j == 4) break;
            mAux[i2][j2] = m1[i][j];
        }
        j2 = 0;
    }

    return (mAux[0][0] * mAux[1][1] * mAux[2][2] +
        mAux[0][1] * mAux[1][2] * mAux[2][0] + 
        mAux[1][0] * mAux[2][1] * mAux[0][2]) -
        (mAux[0][2] * mAux[1][1] * mAux[2][0] +
        mAux[0][1] * mAux[1][0] * mAux[2][2] + 
        mAux[1][2] * mAux[2][1] * mAux[0][0]); 

}


Matrix4 tm_translation(float x, float y, float z){
    Matrix4 m1;

    m1.m[0][0] = 1;
    m1.m[1][1] = 1;
    m1.m[2][2] = 1;
    m1.m[3][3] = 1;

    m1.m[0][3] = x;
    m1.m[1][3] = y;
    m1.m[2][3] = z;

    return m1;
}

Matrix4 tm_scale(float x, float y, float z){
    Matrix4 m1;

    m1.m[0][0] = x;
    m1.m[1][1] = y;
    m1.m[2][2] = z;
    m1.m[3][3] = 1;

    return m1;
}

Matrix4 tm_rotation(float th, int a){

    Matrix4 m1;

    switch (a){
    case 0: //x
        m1.m[0][0] = 1;
        m1.m[1][1] = cos(th);
        m1.m[1][2] = -sin(th);
        m1.m[2][2] = cos(th);
        m1.m[2][1] = sin(th);
        m1.m[3][3] = 1;
        break;
    case 1: //y
        m1.m[0][0] = cos(th);
        m1.m[0][2] = sin(th);
        m1.m[1][1] = 1;
        m1.m[2][0] = -sin(th);
        m1.m[2][2] = cos(th);
        m1.m[3][3] = 1;
        break;
    case 2: //z
        m1.m[0][0] = cos(th);
        m1.m[0][1] = -sin(th);
        m1.m[1][0] = sin(th);
        m1.m[1][1] = cos(th);
        m1.m[2][2] = 1;
        m1.m[3][3] = 1;
        break;
    }

    return m1;
}

/**
 * @brief operator<<
 *
 * @param os
 * @param v
 * @return ostream&
 */
ostream &operator<<(ostream &os, const Matrix4 &m)
{
    os << "/ ";
    for(int i = 0; i < 4; i++) os << setw(4) << fixed << setprecision(2) << m.m[0][i] << " ";
    os << "\\" << endl << "| " ;
    for(int i = 0; i < 4; i++) os << setw(4) << fixed << setprecision(2) << m.m[1][i] << " ";
    os << "|" << endl << "| ";
    for(int i = 0; i < 4; i++) os << setw(4) << fixed << setprecision(2) << m.m[2][i] << " ";
    os << "|" << endl << "\\ ";
    for(int i = 0; i < 4; i++) os << setw(4) << fixed << setprecision(2) << m.m[3][i] << " ";
    os  << "/" << endl;

    return os;
};