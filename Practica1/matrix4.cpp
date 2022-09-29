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

Matrix4 Matrix4::inverse() const {

    /*float det = determinant(m);

    for(int i = 0; i < 4; i++){
        for(int j = 0; j < 4; j++)
        cout<<((mat[(j+1)%3][(i+1)%3] * mat[(j+2)%3][(i+2)%3]) - (mat[(j+1)%3][(i+2)%3] * mat[(j+2)%3][(i+1)%3]))/ determinant<<"\t";*/

}

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


float determinant(float m1[4][4] ) {

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

Matrix4 tm_rotation(float th){
    Matrix4 m1;

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