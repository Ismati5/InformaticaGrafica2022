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

        // cout << "topLeft: " << topLeft << endl;
        // cout << "topRight: " << topRight << endl;
        // cout << "botLeft: " << botLeft << endl;
        // cout << "botRight: " << botRight << endl;

        pixelSize_x = (topLeft - topRight) / size[0];
        pixelSize_y = (topLeft - botLeft) / size[1];

        // cout << "pixelSize_x: " << pixelSize_x << endl;
        // cout << "pixelSize_y: " << pixelSize_y << endl;
    }

    void foo() {}

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

            // Check if it's a shadow
            wi = (light->center - x).normalize();
            shadow.d = wi;
            shadow.p = x;

            for (auto i : objs)
            {
                shadow.p = x + i->normal * shadowBias;
                if (i->intersect(shadow, t1, sur_normal, aux_x))
                {
                    if (t1 > 0.00001 && t1 < (light->center - x).modulus())
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

    /**
     * @brief Renders an scene
     *
     * @param outfile
     * @param sphere_objs
     * @param plane_objs
     * @param rays_per_pix
     */
    void render(string outfile, vector<Object *> objs, int rays_per_pix, vector<Light *> light_points, float shadowBias)
    {

        // file header

        vector<vector<Vect3>> content;
        int max_emission = 0;

        Ray ray;
        ray.p = origin;

        Vect3 closest_emission = Vect3(0, 0, 0);
        Vect3 total_emission = Vect3(0, 0, 0);
        int intersections = 0;
        bool intersected = false;
        float t1, t2, lowest_t1 = numeric_limits<float>::infinity();
        Direction sur_normal;
        Point x;

        unsigned start = clock();

        for (float i = 0; i < size[1]; i++) // i rows
        {
            vector<Vect3> content_aux;

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
                    Direction variation_y(ray2_x, ray2_y, ray2_z);

                    Point pixel = topLeft - pixelSize_x * j - pixelSize_y * i - pixelSize_x / 2 - pixelSize_y / 2 + variation_x + variation_y;

                    // cout << "[" << i << "]" << "[" << j << "] " << pixel << endl;
                    ray.d = (pixel - origin).normalize();

                    // check intersections
                    for (auto i : objs)
                    {

                        if (i->intersect(ray, t1, sur_normal, x))
                        {

                            if (t1 > 0.00001 && t1 < lowest_t1)
                            {
                                lowest_t1 = t1;

                                Direction w0 = (origin - x).normalize();
                                closest_emission = Vect3(0, 0, 0);
                                colorValue(objs, closest_emission, x, w0, light_points, sur_normal, i->emission, shadowBias); // With path tracing
                                // closest_emission = i->emission; // Without path tracing
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
                {
                    content_aux.push_back(Vect3(0, 0, 0));
                }
            }

            content.push_back(content_aux);

            progressBar(i, size[1], start);
        }

        ofstream file("renders/" + outfile);
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

        cout << "   Progress   [||||||||||||||||||||||||||||||||||||||||]\t100%        (Image rendering completed!)" << endl
             << "File saved as renders/"
             << outfile << "." << endl
             << endl;
        file.close();
    }
};