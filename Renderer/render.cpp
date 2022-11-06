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
#include <fstream>
#include <string>
#include "point.hpp"
#include "direction.hpp"
#include "vect3.hpp"
#include "vect4.hpp"
#include "matrix4.hpp"
#include "sphere.hpp"
#include "station.hpp"
#include "camera.hpp"
#include "light.hpp"

using namespace std;
#define MAX_POLYGON 30000

Vect3 red = Vect3(255, 109, 106);
Vect3 yellow = Vect3(233, 236, 107);
Vect3 blue = Vect3(153, 255, 255);
Vect3 green = Vect3(119, 221, 119);
Vect3 gray = Vect3(207, 207, 196);
Vect3 light_grey = Vect3(238, 238, 238);
Vect3 purple = Vect3(177, 162, 202);
Vect3 orange = Vect3(239, 190, 125);
Vect3 white = Vect3(255, 255, 255);

Vect3 yellow_backroom_floor = Vect3(198, 197, 139);
Vect3 yellow_backroom_wall = Vect3(228, 230, 168);
Vect3 yellow_backroom_ceiling = Vect3(225, 226, 187);

int Resol_4K[2] = {4096, 2160};
int Resol_1080[2] = {1920, 1080};
int Resol_720[2] = {1280, 720};
int Resol_256[2] = {256, 256};

struct render_config
{
    int *resol{Resol_256};
    int num_tiles_x;
    int num_tiles_y;
    int tile_size{32};
};

int loadOBJfile(Triangle triangles[MAX_POLYGON], string fileName, Vect3 emi)
{

    ifstream file(fileName);

    if (!file.is_open())
    {
        cout << "Error opening de OBJ file" << endl;
        return -1;
    }

    vector<Point> vertices;
    vector<Vect3> faces;

    string line;
    while (getline(file, line))
    {
        if (line.find("v ", 0) == 0) // Vertices
        {
            Point p;
            sscanf(line.c_str(), "v %f %f %f", &p.x, &p.y, &p.z);
            vertices.push_back(p);
        }
        else if (line.find("f ", 0) == 0) // Faces
        {
            Vect3 v;
            float w;
            float trash;
            int matches = sscanf(line.c_str(), "f %f %f %f %f", &v.x, &v.y, &v.z, &w);
            if (matches == 3)
            {
                faces.push_back(v);
            }
            else if (matches == 4)
            {
                faces.push_back(v);
                Vect3 v2(v.x, v.z, w);
                faces.push_back(v2);
            }
        }
    }

    int e = 0;
    for (Vect3 i : faces)
    {
        triangles[e].p1 = vertices.at(i.x - 1);
        triangles[e].p2 = vertices.at(i.y - 1);
        triangles[e].p3 = vertices.at(i.z - 1);
        triangles[e].setEmission(emi);
        triangles[e].sertNormal();
        e++;
    }

    file.close();

    return faces.size();
}

void createRender(string file, int rays)
{
    vector<Object *> objs;
    vector<Light *> lights;
    render_config config;

    // (Para pruebas con .obj)
    /*

        Point o(0, 20, 0);
        Direction l(1, 0, 0);
        Direction u(0, 0, 1);
        Direction f(0, -1, 0);
        int size[2] = {1000, 1000};

        Camera camera(l, u, f, o, size);

        Point l_c1(0, 25, 0);
        Vect3 l_p1 = Vect3(500, 500, 900);
        Light light1(l_c1, l_p1);
        lights.push_back(&light1);

        Triangle triangles[MAX_POLYGON];

        int numPolygons = loadOBJfile(triangles, "objs/DiamondSword.obj", blue);

        for (int i = 0; i < numPolygons; i++)
        {
            objs.push_back(&triangles[i]);
            // cout << triangles[i].p3 << endl;
        }
    */
    
    config.resol = Resol_1080;
    config.num_tiles_x = (config.resol[0] + config.tile_size - 1) / config.tile_size;
    config.num_tiles_y = (config.resol[1] + config.tile_size - 1) / config.tile_size;

    // The bigger shadowBias is, the bigger the difference from reality is
    float shadowBias = 1e-4;

    Point o(0, 0, -3.5);
    Direction l(-1.7777777, 0, 0);
    Direction u(0, 1, 0);
    Direction f(0, 0, 3);

    Camera camera(l, u, f, o, config.resol);

    Point l_c1(0, 0, 0);
    Vect3 l_p1 = Vect3(1, 1, 1);
    Light light1(l_c1, l_p1);
    lights.push_back(&light1);

    Direction n(1, 0, 0);
    Plane left_plane(n, 1, red);
    objs.push_back(&left_plane);
    Direction n1(-1, 0, 0);
    Plane right_plane(n1, 1, green);
    objs.push_back(&right_plane);
    Direction n2(0, 1, 0);
    Plane floor_plane(n2, 1, light_grey);
    objs.push_back(&floor_plane);
    Direction n3(0, -1, 0);
    Plane ceiling_plane(n3, 1, light_grey);
    objs.push_back(&ceiling_plane);
    Direction n4(0, 0, -1);
    Plane back_plane(n4, 1, light_grey);
    objs.push_back(&back_plane);

    Point c(-0.5, -0.7, 0.25);
    Sphere left_sphere(c, 0.3, purple);
    objs.push_back(&left_sphere);
    Point c1(0.5, -0.7, -0.25);
    Sphere right_sphere(c1, 0.3, blue);
    objs.push_back(&right_sphere);

    Point p1(-0.5, -0.7, 0.25);
    Point p2(0, -0.7, 0.25);
    Point p3(-0.25, 0, 0.25);
    Triangle triangle(p1, p2, p3, red);
    // objs.push_back(&triangle);

    // int num_threads = thread::hardware_concurrency();
    // atomic<int> tiles_left = config.num_tiles_x * config.num_tiles_y;

    // vector<thread> threads;
    // for (int i = 0; i < num_threads; i++)
    //     threads.emplace_back(&Camera::render, camera, file, objs, rays, lights, shadowBias);
    
    // for (auto& t : threads) 
    //     t.join();  

    camera.render(file, objs, rays, lights, shadowBias);
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