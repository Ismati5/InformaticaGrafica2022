/**
 * @file render.cpp
 * @author Adrián Yago & Ismael Tienda
 * @brief
 * @version 0.1
 * @date 2022-10-28
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
#include "light.hpp"

using namespace std;

int red[3] = {255, 109, 106};
int yellow[3] = {233, 236, 107};
int blue[3] = {139, 211, 230};
int green[3] = {119, 221, 119};
int gray[3] = {207, 207, 196};
int purple[3] = {177, 162, 202};
int orange[3] = {239, 190, 125};

void createRender(string file, int rays)
{
    Point o(0, 0, -3.5);
    Direction l(1, 0, 0);
    Direction u(0, 1, 0);
    Direction f(0, 0, 3);
    int size[2] = {256, 256};

    Camera camera(l, u, f, o, size);

    vector<Light *> lights; 
    Point l_c1(0, 0.5, 0);
    float l_p1[3] = {1, 1, 1};
    Light light1(l_c1, l_p1);
    lights.push_back(&light1);

    vector<Object *> objs;

    Direction n(1, 0, 0);
    Plane left_plane(n, 1, orange);
    objs.push_back(&left_plane);
    Direction n1(-1, 0, 0);
    Plane right_plane(n1, 1, yellow);
    objs.push_back(&right_plane);
    Direction n2(0, 1, 0);
    Plane floor_plane(n2, 1, gray);
    objs.push_back(&floor_plane);
    Direction n3(0, -1, 0);
    Plane ceiling_plane(n3, 1, red);
    objs.push_back(&ceiling_plane);
    Direction n4(0, 0, -1);
    Plane back_plane(n4, 1, purple);
    objs.push_back(&back_plane);

    Point c(-0.5, -0.7, 0.25);
    Sphere left_sphere(c, 0.3, blue);
    objs.push_back(&left_sphere);
    Point c1(0.5, -0.7, -0.25);
    Sphere right_sphere(c1, 0.3, green);
    objs.push_back(&right_sphere);

    camera.render(file, objs, rays, lights);
}

int main(int argc, char *argv[])
{

    if (argc == 3)
    {
        createRender(argv[1], stoi(argv[2]));
    }
    else
    {
        cout << "[!] Usage: renderer <filename.ppm> <rays per pixel>" << endl;
    }

    return 0;
}