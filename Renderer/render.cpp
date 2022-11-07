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
#define MAX_POLYGON 20000

Vect3 red = Vect3(255, 109, 106);
Vect3 yellow = Vect3(233, 236, 107);
Vect3 blue = Vect3(153, 255, 255);
Vect3 dark_blue = Vect3(0, 23, 49);
Vect3 green = Vect3(119, 221, 119);
Vect3 gray = Vect3(207, 207, 196);
Vect3 light_grey = Vect3(238, 238, 238);
Vect3 purple = Vect3(177, 162, 202);
Vect3 orange = Vect3(255, 153, 51);
Vect3 white = Vect3(255, 255, 255);
Vect3 black = Vect3(0, 0, 0);

Vect3 yellow_backroom_floor = Vect3(198, 197, 139);
Vect3 yellow_backroom_wall = Vect3(228, 230, 168);
Vect3 yellow_backroom_ceiling = Vect3(225, 226, 187);

// 32:9
int Resol_4K_p[2] = {4096 * 2, 2160};
int Resol_1080_p[2] = {1920 * 2, 1080};
int Resol_720_p[2] = {1280 * 2, 720};
int Resol_480_p[2] = {854 * 2, 480};
int Resol_360_p[2] = {640 * 2, 360};
int Resol_240_p[2] = {426 * 2, 240};

// 16:9
int Resol_8K[2] = {7680, 4320};
int Resol_4K[2] = {4096, 2160};
int Resol_1080[2] = {1920, 1080};
int Resol_720[2] = {1280, 720};
int Resol_480[2] = {854, 480};
int Resol_360[2] = {640, 360};
int Resol_240[2] = {426, 240};

// 1:1
int Resol_256[2] = {256, 256};
int Resol_512[2] = {512, 512};
int Resol_1024[2] = {1024, 1024};

int loadOBJfile(Triangle triangles[MAX_POLYGON], string fileName, Vect3 emi, float scale)
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

            if (scale < 0)
            {
                p = p / abs(scale);
            }
            else
            {
                p = p * scale;
            }

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

int loadOBJfile(Triangle triangles[MAX_POLYGON], string fileName, Vect3 emi, Direction direction)
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

            p = p + direction;

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

    config.resol = Resol_240;
    config.aspect_ratio = float(config.resol[0]) / float(config.resol[1]);
    config.num_tiles_x = (config.resol[0] + config.tile_size - 1) / config.tile_size;
    config.num_tiles_y = (config.resol[1] + config.tile_size - 1) / config.tile_size;
    config.shadow_bias = 1e-4; // The bigger shadowBias is, the bigger the difference from reality is
    config.rays = rays;
    config.outfile = file;
    config.pathtracing = true;

    Point o(0, 5, 25);
    Direction l(config.aspect_ratio, 0, 0);
    Direction u(0, 1, 0);
    Direction f(0, -0.1, -2);
    Camera camera(l, u, f, o, config.resol);

    Point l_c1(0, 0, 0);
    Vect3 l_p1 = Vect3(1, 1, 1);
    Light light1(l_c1, l_p1);
    lights.push_back(&light1);

    Light light2(Point(0, 0, 4), Vect3(1, 1, 1));
    lights.push_back(&light2);

    Light light3(Point(0, 0, -4), Vect3(1, 1, 1));
    lights.push_back(&light3);

    Light light4(Point(0, 0, 2), Vect3(1, 1, 1));
    lights.push_back(&light4);

    Light light5(Point(0, 0, -2), Vect3(1, 1, 1));
    lights.push_back(&light5);

    Light light6(Point(0, 0, 7), Vect3(1, 1, 1));
    lights.push_back(&light6);

    Light light7(Point(0, 0, -6), Vect3(1, 1, 1));
    lights.push_back(&light7);

    Light light8(Point(-0.5, 0, 6.5), Vect3(1, 1, 1));
    // lights.push_back(&light8);

    Light light9(Point(0.5, 0, 6.5), Vect3(1, 1, 1));
    // lights.push_back(&light9);

    Triangle triangles[MAX_POLYGON];

    int numPolygons = loadOBJfile(triangles, "objs/F1.obj", red, Direction(0, -1, 0));

    for (int i = 0; i < numPolygons; i++)
    {
        objs.push_back(&triangles[i]);
        //  cout << triangles[i].p3 << endl;
    }

    Direction n(1, 0, 0);
    Plane left_plane(n, 10, purple);
    // objs.push_back(&left_plane);
    Direction n1(-1, 0, 0);
    Plane right_plane(n1, 10, purple);
    // objs.push_back(&right_plane);
    Direction n2(0, 1, 0);
    Plane floor_plane(n2, 1, purple);
    objs.push_back(&floor_plane);
    Direction n3(0, -1, 0);
    Plane ceiling_plane(n3, 1, purple);
    // objs.push_back(&ceiling_plane);
    Direction n4(0, 0, 1);
    Plane back_plane(n4, 15, purple);
    objs.push_back(&back_plane);

    Sphere left_sphere(Point(0, 0, 0), 0.5, green);
    // objs.push_back(&left_sphere);

    // Point c11(0, -1.63, 0);
    // Sphere left_sphere2(c11, 0.8, dark_blue);
    // objs.push_back(&left_sphere2);

    // Point c2(0.2, -0.33, -0.4);
    // Sphere eye_l(c2, 0.1, white);
    // objs.push_back(&eye_l);
    // Point c3(-0.2, -0.33, -0.4);
    // Sphere eye_r(c3, 0.1, white);
    // objs.push_back(&eye_r);

    // Point c4(0.2, -0.33, -0.5);
    // Sphere eye2_l(c4, 0.04, black);
    // objs.push_back(&eye2_l);
    // Point c5(-0.2, -0.33, -0.5);
    // Sphere eye2_r(c5, 0.04, black);
    // objs.push_back(&eye2_r);

    // Point c6(0, -0.46, -0.45);
    // Sphere mouth(c6, 0.1, orange);
    // objs.push_back(&mouth);

    // Point c(-0.5, -0.7, 0.25);
    // Sphere left_sphere(c, 0.3, purple);
    //  objs.push_back(&left_sphere);
    // Point c1(0.5, -0.7, -0.25);
    // Sphere right_sphere(c1, 0.3, blue);
    // objs.push_back(&right_sphere);

    // Point p1(-0.5, -0.7, 0.25);
    // Point p2(0, -0.7, 0.25);
    // Point p3(-0.25, 0, 0.25);
    // Triangle triangle(p1, p2, p3, red);
    //  objs.push_back(&triangle);

    // int num_threads = thread::hardware_concurrency();
    // atomic<int> tiles_left = config.num_tiles_x * config.num_tiles_y;

    // vector<thread> threads;
    // config.content = (Vect3*)malloc(config.resol[0] * config.resol[1] * 3);
    // for (int i = 0; i < num_threads; i++)
    //     threads.emplace_back(&Camera::render, camera, file, objs, rays, lights, shadowBias);

    // for (auto& t : threads)
    //     t.join();

    camera.render(objs, lights, config);
}

void renderObj(string file, int rays)
{
    // (Para pruebas con .obj)
    vector<Object *> objs;
    vector<Light *> lights;
    render_config config;

    config.resol = Resol_240;
    config.num_tiles_x = (config.resol[0] + config.tile_size - 1) / config.tile_size;
    config.num_tiles_y = (config.resol[1] + config.tile_size - 1) / config.tile_size;
    config.shadow_bias = 1e-4; // The bigger shadowBias is, the bigger the difference from reality is
    config.rays = rays;
    config.outfile = file;

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

    camera.render(objs, lights, config);
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