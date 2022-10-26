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
#include "station.hpp"

using namespace std;

/**
 *
 *   Compilar con
 *   g++ point.cpp direction.cpp test.cpp matrix4.cpp vect4.cpp sphere.cpp station.cpp plane.cpp -o test
 *
 */
int main()
{
    Point center(1, 1, 1);
    Point ref(1, 2, 1);
    Direction axis(2, 0, 0);
    float emission[3] = {0, 0, 0};
    // Direction d1, d2, d3;
    //  Vect4 v(p1);
    //   Matrix4 m1 = tm_rotation(45, 0);

    // Vect4 v1(p1);

    // cout << m1 << endl;
    // cout << inverse(m1) << endl;
    Sphere sp(axis, center, ref, emission);
    Station a(sp, 20, 10);

    cout << a.position << endl;
    return 0;
}