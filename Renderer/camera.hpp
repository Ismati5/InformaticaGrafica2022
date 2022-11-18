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
    int bounces;

    float max_distance = 99;
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

        return (kd / 255.0) / PI;
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
                      Direction n, Vect3 color, float shadowBias);

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
    void light_value(int bounces_left, vector<Primitive *> objs, Vect3 &emission, Vect3 brdfAnt,
                     Point x, Direction w0, vector<Light *> light_points, Direction n,
                     Vect3 color, float shadowBias);

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