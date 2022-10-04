/**
 * @file test.cpp
 * @author Adrián Yago & Ismael Tienda
 * @brief
 * @version 0.1
 * @date 2022-09-25
 *
 * @copyright Copyright (c) 2022
 *
 */
#include <iostream>
#include "point.hpp"
#include "direction.hpp"
#include "vect4.hpp"
#include "matrix4.hpp"
#include "sphere.hpp"

using namespace std;

/**
 *
 *   Compilar con
 *   g++ point.cpp direction.cpp test.cpp matrix4.cpp vect4.cpp sphere.cpp -o test
 *
 */
int main()
{
    Point p1(1.0, 2.0, 3.0);
    Direction d1(3.0, 4.0, 9.0);
    // Direction d1, d2, d3;
    //  Vect4 v(p1);
    //   Matrix4 m1 = tm_rotation(45, 0);

    // Vect4 v1(p1);

    // cout << m1 << endl;
    // cout << inverse(m1) << endl;
    Sphere(d1, p1, p1);

    return 0;
}