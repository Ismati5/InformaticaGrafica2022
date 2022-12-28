using namespace std;

#include "variables.hpp"
#include "material.hpp"
#include "light.hpp"
#include "ray.hpp"
#include "primitive.hpp"

Vect3 fr_ph(Point x, Direction wi, Direction w0, Material material, materialType &type)
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

intersectionType closestObj_ph(vector<Primitive *> objs, Ray ray, Direction &closest_normal, Point &closest_point,
                                    Direction w0, Material material, Material &intersectedMaterial)
{
    float t1, lowest_t1 = numeric_limits<float>::infinity();
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

                intersectedMaterial = obj->material;

                if (obj->isLight())
                {
                    closest_type = LIGHT;
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

            aux_emission *= (fr_ph(x, wi, w0, material, material_type) / PI);

            // Right term
            aux_emission *= abs(n.dotProd((light->center - x).normalize()));

            emission += aux_emission;
        }
    }
}

void light_value_ph(vector<Primitive *> objs, Vect3 &emission, Point x, Direction w0,
                         vector<Light *> light_points, Direction n, float shadowBias, 
                         Material material, list<Photon> &photons, int max_photons, Photon prevPh)
{
    if (max_photons == photons.size()) return;

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
    Direction closest_normal;
    string closest_name;

    Vect3 ld(0, 0, 0), lx(0, 0, 0);

    materialType material_type;
    Vect3 brdf = fr_ph(x, wi, w0, material, material_type);

    if (material_type == ABSORTION)
        return;

    // Only store non diffuse intersections
    bool savePhoton = true;
    if (material_type == SPECULAR || material_type == REFRACTION) 
        savePhoton = false;

    Ray ray(wi, x);

    Material material_aux;
    intersectionType intersected = closestObj_ph(objs, ray, closest_normal, closest_point, w0, material, material_aux);

    Photon ph;
    ph.position_ = closest_point.toVect3();
    ph.wp = wi;
    ph.flux = prevPh.flux * brdf;

    if (ph.flux.x >= 255 && ph.flux.y >= 255 && ph.flux.z >= 255) return; // Black

    if (intersected == NONE) // No intersection with scene
    {
        ph.material = material;
        if(savePhoton) photons.push_front(ph);
        return;
    }
    else if (intersected == LIGHT) // Intersection with area light
    {
        ph.material = material;
        if(savePhoton) photons.push_front(ph);
        return;
    }

    //Save photon
    ph.material = material;
    if(savePhoton) photons.push_front(ph);
    if (max_photons == photons.size()) return;
    
    light_value_ph(objs, lx, closest_point, wi, light_points, closest_normal, shadowBias, material_aux, photons, max_photons, ph);

}

Direction randomWalk()
{
    float theta = (float)(rand()) / (float)(RAND_MAX);                
    float phi = (float)(rand()) / (float)(RAND_MAX);             
 
    theta = acos(2 * theta - 1);
    phi = 2 * PI * phi;

    return Direction(sin(theta) * cos(phi), sin(theta) * sin(phi), cos(theta));
}

// Returns true if ray "ray" hits something. n, x and m will contain the data of the closest hit
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
        cout << "POSITION:" <<  ph.position_ << endl;
        cout << "WP:" <<  ph.wp << endl;
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

    float totalPower = 0;
    for (Light *light : lights) totalPower += light->powerValue();

    int added_photons = 0;
    for (Light *light : lights)
    {
        photons.clear();
        int shots_taken = 0;
        int max_shots = config.max_photons * (light->powerValue() / totalPower);
        for (int i = 0; i < max_shots; i++)
        {
            Direction wi = randomWalk();
            shots_taken++;
            Ray ray(wi, light->center);
            if (hitPosition(objects, ray, n, x, material))
            {   
                Photon ph;
                ph.wp = wi;
                ph.position_ = x.toVect3();
                ph.material = material;
                ph.flux = (light->power * 4 * PI) / max_shots;

                // photons.push_back(ph);

                light_value_ph(objects, emission, x, (x - light->center).normalize(), lights, n, 
                            config.shadow_bias, material, photons, max_shots, ph);
                
                if (photons.size() >= config.max_photons) break;
            }
        }

        // photons.begin()->flux = (4 * PI * light->powerValue()) / shots_taken;

        all_photons.insert(all_photons.end(), photons.begin(), photons.end());
        if (config.max_photons <= all_photons.size()) break;
        
    }

    // printList(all_photons);
    PhotonMap map = PhotonMap(all_photons, PhotonAxisPosition());
    return map;

}




