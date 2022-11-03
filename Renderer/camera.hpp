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
#include <fstream>
#include "direction.hpp"
#include "sphere.hpp"
#include "plane.hpp"
#include "point.hpp"
#include "light.hpp"
#include "ray.hpp"
#include <cmath>
#include <vector>
#include <limits>
#include <ctime>

#define PI (3.14159265358979323846264338327950288)

using namespace std;

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

        //cout << "topLeft: " << topLeft << endl;
        //cout << "topRight: " << topRight << endl;
        //cout << "botLeft: " << botLeft << endl;
        //cout << "botRight: " << botRight << endl;

        pixelSize_x = (topLeft - topRight) / size[0];
        pixelSize_y = (topLeft - botLeft) / size[1];

        //cout << "pixelSize_x: " << pixelSize_x << endl;
        //cout << "pixelSize_y: " << pixelSize_y << endl;
    }

    float fr(Point x, Direction wi, Direction w0, float kd){

        return kd/PI;

    }

    void colorValue(float emission[3], Point x, Direction w0, vector<Light*> light_points, Direction n) {

        float aux_emission[3];
        float aux;
        aux_emission[0] = 0;
        aux_emission[1] = 0;
        aux_emission[2] = 0;

        for (Light* light : light_points) {

            //Left term (Li)
            aux_emission[0] = light->power[0] / ((x-light->center).modulus() * (x-light->center).modulus());
            aux_emission[1] = light->power[1] / ((x-light->center).modulus() * (x-light->center).modulus());
            aux_emission[2] = light->power[2] / ((x-light->center).modulus() * (x-light->center).modulus());
            //cout << "aux_EMISION_1: [" << aux_emission[0] << ", " << aux_emission[1] << ", " << aux_emission[2] << "]" <<  endl;

            //Middle term (fr)
            Direction wi = (light->center - x).normalize();
            aux = fr(x, wi, w0, 0.3);
            aux_emission[0] *= aux;
            aux_emission[1] *= aux;
            aux_emission[2] *= aux;
            //cout << aux << endl;

            //cout << "aux_EMISION_2: [" << aux_emission[0] << ", " << aux_emission[1] << ", " << aux_emission[2] << "]" <<  endl;

            //Right term
            aux = abs(n.dotProd((light->center - x).normalize()));
            aux_emission[0] *= aux;
            aux_emission[1] *= aux;
            aux_emission[2] *= aux;
            //cout << aux << endl; 

            //cout << "aux_EMISION_3: [" << aux_emission[0] << ", " << aux_emission[1] << ", " << aux_emission[2] << "]" <<  endl;

            emission[0] += aux_emission[0];
            emission[1] += aux_emission[1];
            emission[2] += aux_emission[2];
            
        }

        //cout << "EMISION: [" << emission[0] << ", " << emission[1] << ", " << emission[2] << "]" <<  endl;

        //char a;
        //cin >> a;

    }

    /**
     * @brief Renders an scene
     *
     * @param outfile
     * @param sphere_objs
     * @param plane_objs
     * @param rays_per_pix
     */
    void render(string outfile, vector<Object *> objs, int rays_per_pix, vector<Light*> light_points)
    {

        // file header
        ofstream file("renders/" + outfile);
        file << "P3" << endl;
        // file << "# " << outfile << endl;
        file << size[0] << " " << size[1] << endl;
        file << "255" << endl;

        Ray ray;
        ray.p = origin;

        float closest_emission[3] = {0, 0, 0};
        float total_emission[3] = {0, 0, 0};
        int intersections = 0;
        bool intersected = false;
        float t1, t2, lowest_t1 = numeric_limits<float>::infinity();
        Direction sur_normal;
        Point x;

        unsigned start = clock();

        for (float i = 0; i < size[1]; i++) // i rows
        {
            for (float j = 0; j < size[0]; j++) // j columns
            {
                for (float r = 0; r < rays_per_pix; r++)
                {

                    float ray_x = ((float(rand()) / float(RAND_MAX)) * pixelSize_x.x) - pixelSize_x.x / 2;
                    float ray_y = ((float(rand()) / float(RAND_MAX)) * pixelSize_x.y) - pixelSize_x.y / 2;
                    float ray_z = ((float(rand()) / float(RAND_MAX)) * pixelSize_x.z) - pixelSize_x.z / 2;
                    Direction variation_x(ray_x, ray_y, ray_z);

                    float ray2_x = ((float(rand()) / float(RAND_MAX)) * pixelSize_y.x) - pixelSize_y.x / 2;
                    float ray2_y = ((float(rand()) / float(RAND_MAX)) * pixelSize_y.y) - pixelSize_y.y / 2;
                    float ray2_z = ((float(rand()) / float(RAND_MAX)) * pixelSize_y.z) - pixelSize_y.z / 2;
                    Direction variation_y(ray_x, ray_y, ray_z);

                    Point pixel = topLeft - pixelSize_x * j - pixelSize_y * i - pixelSize_x / 2 - pixelSize_y / 2 + variation_x + variation_y;

                    // cout << "[" << i << "]" << "[" << j << "] " << pixel << endl;
                    ray.d = (pixel - origin).normalize();

                    // check sphere intersections
                    for (auto i : objs)
                    {
                        if (i->intersect(ray, t1, sur_normal, x))
                        {
                            if (t1 > 0 && t1 < lowest_t1)
                            {
                                lowest_t1 = t1;

                                Direction w0 = (origin - x).normalize();
                                colorValue(closest_emission, x, w0, light_points, i->normal);
                                // closest_emission[0] += i->emission[0];
                                // closest_emission[1] += i->emission[1];
                                // closest_emission[2] += i->emission[2];
                                intersected = true;
                            }
                        }
                    }

                    if (intersected)
                    {
                        intersections++;
                        total_emission[0] += closest_emission[0];
                        total_emission[1] += closest_emission[1];
                        total_emission[2] += closest_emission[2];
                        intersected = false;
                    }
                }

                if (intersections > 0)
                {
                    total_emission[0] /= intersections;
                    total_emission[1] /= intersections;
                    total_emission[2] /= intersections;

                    file << total_emission[0] << " " << total_emission[1] << " " << total_emission[2] << "    ";

                    intersections = 0;

                    total_emission[0] = 0;
                    total_emission[1] = 0;
                    total_emission[2] = 0;
                    lowest_t1 = numeric_limits<float>::infinity();
                }
                else
                {
                    file << 0 << " " << 0 << " " << 0 << "    ";
                }
            }
            progressBar(i, size[0], start);
            file << endl;
        }

        cout << "   Progress   [||||||||||||||||||||||||||||||||||||||||]\t100%        (Image rendering completed!)" << endl
             << "File saved as renders/"
             << outfile << "." << endl
             << endl;
        file.close();
    }
};