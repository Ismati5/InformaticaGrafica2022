using namespace std;

#include "variables.hpp"
#include "material.hpp"
#include "light.hpp"
#include "ray.hpp"
#include "primitive.hpp"

Vect3 fr_ph(Point x, Direction wi, Direction w0, Material material, materialType &type, bool noAbsortion = false)
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

intersectionType closestObj_ph(vector<Primitive *> objs, Ray ray, Direction &closest_normal, Point &closest_point,
                               Direction w0, Material material, Material &intersectedMaterial)
{
    bool intersected = false;
    float t1, lowest_t1 = numeric_limits<float>::infinity();
    Direction normal;
    Point point;

    materialType material_type;
    intersectionType closest_type;

    for (Primitive *obj : objs)
    {
        if (obj->intersect(ray, t1, normal, point))
        {
            intersected = true;
            if (t1 < lowest_t1 && t1 > 0.0001)
            {
                lowest_t1 = t1;
                closest_point = point;
                closest_normal = normal;
                intersectedMaterial = obj->material;

                if (obj->isLight())
                    closest_type = LIGHT;
                else
                    closest_type = OBJECT;
            }
        }
    }

    if (intersected)
        return closest_type;

    return NONE;
}

void light_value_ph(vector<Primitive *> objs, Point x, Direction w0,
                    vector<Light *> light_points, Direction n, float shadowBias,
                    Material material, list<Photon> &photons, int max_photons, Vect3 prevFlux)
{

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

    materialType material_type;
    Vect3 brdf = fr_ph(x, wi, w0, material, material_type);

    if (material_type == ABSORTION)
        return;

    // Only store non diffuse intersections
    bool savePhoton = true;
    if (material_type == SPECULAR)
    {
        savePhoton = false;
        wi = (w0 - (n * 2 * (w0.dotProd(n)))).normalize();
    }
    else if (material_type == REFRACTION)
    {
        savePhoton = false;
        float no = 1; // Hay que tener en cuenta el medio por el que viene, no el ultimo medio visitado
        float nf, ni = material.ref_coef;

        Direction auxN = n;
        float angI = auxN.dotProd(w0.normalize());

        if (angI < -1)
            angI = -1;
        else if (angI > 1)
            angI = 1;

        if (angI < 0)
        {
            angI = -angI;
        }
        else
        {
            auxN = auxN * -1;
            swap(no, ni);
        }

        nf = no / ni;

        float k = 1 - nf * nf * (1 - angI * angI);
        if (k < 0)
            wi = Direction(0, 0, 0);
        else
            wi = (w0 * nf + auxN * (nf * angI - sqrtf(k))).normalize();
    }

    Ray ray(wi, x);
    Material intersected_material;
    intersectionType intersected = closestObj_ph(objs, ray, closest_normal, closest_point, w0, material, intersected_material);

    if (intersected == NONE) // No intersection with scene
        return;

    Photon ph;
    ph.position_ = closest_point.toVect3();
    ph.wp = wi;
    ph.flux = brdf * prevFlux;
    ph.material = material;

    // Save photon
    if (savePhoton)
        photons.push_front(ph);

    light_value_ph(objs, closest_point, wi, light_points, closest_normal, shadowBias, intersected_material, photons, max_photons, ph.flux);
}

Direction randomWalk()
{
    float theta = (float)(rand()) / (float)(RAND_MAX);
    float phi = (float)(rand()) / (float)(RAND_MAX);

    theta = acos(2 * theta - 1);
    phi = 2 * PI * phi;

    return Direction(sin(theta) * cos(phi), sin(theta) * sin(phi), cos(theta));
}

// Returns true if ray "ray" hits something, n, x and m will contain the data of the closest hit
bool hitPosition(vector<Primitive *> objects, Ray ray, Direction &n, Point &x, Material &m)
{
    bool intersected = false;
    float t1, lowest_t1 = numeric_limits<float>::infinity();
    Direction hitNormal;
    Point hitPoint;

    for (Primitive *obj : objects)
    {
        if (obj->intersect(ray, t1, hitNormal, hitPoint))
        {
            intersected = true;
            if (t1 < lowest_t1 && t1 > 0.0001)
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
        cout << "============================================" << endl;
        cout << "POSITION:" << ph.position_ << endl;
        cout << "WP:" << ph.wp << endl;
        cout << "FLUX:" << ph.flux << endl;
        cout << "============================================" << endl;
    }
    cout << "SIZE:" << list.size() << endl
         << endl;
}

/*
    Example function to generate the photon map with the given photons
*/
PhotonMap generation_of_photon_map(vector<Light *> lights, vector<Primitive *> objects, render_config config)
{

    srand(time(NULL));

    const size_t fixedListSize(config.max_photons);
    list<Photon> all_photons(fixedListSize);
    list<Photon> photons(fixedListSize);
    all_photons.clear();

    // Calculate the sum of the power of the lights
    float totalPower = 0;
    for (Light *light : lights)
        totalPower += light->powerValue();

    Ray ray;
    Point x;
    Direction n, wi;
    Material material;
    int max_shots = 0;
    for (Light *light : lights)
    {
        photons.clear();
        ray.p = light->center;
        max_shots = config.max_photons * (light->powerValue() / totalPower);
        for (int i = 0; i < max_shots; i++)
        {
            ray.d = randomWalk();
            if (hitPosition(objects, ray, n, x, material))
            {
                light_value_ph(objects, x, (x - light->center).normalize(), lights, n,
                               config.shadow_bias, material, photons, max_shots, (light->power * 4 * PI) / max_shots);
            }
        }

        all_photons.insert(all_photons.end(), photons.begin(), photons.end());
    }

    // printList(all_photons);
    PhotonMap map = PhotonMap(all_photons, PhotonAxisPosition());

    return map;
}
