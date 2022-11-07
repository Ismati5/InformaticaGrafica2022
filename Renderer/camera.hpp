/**
 * @file camera.hpp
 * @author Adrián Yago & Ismael Tienda
 * @brief
 * @version 0.1
 * @date 2022-10-29
 *
 * @copyright Copyright (c) 2022
 *
 */
#pragma once

#include <iostream>
#include <atomic>
#include <thread>
#include <fstream>
#include "direction.hpp"
#include "sphere.hpp"
#include "plane.hpp"
#include "triangle.hpp"
#include "point.hpp"
#include "light.hpp"
#include "ray.hpp"
#include <cmath>
#include <vector>
#include <limits>
#include <ctime>

#define PI (3.14159265358979323846264338327950288)

using namespace std;

struct render_config
{
    int *resol;
    float aspect_ratio;
    int rays;
    int num_tiles_x;
    int num_tiles_y;
    int tile_size{32};
    float shadow_bias;
    Vect3 *content; 
    string outfile;
};

/**
 * @brief Generates a progress bar
 *
 * @param row
 * @param height
 * @param start
 */
void progressBar(int row, int height, unsigned start);

/**
 * @brief Camera class
 *
 */
class Camera
{

public:
    Point origin;
    Direction L, U, F; // L = x, U = z, F = y
    int size[2];

    Point topLeft, topRight, botLeft, botRight;
    Direction pixelSize_x, pixelSize_y;

    /**
     * @brief Construct a new Camera object
     *
     * @param _L
     * @param _U
     * @param _F
     * @param O
     * @param _size
     */
    Camera(Direction _L, Direction _U, Direction _F, Point O, int _size[2]) : L(_L), U(_U), F(_F), origin(O)
    {

        size[0] = _size[0];
        size[1] = _size[1];

        topLeft = origin + U + L + F;
        topRight = origin + U - L + F;
        botLeft = origin - U + L + F;
        botRight = origin - U - L + F;

        pixelSize_x = (topLeft - topRight) / size[0];
        pixelSize_y = (topLeft - botLeft) / size[1];
    }

    /**
     * @brief Returns the color
     *
     * @param x
     * @param wi
     * @param w0
     * @param kd
     * @return Vect3
     */
    Vect3 fr(Point x, Direction wi, Direction w0, Vect3 kd)
    {
        return kd / PI;
    }

    /**
     * @brief Sets emission property taking into count shadows
     *
     * @param objs
     * @param emission
     * @param x
     * @param w0
     * @param light_points
     * @param n
     * @param color
     */
    void colorValue(vector<Object *> objs, Vect3 &emission, Point x, Direction w0, vector<Light *> light_points, Direction n, Vect3 color, float shadowBias)
    {

        Vect3 aux_emission;
        Vect3 aux;
        float aux2;
        aux_emission = Vect3(0, 0, 0);

        Direction sur_normal;
        Point aux_x;
        Direction wi;
        Ray shadow;
        bool isShadow = false;
        float t1 = 0;

        for (Light *light : light_points)
        {

            isShadow = false;

            // Check if it's a shadow
            wi = (light->center - (x + n * shadowBias)).normalize();
            shadow.d = wi;
            shadow.p = x + n * shadowBias;

            for (auto i : objs)
            {
                if (i->intersect(shadow, t1, sur_normal, aux_x))
                {
                    if (t1 > 0 && t1 < (light->center - (x + n * shadowBias)).modulus())
                    {
                        isShadow = true;
                        break;
                    }
                }
            }

            // If it's not a shadow
            if (!isShadow)
            {
                // Left term (Li)
                aux_emission = light->power / ((x - light->center).modulus() * (x - light->center).modulus());
                // cout << "aux_EMISION_1: " << aux_emission << endl;

                // Middle term (fr)
                aux = fr(x, wi, w0, color);
                aux_emission *= aux;
                // cout << "aux: " << aux << endl;
                // cout << "aux_EMISION_2: " << aux_emission << endl;

                // Right term

                //  cout << "Normal: " << n << endl;
                //  cout << "Light center: " << light->center - x << endl;

                aux2 = abs(n.dotProd((light->center - x).normalize()));
                aux_emission *= aux2;
                //  cout << "aux2: " << aux2 << endl << endl;
                // cout << "aux_EMISION_3: " << aux_emission << endl;

                emission += aux_emission;
            }
        }

        // cout << "EMISION: " << emission << endl;
    }

    Direction randomDir(Direction pixelSize_x)
    {

        float ray_x = ((float(rand()) / float(RAND_MAX)) * pixelSize_x.x) - pixelSize_x.x / 2;
        float ray_y = ((float(rand()) / float(RAND_MAX)) * pixelSize_x.y) - pixelSize_x.y / 2;
        float ray_z = ((float(rand()) / float(RAND_MAX)) * pixelSize_x.z) - pixelSize_x.z / 2;
        return Direction(ray_x, ray_y, ray_z);
    }

    void render_thread(vector<Object *> objs, vector<Light *> lights,render_config config, atomic<int> num_tile){

        int tile;

        while ((tile = --num_tile) >= 0) { 

            int max_emission = 0, intersections = 0;
            float t1, lowest_t1 = numeric_limits<float>::infinity();
            bool intersected = false;
            Direction sur_normal;
            Vect3 closest_emission = Vect3(0, 0, 0);
            Vect3 total_emission = Vect3(0, 0, 0);
            Point x;

            Ray ray;
            ray.p = origin;

            int tile_y = tile / config.num_tiles_x;
            int tile_x = tile - tile_y * config.num_tiles_x;
            int x0 = tile_x * config.tile_size;
            int x1 = min((tile_x + 1) * config.tile_size, config.resol[0]);
            int y0 = tile_y * config.tile_size;
            int y1 = min((tile_y + 1) * config.tile_size, config.resol[1]);

            unsigned start = clock();
        }

    }

    /**
     * @brief Renders an scene
     *
     * @param outfile
     * @param sphere_objs
     * @param plane_objs
     * @param rays_per_pix
     */
    void render(vector<Object *> objs, vector<Light *> light_points, render_config config)
    {

        vector<vector<Vect3>> content;
        int max_emission = 0, intersections = 0;
        float t1, lowest_t1 = numeric_limits<float>::infinity();
        bool intersected = false;

        Ray ray;
        ray.p = origin;

        Point x;
        Direction sur_normal;
        Vect3 closest_emission = Vect3(0, 0, 0);
        Vect3 total_emission = Vect3(0, 0, 0);

        // int tile_y = tile / config.num_tiles_x;
        // int tile_x = tile - tile_y * config.num_tiles_x;
        // int x0 = tile_x * config.tile_size;
        // int x1 = min((tile_x + 1) * config.tile_size, config.resol[0]);
        // int y0 = tile_y * config.tile_size;
        // int y1 = min((tile_y + 1) * config.tile_size, config.resol[1]);

        unsigned start = clock();

        for (float i = 0; i < size[1]; i++) // i rows
        {
            vector<Vect3> content_aux;
            for (float j = 0; j < size[0]; j++) // j columns
            {
                for (float r = 0; r < config.rays; r++)
                {
                    Direction variation_x = randomDir(pixelSize_x);
                    Direction variation_y = randomDir(pixelSize_y);

                    Point pixel = topLeft - pixelSize_x * j - pixelSize_y * i - pixelSize_x / 2 - pixelSize_y / 2 + variation_x + variation_y;
                    ray.d = (pixel - origin).normalize();
                    // cout << "[" << i << "]" << "[" << j << "] " << pixel << endl;

                    // check intersections
                    for (auto i : objs)
                    {
                        if (i->intersect(ray, t1, sur_normal, x))
                        {
                            if (t1 < lowest_t1)
                            {
                                lowest_t1 = t1;

                                Direction w0 = (origin - x).normalize();
                                closest_emission = Vect3(0, 0, 0);
                                colorValue(objs, closest_emission, x, w0, light_points, sur_normal, i->emission, config.shadow_bias); // With path tracing
                                // closest_emission = i->emission;                                                               // Without path tracing
                                intersected = true;
                            }
                        }
                    }

                    if (intersected)
                    {
                        intersections++;
                        total_emission += closest_emission;
                        intersected = false;
                    }
                }

                if (intersections > 0)
                {

                    total_emission /= intersections;

                    if (max_emission < round(total_emission.x))
                        max_emission = round(total_emission.x);
                    if (max_emission < round(total_emission.y))
                        max_emission = round(total_emission.y);
                    if (max_emission < round(total_emission.z))
                        max_emission = round(total_emission.z);

                    content_aux.push_back(Vect3(round(total_emission.x), round(total_emission.y), round(total_emission.z)));

                    intersections = 0;
                    total_emission = Vect3(0, 0, 0);
                    lowest_t1 = numeric_limits<float>::infinity();
                }
                else
                    content_aux.push_back(Vect3(0, 0, 0));
            }

            content.push_back(content_aux);

            progressBar(i, size[1], start);
        }

        // Saving file
        cout << "> Progress   [|||||||||||||||||||||||||||||||||||||||||] - 100%        (Saving image ...)\r";
        cout.flush();

        ofstream file("renders/" + config.outfile);
        file << "P3" << endl;
        file << "#MAX=255" << endl;
        file << size[0] << " " << size[1] << endl;
        file << max_emission << endl;

        for (float i = 0; i < size[1]; i++) // i rows
        {
            for (float j = 0; j < size[0]; j++) // j columns
            {
                file << content.at(i).at(j).x << " " << content.at(i).at(j).y << " " << content.at(i).at(j).z << "    ";
            }
            file << endl;
        }

        file.close();
        cout << "> Progress   [|||||||||||||||||||||||||||||||||||||||||] - 100%        (Saving completed!)\r" << endl;
        cout.flush();
        cout << "> Completed! File saved as renders/"
             << config.outfile << "." << endl
             << endl;
    }
};