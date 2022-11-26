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

#include "direction.hpp"
#include "sphere.hpp"
#include "plane.hpp"
#include "triangle.hpp"
#include "point.hpp"
#include "light.hpp"
#include "ray.hpp"
#include "matrix4.hpp"
#include "vect4.hpp"

#define PI (3.14159265358979323846264338327950288)

using namespace std;

enum intersectionType
{
    NONE,
    LIGHT,
    OBJECT,
};

/**
 * @brief Render configuration
 *
 */
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
    bool pathtracing;
    unsigned start;

    float max_distance = 9999;
    int num_threads = thread::hardware_concurrency();
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
                                Point &closest_point, Vect3 &closest_emission, Direction w0, Vect3 color, string &name, Material material, Material &intersectedMaterial);

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
    Vect3 fr(Point x, Direction wi, Direction w0, Material material, materialType &type)
    {

        // return material.kd / 255.0; // pa que siga funcionando de momento

        float p = 0;

        type = material.getMatType(p);

        p = 1; // PROVISIONAL

        switch (type)
        {
        case DIFFUSE:
            type = DIFFUSE;
            return (material.kd / 255.0) / p;
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
                      Direction n, Vect3 color, float shadowBias, Material material);

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
                     Vect3 color, float shadowBias, string name, Material material);

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
};