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
#include "photon.hpp"

using namespace std;

// Colors
Vect3 red_p = Vect3(250, 35, 43);
Vect3 blue_p = Vect3(153, 255, 255);
Vect3 green_p = Vect3(119, 221, 119);
Vect3 purple_p = Vect3(177, 162, 202);
Vect3 light_grey_p = Vect3(238, 238, 238);

Vect3 red = Vect3(204, 0, 0);
Vect3 blue = Vect3(143, 255, 255);
Vect3 green = Vect3(0, 255, 0);
Vect3 purple = Vect3(228, 172, 255);
Vect3 light_grey = Vect3(231, 230, 230);

Vect3 grey = Vect3(112, 128, 144);

Vect3 yellow = Vect3(233, 236, 107);
Vect3 dark_blue = Vect3(0, 23, 49);
Vect3 gray = Vect3(207, 207, 196);
Vect3 orange = Vect3(255, 153, 51);
Vect3 white = Vect3(255, 255, 255);
Vect3 black = Vect3(0, 0, 0);

Vect3 none = Vect3(0, 0, 0);

// Materials
//                          kd                  ks              kt              ke              ref     absortion
Material diff_red(red, none, none, none, 0.2);
Material diff_purple(purple, none, none, none, 0.2);
Material diff_blue(blue, none, none, none, 0.2);
Material diff_green(green, none, none, none, 0.2);
Material diff_light_grey(light_grey, none, none, none, 0.2);
Material diff_grey(grey, none, none, none, 0.2);
Material diff_spec_blue(blue, light_grey, none, none, 0.2);
Material diff_spec_red(red, light_grey, none, none, 0.2);

Material spec(none, none, none, 1.5, 0.2);

Material refr(none, none, white, none, 1.5, 0.2);

Material spec_refr(none, Vect3(30, 30, 30), white, none, 1.5, 0.2);

Material em_light_grey(light_grey, none, none, light_grey, 0.2);
Material em_light_blue(blue_p, none, none, blue_p, 0.2);

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
int Resol_400[2] = {400, 400};
int Resol_512[2] = {512, 512};
int Resol_1024[2] = {1024, 1024};
int Resol_2048[2] = {2048, 2048};

void renderPhotonMapping(string file, int max_photons)
{
    vector<Primitive *> objs;
    vector<Light *> lights;
    render_config config;

    config.resol = Resol_256;
    config.aspect_ratio = float(config.resol[0]) / float(config.resol[1]);
    config.num_tiles_x = (config.resol[0] + config.tile_size - 1) / config.tile_size;
    config.num_tiles_y = (config.resol[1] + config.tile_size - 1) / config.tile_size;
    config.shadow_bias = 1e-4; // The bigger shadowBias is, the bigger the difference from reality is
    config.outfile = file;
    config.pathtracing = true;
    config.start = clock();
    config.num_threads = 6;

    config.max_photons = max_photons;
    config.k = 10;
    config.r = 2;

    // Default CORNELL BOX
    Point o(0, 0, -3.5);
    Direction l(-config.aspect_ratio, 0, 0);
    Direction u(0, 1, 0);
    Direction f(0, 0, 3);
    Camera camera(l, u, f, o, config.resol);

    Light light(Point(0, 0.5, 0), white);
    lights.push_back(&light);

    Plane left_plane(Direction(1, 0, 0), 1, "red_plane", diff_red);
    objs.push_back(&left_plane);

    Plane right_plane(Direction(-1, 0, 0), 1, "green_plane", diff_green);
    objs.push_back(&right_plane);

    Plane floor_plane(Direction(0, 1, 0), 1, "floor_plane", diff_light_grey);
    objs.push_back(&floor_plane);

    Plane ceiling_plane(Direction(0, -1, 0), 1, "ceiling_plane", diff_light_grey);
    objs.push_back(&ceiling_plane);

    Plane back_plane(Direction(0, 0, -1), 1, "back_plane", diff_light_grey);
    objs.push_back(&back_plane);

    Sphere left_sphere(Point(-0.5, -0.7, 0.25), 0.3, "blue_sphere", diff_blue);
    objs.push_back(&left_sphere);

    Sphere right_sphere(Point(0.5, -0.7, -0.25), 0.3, "red_sphere", diff_red);
    objs.push_back(&right_sphere);

    PhotonMap map = generation_of_photon_map(lights, objs, config);
}

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
    config.num_threads = 12;

    // Default CORNELL BOX
    /*Point o(0, 0, -3.5);
    Direction l(-config.aspect_ratio, 0, 0);
    Direction u(0, 1, 0);
    Direction f(0, 0, 3);
    Camera camera(l, u, f, o, config.resol);

    // sLight light(Point(0, 0.5, 0), white);
    // lights.push_back(&light);

    Plane left_plane(Direction(1, 0, 0), 1, "red_plane", diff_red);
    objs.push_back(&left_plane);

    Plane right_plane(Direction(-1, 0, 0), 1, "green_plane", diff_green);
    objs.push_back(&right_plane);

    Plane floor_plane(Direction(0, 1, 0), 1, "floor_plane", diff_light_grey);
    objs.push_back(&floor_plane);

    Plane ceiling_plane(Direction(0, -1, 0), 1, "ceiling_plane", em_light_grey);
    objs.push_back(&ceiling_plane);

    Plane back_plane(Direction(0, 0, -1), 1, "back_plane", diff_light_grey);
    objs.push_back(&back_plane);

    Sphere left_sphere(Point(-0.5, -0.7, 0.25), 0.3, "blue_plastic_sphere", diff_spec_blue);
    objs.push_back(&left_sphere);

    Sphere right_sphere(Point(0.5, -0.7, -0.25), 0.3, "refraction_sphere", spec_refr);
    objs.push_back(&right_sphere);
    */

    // TEST FIAT
    Point o(0, 1.6, 8);
    Direction l(config.aspect_ratio, 0, 0);
    Direction u(0, 1, 0);
    Direction f(0, 0, -3);
    Camera camera(l, u, f, o, config.resol);

    Plane floor_plane(Direction(0, 1, 0), 1, "floor_plane", diff_grey);
    objs.push_back(&floor_plane);

    Plane back_pane(Direction(0, 0, 1), 15, "back_pane", diff_light_grey);
    // objs.push_back(&back_pane);

    Plane ceiling_plane(Direction(0, -1, 0), 30, "ceiling_plane", em_light_blue);
    objs.push_back(&ceiling_plane);

    Object Fiat("Fiat", "objs/FiatUno.obj", diff_spec_red);
    Fiat.translate(Direction(0, -1, -4));

    for (int i = 0; i < Fiat.getPolygons(); i++)
    {
        objs.push_back(Fiat.getTriangles(i));
    }

    // TEST TREE
    /*Point o(-0.5, 6, 25);
    Direction l(-config.aspect_ratio, 0, 0);
    Direction u(0, 1, 0);
    Direction f(0, 0, -3);
    Camera camera(l, u, f, o, config.resol);

    Light light1(Point(12, 30, 20), Vect3(5000, 5000, 5000));
    lights.push_back(&light1);

    Plane floor_plane(Direction(0, 1, 0), 1, "floor_plane", diff_green);
    objs.push_back(&floor_plane);

    Plane ceiling_plane(Direction(0, -1, 0), 10, "ceiling_plane", em_light_grey);
    objs.push_back(&ceiling_plane);

    Object Tree("Tree", "objs/tree.obj", diff_purple);
    Tree.scale(-2);
    Tree.translate(Direction(0, -1, 0));

    for (int i = 0; i < Tree.getPolygons(); i++)
    {
        objs.push_back(Tree.getTriangles(i));
    }*/

    // TEST F1

    /*Point o(0, 5, 25);
    Direction l(config.aspect_ratio, 0, 0);
    Direction u(0, 1, 0);
    Direction f(0, 0, -3);
    Camera camera(l, u, f, o, config.resol);

    Plane floor_plane(Direction(0, 1, 0), 1, "floor_plane", diff_grey);
    objs.push_back(&floor_plane);

    Plane back_pane(Direction(0, 0, 1), 15, "back_pane", diff_light_grey);
    objs.push_back(&back_pane);

    Plane ceiling_plane(Direction(0, -1, 0), 30, "ceiling_plane", em_light_grey);
    objs.push_back(&ceiling_plane);

    Object F1("F1", "objs/F1.obj", diff_spec_red);
    F1.translate(Direction(0, -1, -0.5));

    for (int i = 0; i < F1.getPolygons(); i++)
    {
        objs.push_back(F1.getTriangles(i));
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

    if (argc == 4)
    {
        if (stoi(argv[3]) == 0) // non photon mapping
        {
            renderScene(argv[1], stoi(argv[2]));
        }
        else
        {
            renderPhotonMapping(argv[1], stoi(argv[2]));
        }
    }
    else
    {
        cout << "[!] Usage: renderer <filename.ppm> <rays per pixel> <photon mapping>" << endl;
    }

    return 0;
}