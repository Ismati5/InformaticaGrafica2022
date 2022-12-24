
#pragma once

#include <iostream>
#include <thread>
#include "matrix4.hpp"
#include "kdtree.hpp"

#define PI (3.14159265358979323846264338327950288)

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
    bool pathtracing = true;
    unsigned start;

    float max_distance = 9999;
    int num_threads = thread::hardware_concurrency();

    // Photon mapping
    int max_photons;
    int k;
    float r;
};

class Photon
{
public:
    Vect3 position_; // 3D point of the interaction
    Direction wp;
    float flux;
    Material material;

    float position(size_t i) const { return position_[i]; } // It returns the axis i position (x, y or z)
};

/*
    An additional struct that allows the KD-Tree to access your photon position
*/
struct PhotonAxisPosition
{
    float operator()(const Photon &p, size_t i) const
    {
        return p.position(i);
    }
};

/*
    The KD-Tree ready to work in 3 dimensions, with Photon s, under a
    brand-new name: PhotonMap
*/
using PhotonMap = nn::KDTree<Photon, 3, PhotonAxisPosition>;
