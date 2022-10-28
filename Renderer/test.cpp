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
#include "camera.hpp"

using namespace std;

/**
 *
 *   Compilar con
 *   g++ point.cpp direction.cpp test.cpp matrix4.cpp vect4.cpp sphere.cpp station.cpp plane.cpp camera.cpp -o test -std=c++11
 *
 */
int main()
{
    Point center(0, 0, 0);
    Point ref(1, 0, 0);
    Direction axis(0, 0, 2);
    float emission[3] = {0, 0, 0};
    // Direction d1, d2, d3;
    //  Vect4 v(p1);
    //   Matrix4 m1 = tm_rotation(45, 0);

    // Vect4 v1(p1);

    // cout << m1 << endl;
    // cout << inverse(m1) << endl;
    // Sphere sp(axis, center, ref, emission);
    // Station a(sp, 90, 90);

    // cout << a.position << endl;

    // RENDER TESTS

    /*
     *
     * Pastel RGB color codes
     *
     */
    int red[3] = {255, 109, 106};
    int yellow[3] = {233, 236, 107};
    int blue[3] = {139, 211, 230};
    int green[3] = {119, 221, 119};
    int gray[3] = {207, 207, 196};
    int purple[3] = {177, 162, 202};
    int orange[3] = {239, 190, 125};
    /*
       Direction n(-1, 0, 0);
       Plane test_pl(n, 1, black);

       Ray test_ray;
       test_ray.p = Point(0, 0, 0);
       test_ray.d = Direction(1, 0, 0);

       float t;
       Direction sur_normal;
       bool falangista = test_pl.Intersect(test_ray, t, sur_normal);

       cout << "Samiti es facha: " << falangista << endl;
   */

    Point o(0, 0, -3.5);
    Direction l(-1, 0, 0);
    Direction u(0, 1, 0);
    Direction f(0, 0, 3);
    int size[2] = {256, 256};

    Camera camera(l, u, f, o, size);

    vector<Plane> plane_objs;
    Direction n(1, 0, 0);
    Plane left_plane(n, 1, orange);
    plane_objs.push_back(left_plane);
    Direction n1(-1, 0, 0);
    Plane right_plane(n1, 1, yellow);
    plane_objs.push_back(right_plane);
    Direction n2(0, 1, 0);
    Plane floor_plane(n2, 1, gray);
    plane_objs.push_back(floor_plane);
    Direction n3(0, -1, 0);
    Plane ceiling_plane(n3, 1, red);
    plane_objs.push_back(ceiling_plane);
    Direction n4(0, 0, -1);
    Plane back_plane(n4, 1, purple);
    plane_objs.push_back(back_plane);

    vector<Sphere> sphere_objs;
    Point c(-0.5, -0.7, 0.25);
    Sphere left_sphere(c, 0.3, blue);
    sphere_objs.push_back(left_sphere);
    Point c1(0.5, -0.7, -0.25);
    Sphere right_sphere(c1, 0.3, green);
    sphere_objs.push_back(right_sphere);

    camera.render(sphere_objs, plane_objs, 64);
    return 0;
}
