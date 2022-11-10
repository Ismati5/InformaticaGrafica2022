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
#include "direction.hpp"
#include "point.hpp"
#include "matrix4.hpp"
#include "vect4.hpp"

using namespace std;

/**
 *
 *   Compilar con
 *   g++ point.cpp direction.cpp test.cpp matrix4.cpp vect4.cpp sphere.cpp station.cpp plane.cpp camera.cpp -o test -std=c++11
 *
 */
int main()
{
    Point p(0, 0, 0);
    Direction n(0, 1, 0);
    Direction tangent1(n.y, -n.x, 0);
    Direction tangent2 = n.crossProd(tangent1);

    Direction z = n.normalize();
    Direction x = tangent1.normalize();
    Direction y = tangent2.normalize();

    cout << z << endl;
    cout << x << endl;
    cout << y << endl;

    Matrix4 T_change = TM_changeBase(x, y, z, p);
    cout << T_change << endl;

    return 0;
}
