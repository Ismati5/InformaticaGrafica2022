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
#include <ctime>

#include "point.hpp"
#include "direction.hpp"
#include "vect3.hpp"
#include "vect4.hpp"
#include "matrix4.hpp"
#include "sphere.hpp"
#include "station.hpp"
#include "camera.hpp"
#include "light.hpp"
#include "object.hpp"

using namespace std;

Vect3 red_p = Vect3(250, 35, 43);
Vect3 blue_p = Vect3(153, 255, 255);
Vect3 green_p = Vect3(119, 221, 119);
Vect3 purple_p = Vect3(177, 162, 202);
Vect3 light_grey_p = Vect3(238, 238, 238);

Vect3 red = Vect3(254, 0, 0);
Vect3 blue = Vect3(141, 255, 255);
Vect3 green = Vect3(0, 255, 1);
Vect3 purple = Vect3(228, 170, 255);
Vect3 light_grey = Vect3(199, 199, 199);

Vect3 yellow = Vect3(233, 236, 107);
Vect3 dark_blue = Vect3(0, 23, 49);
Vect3 gray = Vect3(207, 207, 196);
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
int Resol_5[2] = {5, 5};
int Resol_50[2] = {50, 50};
int Resol_256[2] = {256, 256};
int Resol_512[2] = {512, 512};
int Resol_1024[2] = {1024, 1024};
int Resol_2048[2] = {2048, 2048};

/**
 * @brief Create a Render scene
 *
 * @param file
 * @param rays
 */
void renderScene(string file, int rays)
{
    vector<Primitive *> objs;
    vector<Light *> lights;
    render_config config;

    config.resol = Resol_256;
    config.aspect_ratio = float(config.resol[0]) / float(config.resol[1]);
    config.num_tiles_x = (config.resol[0] + config.tile_size - 1) / config.tile_size;
    config.num_tiles_y = (config.resol[1] + config.tile_size - 1) / config.tile_size;
    config.shadow_bias = 1e-4; // The bigger shadowBias is, the bigger the difference from reality is
    config.rays = rays;
    config.outfile = file;
    config.pathtracing = true;
    config.start = clock();
    // config.num_threads = 10;
    config.bounces = 4;

    // Default CORNELL BOX
    Point o(0, 0, -3.5);
    Direction l(-config.aspect_ratio, 0, 0);
    Direction u(0, 1, 0);
    Direction f(0, 0, 3);
    Camera camera(l, u, f, o, config.resol);

    Light light(Point(0, 0.5, 0), Vect3(1, 1, 1));
    lights.push_back(&light);

    Plane left_plane(Direction(1, 0, 0), 1, red);
    objs.push_back(&left_plane);

    Plane right_plane(Direction(-1, 0, 0), 1, green);
    objs.push_back(&right_plane);

    Plane floor_plane(Direction(0, 1, 0), 1, light_grey);
    objs.push_back(&floor_plane);

    Plane ceiling_plane(Direction(0, -1, 0), 1, light_grey);
    // ceiling_plane.setPower(Vect3(1, 1, 1));
    objs.push_back(&ceiling_plane);

    Plane back_plane(Direction(0, 0, -1), 1, light_grey);
    objs.push_back(&back_plane);

    Sphere left_sphere(Point(-0.5, -0.7, 0.25), 0.3, purple);
    objs.push_back(&left_sphere);

    Sphere right_sphere(Point(0.5, -0.7, -0.25), 0.3, blue);
    objs.push_back(&right_sphere);

    // TEST F1 y Tree

    /*Point o(-0.5, 6, 25);
    Direction l(-config.aspect_ratio, 0, 0);
    Direction u(0, 1, 0);
    Direction f(0, 0, -3);
    Camera camera(l, u, f, o, config.resol);

    Light light1(Point(12, 30, 20), Vect3(5000, 5000, 5000));
    lights.push_back(&light1);

    Light light2(Point(0, 5, 6), Vect3(100, 100, 100));
    // lights.push_back(&light2);

    Light light3(Point(0, 5, -5), Vect3(100, 100, 100));
    // lights.push_back(&light3);

    Light light4(Point(-3, 5, 3), Vect3(100, 100, 100));
    // lights.push_back(&light4);

    Light light5(Point(3, 5, 3), Vect3(100, 100, 100));
    // lights.push_back(&light5);

    Light light6(Point(-3, 5, -3), Vect3(100, 100, 100));
    // lights.push_back(&light6);

    Light light7(Point(3, 5, -3), Vect3(100, 100, 100));
    // lights.push_back(&light7);

    Sphere test1(Point(0, 5, 0), 0.3, green);
    // objs.push_back(&test1);

    Sphere test2(Point(0, 5, 6), 0.3, green);
    // objs.push_back(&test2);

    Sphere test3(Point(0, 5, -5), 0.3, green);
    // objs.push_back(&test3);

    Sphere test4(Point(-3, 5, 3), 0.3, green);
    // objs.push_back(&test4);

    Sphere test5(Point(3, 5, 3), 0.3, green);
    // objs.push_back(&test5);

    Sphere test6(Point(-3, 5, -3), 0.3, green);
    // objs.push_back(&test6);

    Sphere test7(Point(3, 5, -3), 0.3, green);
    // objs.push_back(&test7);

    Plane floor_plane(Direction(0, 1, 0), 1, green);
    objs.push_back(&floor_plane);

    Plane ceiling_plane(Direction(0, -1, 0), 10, light_grey);
    // objs.push_back(&ceiling_plane);

    Plane back_plane(Direction(0, 0, 1), 50, blue);
    objs.push_back(&back_plane);

    Plane left_plane(Direction(1, 0, 0), 8, light_grey);
    // objs.push_back(&left_plane);

    Plane right_plane(Direction(-1, 0, 0), 8, light_grey);
    // objs.push_back(&right_plane);

    // Object F1("F1", "objs/F1.obj", red);
    // F1.translate(Direction(0, -1, -0.5));

    Object Tree("Tree", "objs/tree.obj", purple);
    Tree.scale(-2);
    Tree.translate(Direction(0, -1, 0));

    for (int i = 0; i < Tree.getPolygons(); i++)
    {
        objs.push_back(&Tree.getTriangles()[i]);
    }*/

    // Multi-Threading rendering
    static atomic<int>
        tiles_left;
    tiles_left = config.num_tiles_x * config.num_tiles_y;

    static atomic<int> max_emission;
    max_emission = 0;

    vector<thread> threads;
    config.content = (Vect3 *)malloc(config.resol[0] * config.resol[1] * sizeof(Vect3));
    for (int i = 0; i < config.num_threads; i++)
        threads.emplace_back(&Camera::render_thread, camera, i, objs, lights, ref(config), ref(tiles_left), ref(max_emission));

    for (auto &t : threads)
        t.join();

    //  Saving file
    cout << "> Progress   [||||||||||||||||||||||||||||||||||||||||||||||||||] - 100%        (Saving image ...)\r";
    cout.flush();

    ofstream file_stream("renders/" + config.outfile);
    file_stream << "P3" << endl;
    file_stream << "#MAX=255" << endl;
    file_stream << config.resol[0] << " " << config.resol[1] << endl;
    file_stream << max_emission << endl;

    for (float i = 0; i < config.resol[1]; i++) // i rows
    {
        for (float j = 0; j < config.resol[0]; j++) // j columns
        {
            file_stream << config.content[(int)((i * config.resol[0] + j))].x << " " << config.content[(int)((i * config.resol[0] + j))].y << " " << config.content[(int)((i * config.resol[0] + j))].z << "    ";
        }
        file_stream << endl;
    }

    file_stream.close();
    cout << "> Progress   [||||||||||||||||||||||||||||||||||||||||||||||||||] - 100%         (Saving completed!)\r" << endl;
    cout.flush();

    if (double(clock() - config.start) / CLOCKS_PER_SEC > 60)
    {
        cout << "> Completed in " << int(double(clock() - config.start) / CLOCKS_PER_SEC) / 60 << "min " << int(double(clock() - config.start / CLOCKS_PER_SEC)) % 60 << "s! File saved as renders/" << config.outfile << "." << endl
             << endl;
    }
    else
    {
        cout << "> Completed in " << int(double(clock() - config.start) / CLOCKS_PER_SEC) << "s! File saved as renders/" << config.outfile << "." << endl
             << endl;
    }

    free(config.content);
}

int main(int argc, char *argv[])
{

    if (argc == 3)
    {
        renderScene(argv[1], stoi(argv[2]));
    }
    else
    {
        cout << "[!] Usage: renderer <filename.ppm> <rays per pixel>" << endl;
    }

    return 0;
}