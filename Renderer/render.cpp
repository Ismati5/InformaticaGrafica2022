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

/**
 * @brief Create a Render scene
 *
 * @param file
 * @param rays
 */
void renderScene(string file, int rays)
{
    vector<Object *> objs;
    vector<Light *> lights;
    render_config config;

    config.resol = Resol_720;
    config.aspect_ratio = float(config.resol[0]) / float(config.resol[1]);
    config.num_tiles_x = (config.resol[0] + config.tile_size - 1) / config.tile_size;
    config.num_tiles_y = (config.resol[1] + config.tile_size - 1) / config.tile_size;
    config.shadow_bias = 1e-4; // The bigger shadowBias is, the bigger the difference from reality is
    config.rays = rays;
    config.outfile = file;
    config.pathtracing = true;
    config.start = clock();
    // config.num_threads = 1;

    // Default CORNELL BOX
    /*Point o(0, 0, -3.5);
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
    objs.push_back(&ceiling_plane);

    Plane back_plane(Direction(0, 0, -1), 1, light_grey);
    objs.push_back(&back_plane);

    Sphere left_sphere(Point(-0.5, -0.7, 0.25), 0.3, purple);
    objs.push_back(&left_sphere);

    Sphere right_sphere(Point(0.5, -0.7, -0.25), 0.3, blue);
    objs.push_back(&right_sphere);

    Triangle triangle_top(Point(0.507443, 0.662495, 2.225192), Point(1.007443, 0.452080, -2.174808), Point(0.952443, 0.356765, -2.274808), red);
    // objs.push_back(&triangle_top);*/

    // TEST F1

    Point o(0, 5, 25);
    Direction l(config.aspect_ratio, 0, 0);
    Direction u(0, 1, 0);
    Direction f(0, -0.1, -3);
    Camera camera(l, u, f, o, config.resol);

    Light light1(Point(0, 6, 0), Vect3(1, 1, 1));
    lights.push_back(&light1);

    Light light2(Point(0, 5, 6), Vect3(1, 1, 1));
    lights.push_back(&light2);

    Light light3(Point(0, 5, -5), Vect3(1, 1, 1));
    lights.push_back(&light3);

    Light light4(Point(-3, 5, 3), Vect3(1, 1, 1));
    lights.push_back(&light4);

    Light light5(Point(3, 5, 3), Vect3(1, 1, 1));
    lights.push_back(&light5);

    Light light6(Point(-3, 5, -3), Vect3(1, 1, 1));
    lights.push_back(&light6);

    Light light7(Point(3, 5, -3), Vect3(1, 1, 1));
    lights.push_back(&light7);

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

    Plane floor_plane(Direction(0, 1, 0), 1, light_grey);
    objs.push_back(&floor_plane);

    Plane ceiling_plane(Direction(0, -1, 0), 10, light_grey);
    objs.push_back(&ceiling_plane);

    Plane back_plane(Direction(0, 0, 1), 15, light_grey);
    objs.push_back(&back_plane);

    Plane left_plane(Direction(1, 0, 0), 8, light_grey);
    objs.push_back(&left_plane);

    Plane right_plane(Direction(-1, 0, 0), 8, light_grey);
    objs.push_back(&right_plane);

    Triangle triangles[MAX_POLYGON];

    int numPolygons = loadOBJfile(triangles, "objs/F1.obj", red, Direction(0, -1, 0));

    for (int i = 0; i < numPolygons; i++)
    {
        objs.push_back(&triangles[i]);
        //  cout << triangles[i].p3 << endl;
    }

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

/**
 * @brief Render just an .obj object
 *
 * @param file
 * @param rays
 */
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

    // camera.render(objs, lights, config);
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