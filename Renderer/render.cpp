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
#include <iomanip>
#include <windows.h>
#include <ShellApi.h>

#include "station.hpp"
#include "camera.hpp"
#include "object.hpp"
#include "photonMap.cpp"

using namespace std;

// Colors
Vect3 red_p = Vect3(250, 35, 43);
Vect3 blue_p = Vect3(153, 255, 255);
Vect3 green_p = Vect3(119, 221, 119);
Vect3 purple_p = Vect3(177, 162, 202);
Vect3 light_grey_p = Vect3(238, 238, 238);

Vect3 red = Vect3(255, 40, 0);
Vect3 blue = Vect3(143, 255, 255);
Vect3 green = Vect3(0, 255, 0);
Vect3 purple = Vect3(228, 172, 255);
Vect3 dark_purple = Vect3(156, 12, 153);
Vect3 light_grey = Vect3(231, 230, 230);

Vect3 grey = Vect3(112, 128, 144);

Vect3 yellow = Vect3(233, 236, 107);
Vect3 dark_blue = Vect3(0, 11, 141);
Vect3 gray = Vect3(207, 207, 196);
Vect3 orange = Vect3(255, 153, 51);
Vect3 white = Vect3(255, 255, 255);
Vect3 black = Vect3(0, 0, 0);

Vect3 none = Vect3(0, 0, 0);

// Materials
//                          kd                  ks              kt              ke              ref     absortion
Material diff_red(red, none, none, none, 0.2);
Material diff_black(none, none, none, none, 0.2);
Material diff_orange(orange, none, none, none, 0.2);
Material diff_purple(purple, none, none, none, 0.2);
Material diff_dark_purple(dark_purple, none, none, none, 0.2);
Material diff_blue(blue, none, none, none, 0.2);
Material diff_dark_blue(dark_blue, none, none, none, 0.2);
Material diff_green(green, none, none, none, 0.2);
Material diff_light_grey(light_grey, none, none, none, 0.2);
Material diff_grey(grey, none, none, none, 0.2);
Material diff_spec_blue(blue, light_grey, none, none, 0.2);
Material diff_spec_dark_blue(dark_blue, light_grey, none, none, 0.2);
Material diff_spec_red(red, light_grey, none, none, 0.2);
Material diff_spec_purple(purple, light_grey, none, none, 0.2);

Material spec(grey, grey, none, none, 0.2);

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

/**
 * @brief Returns color from string
 *
 * @param name
 * @return Vect3
 */
Vect3 stringToColor(string name)
{
    if (name == "WHITE")
        return white;
    else
    {
        cout << "[!] Invalid color used: " << name << endl;
        exit(1);
    }
}

/**
 * @brief Returns material from string
 *
 * @param name
 * @return Material
 */
Material stringToMaterial(string name)
{
    if (name == "DIFF_RED")
        return diff_red;
    else if (name == "DIFF_PURPLE")
        return diff_purple;
    else if (name == "DIFF_DARK_PURPLE")
        return diff_dark_purple;
    else if (name == "DIFF_BLUE")
        return diff_blue;
    else if (name == "DIFF_DARK_BLUE")
        return diff_dark_blue;
    else if (name == "DIFF_GREEN")
        return diff_green;
    else if (name == "DIFF_LIGHT_GREY")
        return diff_light_grey;
    else if (name == "DIFF_GREY")
        return diff_grey;
    else if (name == "DIFF_ORANGE")
        return diff_orange;
    else if (name == "DIFF_BLACK")
        return diff_black;
    else if (name == "DIFF_SPEC_BLUE")
        return diff_spec_blue;
    else if (name == "DIFF_SPEC_RED")
        return diff_spec_red;
    else if (name == "DIFF_SPEC_DARK_BLUE")
        return diff_spec_dark_blue;
    else if (name == "DIFF_SPEC_PURPLE")
        return diff_spec_purple;
    else if (name == "SPEC_REFR")
        return spec_refr;
    else if (name == "SPEC")
        return spec;
    else if (name == "REFR")
        return refr;
    else if (name == "EM_LIGHT_GREY")
        return em_light_grey;
    else if (name == "EM_LIGHT_BLUE")
        return em_light_blue;
    else
    {
        cout << "[!] Invalid material used: " << name << endl;
        exit(1);
    }
}

/**
 * @brief Load scene from a file
 *
 * @param file
 * @param objs
 * @param lights
 * @return render_config
 */
render_config loadScene(string file, Camera &camera, vector<Primitive *> &objs, vector<Light *> &lights)
{
    ifstream file_stream("scenes/" + file + ".dat"); // Scene file
    render_config config;

    if (file_stream.is_open())
    {
        cout << endl
             << "1 > Loading " << file + ".dat"
             << " scene ..." << endl;
    }
    else
    {
        cout << "[!] Failed loading " << file + ".dat"
             << " file!" << endl;
        exit(1);
    }

    string data;

    file_stream >> data;

    while (!file_stream.eof())
    {

        if (data == "#")
        {
            getline(file_stream, data);
        }
        else
        {
            if (data == "RESOLUTION") // Set scene resolution
            {
                int resol1, resol2;
                file_stream >> resol1 >> resol2;
                config.resol[0] = resol1;
                config.resol[1] = resol2;
                config.aspect_ratio = float(config.resol[0]) / float(config.resol[1]);
            }
            else if (data == "THREADS") // Set scene num of threads
            {
                string threads;
                file_stream >> threads;
                config.num_threads = stoi(threads);
            }
            else if (data == "CAMERA") // Load a plane
            {
                float x, y, z;
                file_stream >> x >> y >> z;
                Point o(x, y, z);
                Direction l(-config.aspect_ratio, 0, 0);
                Direction u(0, 1, 0);
                Direction f(0, 0, 3);
                camera = Camera(l, u, f, o, config.resol);
            }
            else if (data == "LIGHT") // Load a plane
            {
                string color;
                float x, y, z;
                file_stream >> x >> y >> z >> color;
                Light *auxLight = new Light(Point(x, y, z), stringToColor(color));
                lights.push_back(auxLight);
            }
            else if (data == "PLANE") // Load a plane
            {
                string name, material;
                float x, y, z, d;
                file_stream >> name >> x >> y >> z >> d >> material;
                Plane *auxPlane = new Plane(Direction(x, y, z), d, name, stringToMaterial(material));
                objs.push_back(auxPlane);
            }
            else if (data == "SPHERE") // Load a sphere
            {
                string name, material;
                float x, y, z, d;
                file_stream >> name >> x >> y >> z >> d >> material;
                Sphere *auxSphere = new Sphere(Point(x, y, z), d, name, stringToMaterial(material));
                objs.push_back(auxSphere);
            }
            else if (data == "TRIANGLE") // Load a sphere
            {
                string material;
                float x1, y1, z1, x2, y2, z2, x3, y3, z3;
                file_stream >> x1 >> y1 >> z1 >> x2 >> y2 >> z2 >> x3 >> y3 >> z3 >> material;
                Triangle *auxTriangle = new Triangle(Point(x1, y1, z1), Point(x2, y2, z2), Point(x3, y3, z3), stringToMaterial(material));
                objs.push_back(auxTriangle);
            }
            else if (data == "OBJECT") // Load a sphere
            {
                string name, obj_file, material, axis;
                float x, y, z, scale, degx, degy, degz;
                file_stream >> name >> obj_file >> scale >> x >> y >> z >> degx >> axis >> degy >> axis >> degz >> axis >> material;
                Object *aux_Object = new Object(name, obj_file, stringToMaterial(material));
                if (degx != 0)
                    aux_Object->rotate(degx * PI / 180, 0);
                if (degy != 0)
                    aux_Object->rotate(degy * PI / 180, 1);
                if (degz != 0)
                    aux_Object->rotate(degz * PI / 180, 2);
                aux_Object->scale(scale);
                aux_Object->translate(Direction(x, y, z));
                for (int i = 0; i < aux_Object->getPolygons(); i++)
                {
                    objs.push_back(aux_Object->getTriangles(i));
                }
            }
            else if (data == "K") // K for photon mapping
            {
                string K;
                file_stream >> K;
                config.k = stoi(K);
            }
            else if (data == "R") // r for photon mapping
            {
                string r;
                file_stream >> r;
                config.r = stof(r);
            }
            else if (data == "$DEBUG_PATH") // Set scene num of threads
            {
                config.pathtracing = false;
            }
            else if (data == "$DEBUG_THREAD") // Set scene num of threads
            {
                config.num_threads = 1;
            }
        }

        file_stream >> data;
    }

    file_stream.close();

    return config;
}

/**
 * @brief Create a Photon Render scene
 *
 * @param file
 * @param rays
 * @param max_photons
 */
void renderPhotonMapping(string file, int rays, int max_photons)
{
    vector<Primitive *> objs;
    vector<Light *> lights;
    vector<Light *> lights_aux;

    Camera *camera = new Camera();

    render_config config = loadScene(file, *camera, objs, lights);

    config.num_tiles_x = (config.resol[0] + config.tile_size - 1) / config.tile_size;
    config.num_tiles_y = (config.resol[1] + config.tile_size - 1) / config.tile_size;
    config.shadow_bias = 1e-4; // The bigger shadowBias is, the bigger the difference from reality is
    config.outfile = file + ".ppm";
    config.start = clock();
    config.rays = rays;
    // Photon mapping
    config.max_photons = max_photons;

    if (objs.size() == 0)
    {
        cout << "[!] Scene is empty! Objects are needed." << endl;
        exit(1);
    }

    if (lights.size() == 0)
    {
        lights = lights_aux;
    }

    cout << " - Resolution: " << config.resol[0] << "x" << config.resol[1] << " pixels." << endl;
    cout << " - Threads used: " << config.num_threads << " threads." << endl;
    cout << " - Total Objects: " << objs.size() << " objects." << endl;
    cout << " - Total Point Lights: " << lights.size() << " lights." << endl;
    cout << " - Number of Photons: " << config.max_photons << " photons." << endl;
    cout << " - R: " << config.r << endl;
    cout << " - K: " << config.k << " photons." << endl
         << endl;

    cout << "2 > Filling the map ..." << endl << endl;


    // // Default CORNELL BOX
    // Point o(0, 0, -3.5);
    // Direction l(-config.aspect_ratio, 0, 0);
    // Direction u(0, 1, 0);
    // Direction f(0, 0, 3);
    // Camera camera(l, u, f, o, config.resol);

    // Light light(Point(0, 0.5, 0), white);
    // lights.push_back(&light);

    // Plane left_plane(Direction(1, 0, 0), 1, "red_plane", diff_red);
    // objs.push_back(&left_plane);

    // Plane right_plane(Direction(-1, 0, 0), 1, "green_plane", diff_green);
    // objs.push_back(&right_plane);

    // Plane floor_plane(Direction(0, 1, 0), 1, "floor_plane", diff_light_grey);
    // objs.push_back(&floor_plane);

    // Plane ceiling_plane(Direction(0, -1, 0), 1, "ceiling_plane", diff_light_grey);
    // objs.push_back(&ceiling_plane);

    // Plane back_plane(Direction(0, 0, -1), 1, "back_plane", diff_light_grey);
    // objs.push_back(&back_plane);

    // Sphere left_sphere(Point(-0.5, -0.7, 0.25), 0.3, "blue_sphere", diff_blue);
    // objs.push_back(&left_sphere);

    // Sphere right_sphere(Point(0.5, -0.7, -0.25), 0.3, "red_sphere", diff_red);
    // objs.push_back(&right_sphere);

    PhotonMap map = generation_of_photon_map(lights, objs, config);

    cout << "3 > Starting rendering scene ..." << endl;

    // Multi-Threading rendering
    static atomic<int>
        tiles_left;
    tiles_left = config.num_tiles_x * config.num_tiles_y;

    static atomic<int> max_emission;
    max_emission = 0;

    vector<thread> threads;
    config.content = (Vect3 *)malloc(config.resol[0] * config.resol[1] * sizeof(Vect3));
    for (int i = 0; i < config.num_threads; i++)
        threads.emplace_back(&Camera::renderPhoton_thread, camera, i, objs, lights, ref(config), ref(tiles_left), ref(max_emission), map);

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
        cout << endl
             << "4 > Completed in " << int(double(clock() - config.start) / CLOCKS_PER_SEC) / 60 << "min " << int(double(clock() - config.start / CLOCKS_PER_SEC)) % 60 << "s! File saved as renders/" << config.outfile << "." << endl
             << endl;
    }
    else
    {
        cout << endl
             << "4 > Completed in " << int(double(clock() - config.start) / CLOCKS_PER_SEC) << "s! File saved as renders/" << config.outfile << "." << endl
             << endl;
    }

    free(config.content);

    string output_file = "renders\\" + config.outfile;
    ShellExecuteA(GetDesktopWindow(), NULL, output_file.c_str(), NULL, NULL, SW_SHOW); // Open output file created
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
    vector<Light *> lights_aux;

    Camera *camera = new Camera();

    render_config config = loadScene(file, *camera, objs, lights);

    config.num_tiles_x = (config.resol[0] + config.tile_size - 1) / config.tile_size;
    config.num_tiles_y = (config.resol[1] + config.tile_size - 1) / config.tile_size;
    config.shadow_bias = 1e-4; // The bigger shadowBias is, the bigger the difference from reality is
    config.outfile = file + ".ppm";
    config.start = clock();
    config.rays = rays;

    if (objs.size() == 0)
    {
        cout << "[!] Scene is empty! Objects are needed." << endl;
        exit(1);
    }

    if (lights.size() == 0)
    {
        lights = lights_aux;
    }

    cout << " - Resolution: " << config.resol[0] << "x" << config.resol[1] << " pixels." << endl;
    cout << " - Threads used: " << config.num_threads << " threads." << endl;
    cout << " - Total Objects: " << objs.size() << " objects." << endl;
    cout << " - Total Point Lights: " << lights.size() << " lights." << endl
         << endl;

    cout << "2 > Starting rendering scene ..." << endl;

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
    cout << " - Progress   [||||||||||||||||||||||||||||||||||||||||||||||||||] - 100%        (Saving image ...)\r";
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
    cout << " - Progress   [||||||||||||||||||||||||||||||||||||||||||||||||||] - 100%         (Saving completed!)\r" << endl;
    cout.flush();

    if (double(clock() - config.start) / CLOCKS_PER_SEC > 60)
    {
        cout << endl
             << "3 > Completed in " << int(double(clock() - config.start) / CLOCKS_PER_SEC) / 60 << "min " << int(double(clock() - config.start / CLOCKS_PER_SEC)) % 60 << "s! File saved as renders/" << config.outfile << "." << endl
             << endl;
    }
    else
    {
        cout << endl
             << "3 > Completed in " << int(double(clock() - config.start) / CLOCKS_PER_SEC) << "s! File saved as renders/" << config.outfile << "." << endl
             << endl;
    }

    free(config.content);

    string output_file = "renders\\" + config.outfile;
    ShellExecuteA(GetDesktopWindow(), NULL, output_file.c_str(), NULL, NULL, SW_SHOW); // Open output file created
}

/**
 * @brief render [-r | -p] <filename.ppm> <rays per pixel> [max photons]
 *
 * @param argc
 * @param argv
 * @return int
 */
int main(int argc, char *argv[]) // 1 = -r / -p,  2 = file, 3 = rays, 4 ? = photons
{

    if (argc >= 4)
    {
        if (argv[1][1] == 'r') // Ray-tracing
        {
            renderScene(argv[2], stoi(argv[3]));
        }
        else if (argv[1][1] == 'p' && argc == 5) // Photon-Mapping
        {
            renderPhotonMapping(argv[2], stoi(argv[3]), stoi(argv[4]));
        }
        else
        {
            cout << "[!] Usage: render [-r | -p] <filename> <rays per pixel> [max photons]" << endl;
            exit(1);
        }
    }
    else
    {
        cout << "[!] Usage: render [-r | -p] <filename> <rays per pixel> [max photons]" << endl;
        exit(1);
    }

    return 0;
}