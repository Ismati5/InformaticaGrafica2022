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
public:
    Vect3 emission;
    Vect3 position_;    // 3D point of the interaction
    Direction direction;
    float flux;

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

Vect3 fr(Point x, Direction wi, Direction w0, Material material, materialType &type)
{

    return material.kd / 255.0;

}

intersectionType closestObj(vector<Primitive *> objs, Ray ray, Direction &closest_normal, Point &closest_point,
                                    Vect3 &closest_emission, Direction w0, string &name, Material material, Material &intersectedMaterial)
{
    float t1, lowest_t1 = numeric_limits<float>::infinity();
    Vect3 emission;
    Direction normal;
    Point point;
    bool intersected = false;

    materialType material_type;

    intersectionType closest_type;

    for (Primitive *obj : objs)
    {
        if (obj->intersect(ray, t1, normal, point))
        {
            intersected = true;

            if (t1 < lowest_t1)
            {
                lowest_t1 = t1;
                closest_point = point;
                closest_normal = normal;
                closest_emission = obj->color();
                name = obj->name;

                intersectedMaterial = obj->material;

                if (obj->isLight())
                {
                    closest_type = LIGHT;
                    closest_emission = obj->power() * fr(closest_point, ray.d, w0, material, material_type);
                }

                else
                    closest_type = OBJECT;
            }
        }
    }

    if (intersected)
        return closest_type;

    return NONE;
}

void direct_light(vector<Primitive *> objs, Vect3 &emission,
                          Point x, Direction w0, vector<Light *> light_points,
                          Direction n, float shadowBias, Material material)
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

    materialType material_type;

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

            aux_emission *= (fr(x, wi, w0, material, material_type) / PI);

            // Right term
            aux_emission *= abs(n.dotProd((light->center - x).normalize()));

            emission += aux_emission;
        }
    }
}

void light_value(vector<Primitive *> objs, Vect3 &emission, Point x, Direction w0,
                         vector<Light *> light_points, Direction n, float shadowBias, Material material, list<Photon> &photons, int max_photons)
{

    if (photons.size() >= max_photons) return;

    // Calculate random vector
    float theta = (float)(rand()) / (float)(RAND_MAX);
    float phi = (float)(rand()) / (float)(RAND_MAX);

    theta = acos(sqrt(1 - theta));
    phi = 2 * PI * phi;

    // Local coordinate system
    Direction axis_z = n.normalize();
    Direction axis_x = w0.crossProd(n).normalize();
    Direction axis_y = axis_z.crossProd(axis_x).normalize();

    // Local to global transform matrix T
    Matrix4 T = TM_changeBase(axis_x, axis_y, axis_z, x);

    // Local direction ωi'
    Direction wi(sin(theta) * cos(phi), sin(theta) * sin(phi), cos(theta));

    // Change to global coordinates
    Vect4 wi_aux(wi);
    wi = (T * wi_aux).toDirecton().normalize();

    Point closest_point;
    Vect3 closest_emission;
    Direction closest_normal;
    string closest_name;

    Vect3 ld(0, 0, 0), lx(0, 0, 0);

    materialType material_type;
    Vect3 brdf = fr(x, wi, w0, material, material_type);

    Ray ray(wi, x);


    // Light from point sources
    direct_light(objs, ld, x, w0, light_points, n, shadowBias, material);

    Material material_aux;
    intersectionType intersected = closestObj(objs, ray, closest_normal, closest_point, closest_emission, w0, closest_name, material, material_aux);

    Photon ph;
    ph.position_ = closest_point.toVect3();
    ph.direction = wi;

    if (intersected == NONE) // No intersection with scene
    {
        ph.emission = Vect3(0, 0, 0);
        photons.push_front(ph);
        return;
    }
    else if (intersected == LIGHT) // Intersection with area light
    {
        ph.emission = closest_emission;
        photons.push_front(ph);
        return;
    }

    light_value(objs, lx, closest_point, wi, light_points, closest_normal, shadowBias, material_aux, photons, max_photons);

    //Save photon
    ph.emission = ld + lx * brdf;
    photons.push_front(ph);
}

Direction randomWalk()
{
    float theta = (float)(rand()) / (float)(RAND_MAX);                
    float phi = (float)(rand()) / (float)(RAND_MAX);             
 
    theta = acos(2 * theta - 1);
    phi = 2 * PI * phi;

    return Direction(sin(theta) * cos(phi), sin(theta) * sin(phi), cos(theta));
}

bool hitPosition(vector<Primitive *> objects, Ray ray, Direction &n, Point &x, Material &m) 
{
    float t1, lowest_t1 = numeric_limits<float>::infinity();
    Direction hitNormal;
    bool intersected = false;
    Point hitPoint;

    for (Primitive *obj : objects)
    {
        if (obj->intersect(ray, t1, hitNormal, hitPoint))
        {
            intersected = true;
            if (lowest_t1 < t1)
            {
                x = hitPoint;
                n = hitNormal;
                m = obj->material;
                lowest_t1 = 1;
            }
        }
    }

    return intersected;

}

/*
    Example function to generate the photon map with the given photons
*/
PhotonMap generation_of_photon_map(vector<Light *> lights, vector<Primitive *> objects, int max_photons, float shadowBias){

    const size_t fixedListSize(max_photons);
    list<Photon> photons(fixedListSize);        // Create a list of photons 

    int totalPower;
    for (Light *light : lights)
        totalPower += light->power.x + light->power.y + light->power.z;

    Material material;
    Vect3 emission = Vect3(0,0,0);
    Direction n;
    Photon ph;
    Point x;
    for (Light *light : lights)
    {
        int initial_size = photons.size();
        int shots_taken = 0;
        int max_shots = max_photons * ((light->power.x + light->power.y + light->power.z) / totalPower);
        while (shots_taken < max_shots)
        {
            Direction wi = randomWalk();
            // Cambiar a coordenadas globales
            //TODO

            // Calcular foton
            Ray ray(wi, light->center);

            if (hitPosition(objects, ray, n, x, material))
            {
                ph.position_ = x.toVect3();
                ph.direction = wi;

                light_value(objects, emission, x, (x - light->center).normalize(), lights, n, shadowBias, material, photons, max_photons);
                ph.emission = emission;
                emission = Vect3(0,0,0);
                
                if (photons.size() >= max_photons) goto out;
                photons.push_front(ph); // First bounce
            }
            shots_taken++;
        }

        // Modify flux of the last shots_taken photons
        int added_photons = photons.size() - initial_size; 
        auto it = photons.begin();
        for (int i = 0; i < added_photons; i++)
        {
            it->flux = (4*PI*(light->power.x + light->power.y + light->power.z)) / shots_taken;
            it++;
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



