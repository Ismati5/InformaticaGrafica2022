/*
Description :   This is an example of usage of the KDTree class. It does not
                compile, but should give you the idea of how to integrate the
                KDTree in your code
*/

using namespace std;

#include "kdtree.h"

/* 
    Your Photon class implementation, which stores each 
    photon walk interaction 
*/
class Photon {
    Vect3 position_;    // 3D point of the interaction

public:
    float position(size_t i) const { return position_[i];}    // It returns the axis i position (x, y or z)
};

/* 
    An additional struct that allows the KD-Tree to access your photon position
*/
struct PhotonAxisPosition {
    float operator()(const Photon& p, size_t i) const {
        return p.position(i);
    }
};

/* 
    The KD-Tree ready to work in 3 dimensions, with Photon s, under a 
    brand-new name: PhotonMap 
*/
using PhotonMap = nn::KDTree<Photon,3,PhotonAxisPosition>;

Vect3 randomWalk()
{
    float theta = (float)(rand()) / (float)(RAND_MAX);                
    float phi = (float)(rand()) / (float)(RAND_MAX);             
 
    theta = acos(2 * theta - 1);
    phi = 2 * PI * phi;

    return Vect3(sin(theta) * cos(phi), sin(theta) * sin(phi), cos(theta));
}

/*
    Example function to generate the photon map with the given photons
*/
PhotonMap generation_of_photon_map(vector<Light *> lights, int numPhotons, int maxPhotons){

    const size_t fixedListSize(maxPhotons);
    list<Photon> photons(fixedListSize);        // Create a list of photons 

    int totalPower;
    for (Light *light : lights)
        totalPower += light->power.x + light->power.y + light->power.z;

    Vect3 wi;
    Photon ph;
    int numPhotons_perLight;
    for (Light *light : lights)
    {
        numPhotons_perLight = numPhotons * ((light->power.x + light->power.y + light->power.z) / totalPower);
        for (int i = 0; i < numPhotons_perLight; i++)
        {

            if (photons.size() >= photons.max_size()) goto out;

            wi = randomWalk();
            // Cambiar a coordenadas globales

            // Calcular foton

            photons.push_front(ph);
        }
    }
    out:

    PhotonMap map = PhotonMap(photons, PhotonAxisPosition());
    return map;
}

/*
    Example method to search for the nearest neighbors of the photon map
*/
// void search_nearest(PhotonMap map, ...){
//     // Position to look for the nearest photons
//     Vect3 query_position = ...;    

//     // Maximum number of photons to look for
//     unsigned long nphotons_estimate = ...;

//     // Maximum distance to look for photons
//     float radius_estimate = ...;

//     // nearest is the nearest photons returned by the KDTree
//     auto nearest = map.nearest_neighbors(query_position,
//                                          nphotons_estimate,
//                                          radius_estimate)
// }



