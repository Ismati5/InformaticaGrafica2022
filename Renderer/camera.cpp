/**
 * @file camera.cpp
 * @author Adrián Yago & Ismael Tienda
 * @brief
 * @version 0.1
 * @date 2022-10-01
 *
 * @copyright Copyright (c) 2022
 *
 */
#include "camera.hpp"

/**
 * @brief Generates a progress bar
 *
 * @param row
 * @param height
 * @param start
 */
void progressBar(int tile, int total_tiles, unsigned start)
{
    int bar = tile;
    int percentage = 0;
    unsigned now = clock();
    double estimated = (double(now - start) / CLOCKS_PER_SEC) / tile;

    estimated = (total_tiles - tile) * estimated;

    cout << "   Progress   [";
    percentage = tile * 100 / total_tiles;
    for (int i = 0; i < 50; i++)
    {
        if (bar >= total_tiles / 50)
        {
            cout << "|";
        }
        else
        {
            cout << " ";
        }
        bar -= total_tiles / 50;
    }
    cout << "] - " << percentage << "\%";

    if (estimated < 9999)
    {
        if (estimated > 60)
        {
            cout << "       (Estimated time: " << int(estimated) / 60 << "min " << int(estimated) % 60
                 << "s)         \r";
        }
        else
        {
            cout << "       (Estimated time: " << int(estimated)
                 << "s)         \r";
        }
    }
    else
    {
        cout << "\r";
    }

    cout.flush();
}

/**
 * @brief Creates a random Direction
 *
 * @param pixelSize_x
 * @return Direction
 */
Direction Camera::randomDir(Direction pixelSize)
{

    float ray_x = ((float(rand()) / float(RAND_MAX)) * pixelSize.x) - pixelSize.x / 2;
    float ray_y = ((float(rand()) / float(RAND_MAX)) * pixelSize.y) - pixelSize.y / 2;
    float ray_z = ((float(rand()) / float(RAND_MAX)) * pixelSize.z) - pixelSize.z / 2;
    return Direction(ray_x, ray_y, ray_z);
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
void Camera::colorValue_next_event(vector<Primitive *> objs, Vect3 &emission, Point x, Direction w0, vector<Light *> light_points, Direction n, Vect3 color, float shadowBias)
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

            // Middle term (fr)
            aux = fr(x, wi, w0, color);
            aux_emission *= aux;

            // Right term
            aux2 = abs(n.dotProd((light->center - x).normalize()));
            aux_emission *= aux2;

            emission += aux_emission;
        }
    }
}

void Camera::colorValue_sample(int bounces_left, vector<Primitive *> objs, Vect3 &emission, Point x, Direction w0, vector<Light *> light_points, Direction n, Vect3 color, float shadowBias)
{

    Vect3 ld, lx;
    colorValue_next_event(objs, ld, x, w0, light_points, n, color, shadowBias);

    emission = ld;

    // Calculate random vector
    float theta = (float)(rand()) / (float)(RAND_MAX);
    float phi = (float)(rand()) / (float)(RAND_MAX);

    theta = acos(sqrt(1 - theta));
    phi = 2 * PI * phi;

    // Local coordinate system
    Direction axis_z = n.normalize();

    Direction axis_x;

    if (axis_z.x == 0 && axis_z.y == 0)
    {
        axis_x = Direction(axis_z.y, -axis_z.z, 0); // garantizado por Jorge A.
    }
    else
    {
        axis_x = Direction(axis_z.y, -axis_z.x, 0); // garantizado por Jorge A.
    }

    Direction axis_y = axis_z.crossProd(axis_x);

    // Local to global transform matrix T
    Matrix4 T = TM_changeBase(axis_x, axis_y, axis_z, x);

    // Local direction ωi'
    Direction wi(sin(theta) * cos(phi), sin(theta) * sin(phi), cos(theta));

    Vect4 wi_aux(wi);

    // Change to global coordinates:
    wi_aux = T * wi_aux;

    wi = wi_aux.toDirecton().normalize();

    // cout << wi << endl;

    char a;
    // cin >> a;

    Ray ray;

    ray.p = x;
    ray.d = wi;

    if (bounces_left == 0)
        return;

    float t1, lowest_t1 = numeric_limits<float>::infinity();
    Direction sur_normal;
    Vect3 closest_emisson;
    Point hit;
    Point closest_point;
    Direction closest_normal;
    bool intersected = false;
    for (Primitive *obj : objs)
    {
        if (obj->intersect(ray, t1, sur_normal, hit))
        {
            if (obj->isLight())
                return;
            intersected = true;
            if (t1 < lowest_t1)
            {
                lowest_t1 = t1;
                closest_point = hit;
                closest_normal = sur_normal;
                closest_emisson = obj->emission;
            }
        }
    }

    if (!intersected)
        return;

    colorValue_sample(bounces_left - 1, objs, lx, closest_point, wi, light_points, closest_normal, closest_emisson, shadowBias);

    lx *= fr(x, wi, w0, color);
    emission += (lx * abs(n.dotProd(wi.normalize()))); // ld already added
}

/**
 * @brief Render a tile
 *
 * @param objs
 * @param lights
 * @param config
 * @param num_tile
 * @param max_emission
 */
void Camera::render_thread(int id, vector<Primitive *> objs, vector<Light *> lights, render_config &config, atomic_int &num_tile, atomic_int &max_emission)
{

    int tile, intersections;
    while ((tile = --num_tile) >= 0)
    {
        intersections = 0;
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

        for (float i = y0; i < y1; i++) // i rows
        {
            for (float j = x0; j < x1; j++) // j columns
            {
                for (float r = 0; r < config.rays; r++)
                {
                    Direction variation_x = randomDir(pixelSize_x);
                    Direction variation_y = randomDir(pixelSize_y);
                    Point pixel = topLeft - pixelSize_x * j - pixelSize_y * i - pixelSize_x / 2 - pixelSize_y / 2 + variation_x + variation_y;
                    ray.d = (pixel - origin).normalize();

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

                                if (config.pathtracing)
                                    colorValue_sample(config.bounces, objs, closest_emission, x, w0, lights, sur_normal, i->emission, config.shadow_bias);
                                // colorValue_next_event(objs, closest_emission, x, w0, lights, sur_normal, i->emission, config.shadow_bias); // With path tracing
                                else
                                    closest_emission = i->emission; // Without path tracing
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

                    config.content[(int)(i * config.resol[0] + j)] =
                        Vect3(round(total_emission.x), round(total_emission.y), round(total_emission.z));

                    intersections = 0;
                    total_emission = Vect3(0, 0, 0);
                    lowest_t1 = numeric_limits<float>::infinity();
                }
                else
                    config.content[(int)(i * config.resol[0] + j)] = Vect3(0, 0, 0);
            }
        }
        if (id == 0 && tile > -1)
            progressBar(config.num_tiles_x * config.num_tiles_y - tile, config.num_tiles_x * config.num_tiles_y, config.start);
    }
}