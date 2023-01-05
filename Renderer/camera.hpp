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
#include <cmath>
#include <vector>
#include <limits>
#include <ctime>

#include "sphere.hpp"
#include "plane.hpp"
#include "triangle.hpp"
#include "square.hpp"
#include "light.hpp"
#include "variables.hpp"

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

        pixelSize_x = (topLeft - topRight) / size[0];
        pixelSize_y = (topLeft - botLeft) / size[1];
    }

    Camera() {}

    /**
     * @brief Intersects ray with scene
     *
     * @param objs
     * @param ray
     * @param closest_normal
     * @param closest_point
     * @param closest_emission
     * @param w0
     * @param color
     * @param name
     * @return intersectionType
     */
    intersectionType closestObj(vector<Primitive *> objs, Ray ray, Direction &closest_normal,
                                Point &closest_point, Vect3 &closest_emission, Direction w0, string &name, Material material, Material &intersectedMaterial);

    /**
     * @brief Returns the color
     *
     * @param x
     * @param wi
     * @param w0
     * @param texture
     * @param absorbed
     * @return Vect3
     */
    Vect3 fr(Point x, Direction wi, Direction w0, Material material, materialType &type, bool noAbsortion = false)
    {

        if (material.isLight())
        {
            return (material.ke / 255.0);
        }

        float p = 0;
        Vect3 kd;

        type = material.getMatType(p, noAbsortion);

        switch (type)
        {
        case DIFFUSE:
            type = DIFFUSE;

            if (material.hasTexture())
            {

                float dist_x, dist_y, width, height;
                int texel_x, texel_y;

                if (material.normal.x != 0) // Facing X axis
                {
                    dist_x = abs((material.ref_point - x).z);
                    dist_y = abs((material.ref_point - x).y);

                    width = abs(material.e1.z);
                    height = abs(material.e2.y);

                    dist_x /= width;
                    dist_y /= height;

                    dist_x *= material.texture_res[0];
                    dist_y *= material.texture_res[1];

                    texel_x = dist_x;
                    texel_y = dist_y;
                }
                else if (material.normal.y != 0) // Facing Y axis
                {
                    dist_x = abs((material.ref_point - x).x);
                    dist_y = abs((material.ref_point - x).z);

                    width = abs(material.e1.x);
                    height = abs(material.e2.z);

                    dist_x /= width;
                    dist_y /= height;

                    dist_x *= material.texture_res[0];
                    dist_y *= material.texture_res[1];

                    texel_x = dist_x;
                    texel_y = dist_y;
                }
                else if (material.normal.z != 0) // Facing Z axis
                {
                    dist_x = abs((material.ref_point - x).x);
                    dist_y = abs((material.ref_point - x).y);

                    width = abs(material.e1.x);
                    height = abs(material.e2.y);

                    dist_x /= width;
                    dist_y /= height;

                    dist_x *= material.texture_res[0];
                    dist_y *= material.texture_res[1];

                    texel_x = dist_x;
                    texel_y = dist_y;
                }

                kd = material.texture[texel_x + texel_y * material.texture_res[0]];
            }
            else
            {
                kd = material.kd;
            }

            return (kd / 255.0) / p;
        case SPECULAR:
            type = SPECULAR;
            return (material.ks / 255.0) / p;
            break;
        case REFRACTION:
            type = REFRACTION;
            return (material.kt / 255.0) / p;
            break;
        case ABSORTION:
            type = ABSORTION;
            return Vect3(0, 0, 0);
        }
        return Vect3(0, 0, 0);
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
    void direct_light(vector<Primitive *> objs, Vect3 &emission,
                      Point x, Direction w0, vector<Light *> light_points,
                      Direction n, Vect3 color, float shadowBias, Material material, Vect3 brdf);

    /**
     * @brief Light value emission
     *
     * @param bounces_left
     * @param objs
     * @param emission
     * @param x
     * @param w0
     * @param light_points
     * @param n
     * @param color
     * @param shadowBias
     */
    void light_value(vector<Primitive *> objs, Vect3 &emission,
                     Point x, Direction w0, vector<Light *> light_points, Direction n,
                     Vect3 color, float shadowBias, string name, Material material, float ref_coef);

    /**
     * @brief Creates a random Direction
     *
     * @param pixelSize_x
     * @return Direction
     */
    Direction randomDir(Direction pixelSize);

    /**
     * @brief Render a tile
     *
     * @param objs
     * @param lights
     * @param config
     * @param num_tile
     * @param max_emission
     */
    void render_thread(int id, vector<Primitive *> objs, vector<Light *> lights, render_config &config, atomic_int &num_tile, atomic_int &max_emission);

    vector<Photon> search_nearest(PhotonMap &map, Vect3 x, unsigned long K, float r);

    bool hitPosition(vector<Primitive *> objects, Ray ray, Direction &n, Point &x, Material &m);

    Vect3 emission_ph(vector<Primitive *> objs, vector<Light *> lights, render_config config, PhotonMap &map, Point x, Direction w0, Direction n, Material material);

    void renderPhoton_thread(int id, vector<Primitive *> objs, vector<Light *> lights, render_config &config, atomic_int &num_tile, atomic_int &max_emission, PhotonMap map);
};