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

using namespace std;

/**
 *
 *   Compilar con
 *   g++ point.cpp direction.cpp test.cpp -o test
 *
 */
int main()
{
    Point p1(1.0, 2.0, 3.0);
    Direction d1;

    cout << p1 - p1 << endl;
    cout << d1.dotProd(d1) << endl;

    return 0;
}