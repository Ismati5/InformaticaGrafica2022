/*
Description :   This is an example of usage of the KDTree class. It does not
                compile, but should give you the idea of how to integrate the
                KDTree in your code
*/

using namespace std;

#include "kdtree.hpp"

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
    if (material.isLight())
    {
        return (material.ke / 255.0);
    }

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
    return Vect3(0,0,0);

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
    if (max_photons == 0) return;

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

    // Only store non diffuse intersections
    if (material_type != DIFFUSE) 
        return ;

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

    light_value(objs, lx, closest_point, wi, light_points, closest_normal, shadowBias, material_aux, photons, max_photons-1);

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

// Returns true if ray "ray" hits something. n,x and m will contain the data of the closest hit
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
            if (t1 < lowest_t1)
            {
                x = hitPoint;
                n = hitNormal;
                m = obj->material;
                lowest_t1 = t1;
            }
        }
    }

    return intersected;

}

void printList(list<Photon> list)
{
    for (Photon ph : list)
    {
        cout << "============================================" <<endl;
        cout << "EMISSION:" <<  ph.emission << endl;
        cout << "POSITION:" <<  ph.position_ << endl;
        cout << "DIRECTION:" <<  ph.direction << endl;
        cout << "FLUX:" <<  ph.flux << endl;
        cout << "============================================" <<endl;

    }
    cout << "SIZE:" <<  list.size() << endl << endl;
}

/*
    Example function to generate the photon map with the given photons
*/
PhotonMap generation_of_photon_map(vector<Light *> lights, vector<Primitive *> objects, render_config config){

    Point x;
    Direction n;
    Material material;
    Vect3 emission = Vect3(0,0,0);

    const size_t fixedListSize(config.max_photons);
    list<Photon> all_photons(fixedListSize);
    all_photons.clear();
    list<Photon> photons(fixedListSize);

    int totalPower = 0;
    for (Light *light : lights) totalPower += light->powerValue();

    int added_photons = 0;
    for (Light *light : lights)
    {
        photons.clear();
        int max_shots = config.max_photons * (light->powerValue() / totalPower);
        for (int i = 0; i < max_shots; i++)
        {
            Direction wi = randomWalk();

            Ray ray(wi, light->center);
            if (hitPosition(objects, ray, n, x, material))
            {
                
                light_value(objects, emission, x, (x - light->center).normalize(), lights, n, 
                            config.shadow_bias, material, photons, config.max_photons - all_photons.size() - photons.size());

                if (config.max_photons <= all_photons.size() + photons.size()) break;
            }
        }

        // Update flux
        for (auto it = photons.begin(); it != photons.end(); ++it)
            it->flux = (4 * PI * light->powerValue()) / max_shots;

        all_photons.insert(all_photons.end(), photons.begin(), photons.end());
        if (config.max_photons <= all_photons.size()) break;
        
    }

    printList(all_photons);

    PhotonMap map = PhotonMap(all_photons, PhotonAxisPosition());
    return map;

}

/*
    Example method to search for the nearest neighbors of the photon map
*/
void search_nearest(PhotonMap map, Vect3 x, unsigned long K, float r){
    // Position to look for the nearest photons
    Vect3 query_position = x;    

    // Maximum number of photons to look for
    unsigned long nphotons_estimate = K;

    // Maximum distance to look for photons
    float radius_estimate = r;

    // nearest is the nearest photons returned by the KDTree
    auto nearest = map.nearest_neighbors(query_position,
                                         nphotons_estimate,
                                         radius_estimate);
}



