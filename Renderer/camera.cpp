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

    cout << " - Progress   [";
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

    float ray_x = ((float(rand()) / float(RAND_MAX)) * pixelSize.x) - pixelSize.x;
    float ray_y = ((float(rand()) / float(RAND_MAX)) * pixelSize.y) - pixelSize.y;
    float ray_z = ((float(rand()) / float(RAND_MAX)) * pixelSize.z) - pixelSize.z;
    return Direction(ray_x, ray_y, ray_z);
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
intersectionType Camera::closestObj(vector<Primitive *> objs, Ray ray, Direction &closest_normal, Point &closest_point,
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

            if (t1 < lowest_t1 && t1 > 0.0001)
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
void Camera::direct_light(vector<Primitive *> objs, Vect3 &emission,
                          Point x, Direction w0, vector<Light *> light_points,
                          Direction n, Vect3 color, float shadowBias, Material material, Vect3 brdf)
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
            // cout <<"NOT SHADOW" << endl;
            // Left term (Li)
            aux_emission = light->power / ((x - light->center).modulus() * (x - light->center).modulus());

            // Middle term (fr)

            aux_emission *= (brdf / PI);
            // cout << "SSS: " << (fr(x, wi, w0, material, material_type) / PI) << endl;
            // Right term
            aux_emission *= abs(n.dotProd((light->center - x).normalize()));

            emission += aux_emission;
        }
    }
}

/**
 * @brief Recursive light function
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
void Camera::light_value(vector<Primitive *> objs, Vect3 &emission, Point x, Direction w0,
                         vector<Light *> light_points, Direction n, Vect3 color, float shadowBias, string name, Material material, float ref_coef)
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
    Vect3 closest_emission;
    Direction closest_normal;
    string closest_name;

    Vect3 ld(0, 0, 0), lx(0, 0, 0);

    materialType material_type;
    Vect3 brdf = fr(x, wi, w0, material, material_type);

    if (material_type == ABSORTION) // Case absortion
    {
        emission = Vect3(0, 0, 0);
        return;
    }
    else if (material_type == SPECULAR)
    {
        wi = (w0 - (n * 2 * (w0.dotProd(n)))).normalize();
    }
    else if (material_type == REFRACTION)
    {
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

        /*cout << "x: " << x << endl;
        cout << "n: " << n << endl;
        cout << "name: " << name << endl;
        cout << "w0: " << w0 << endl;
        cout << "wi: " << wi << endl;
        cout << "no: " << no << endl;
        cout << "ni: " << ni << endl;

        char a;
        cin >> a;*/
    }

    Ray ray(wi, x);

    Material material_aux;

    intersectionType intersected = closestObj(objs, ray, closest_normal, closest_point, closest_emission, w0, closest_name, material, material_aux);

    if (intersected == NONE) // No intersection with scene
    {
        emission = Vect3(0, 0, 0);
        return;
    }
    else if (intersected == LIGHT) // Intersection with area light
    {
        emission = closest_emission;
        return;
    }

    light_value(objs, lx, closest_point, wi, light_points, closest_normal, closest_emission, shadowBias, closest_name, material_aux, material.ref_coef);

    if (material_type == SPECULAR || material_type == REFRACTION)
    {
        emission = lx;
        return;
    }

    // Light from point sources
    direct_light(objs, ld, x, w0, light_points, n, color, shadowBias, material, brdf);

    emission = ld + lx * brdf;
    // cout << "B - Emission from: " << name << " = " << emission << " (ld = " << ld << ", fr = " << brdf << ", lx = " << lx << ")" << endl;
}

/**
 * @brief Render a tile
 *
 * @param id
 * @param objs
 * @param lights
 * @param config
 * @param num_tile
 * @param max_emission
 */
void Camera::render_thread(int id, vector<Primitive *> objs, vector<Light *> lights, render_config &config, atomic_int &num_tile, atomic_int &max_emission)
{

    srand(time(NULL));

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
                    Point pixel = topLeft - pixelSize_x * j - pixelSize_y * i - variation_x - variation_y;
                    ray.d = (pixel - origin).normalize();

                    // check intersections

                    Direction w0;
                    Direction closest_normal;
                    string closest_name;
                    Point closest_x;
                    Material closest_material;
                    bool closest_light = false;
                    Vect3 closest_power;

                    materialType material_type;

                    closest_emission = Vect3(0, 0, 0);

                    for (Primitive *i : objs)
                    {
                        if (i->intersect(ray, t1, sur_normal, x))
                        {
                            if (t1 < lowest_t1)
                            {
                                lowest_t1 = t1;

                                if (i->isLight())
                                {
                                    closest_light = true;
                                    closest_power = i->power();
                                }
                                else
                                    closest_light = false;

                                closest_material = i->material;
                                closest_name = i->name;
                                closest_normal = sur_normal;
                                closest_x = x;

                                w0 = (x - origin).normalize();
                                intersected = true;
                            }
                        }
                    }

                    lowest_t1 = numeric_limits<float>::infinity();

                    if (intersected)
                    {

                        if (config.pathtracing)
                        {

                            if (!closest_light)
                                light_value(objs, closest_emission, closest_x, w0, lights, closest_normal, closest_material.kd, config.shadow_bias, closest_name, closest_material, 1);
                            else
                                closest_emission = closest_power * fr(closest_x, ray.d, w0, closest_material, material_type);

                            /*if (closest_name == "refraction_sphere")
                            {
                                cout << "Hit with " << closest_name << endl;
                                cout << "Total emission: " << closest_emission << " from ray " << r << endl
                                     << endl
                                     << endl;

                                char a;
                                cin >> a;
                            }*/
                        }
                        else
                            closest_emission = closest_material.kd; // Without path tracing

                        intersections++;
                        total_emission += closest_emission;
                        intersected = false;
                    }
                }

                if (intersections > 0)
                {

                    /*cout << endl
                         << "@@@ TOTAL: "
                         << total_emission << " from " << config.rays << " intersections. @@@" << endl
                         << endl;

                    cout << i << ":" << j << "  -> " << intersections << endl;*/

                    total_emission /= (float)config.rays;

                    /*cout << endl
                         << "@@@ - Final TOTAL: "
                         << total_emission << " - @@@" << endl
                         << endl;*/

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

vector<Photon> Camera::search_nearest(PhotonMap map, Vect3 x, unsigned long K, float r)
{
    // Position to look for the nearest photons
    Vect3 query_position = x;

    // Maximum number of photons to look for
    unsigned long nphotons_estimate = K;

    // Maximum distance to look for photons
    float radius_estimate = r;

    // nearest is the nearest photons returned by the KDTree
    vector<const Photon *> aux = map.nearest_neighbors(query_position,
                                                       nphotons_estimate,
                                                       radius_estimate);

    vector<Photon> photons;
    for (const Photon *ph : aux)
    {
        photons.push_back(*ph);
    }
    return photons;
}

bool Camera::hitPosition(vector<Primitive *> objects, Ray ray, Direction &n, Point &x, Material &m)
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

Vect3 Camera::emission_ph(vector<Primitive *> objs, vector<Light *> lights, render_config config, PhotonMap map, Point x, Direction w0, Direction n, Material material)
{
    materialType material_type;
    Vect3 brdf = fr(x, Direction(0,0,0), w0, material, material_type, true); // No se puede absorber
    if (material_type == DIFFUSE)
    {
        materialType type;
        vector<Photon> photons = search_nearest(map, x.toVect3(), config.k, config.r);

        Vect3 leftComp = Vect3(0, 0, 0);
        Vect3 rightComp = Vect3(0, 0, 0);
        Vect3 ld = Vect3(0, 0, 0);

        Vect3 kernel_dens = Vect3(0, 0, 0);
        for (Photon ph : photons)
        {
            leftComp = fr(x, ph.wp, w0, ph. material, type);
            // rightComp = ph.flux;
            rightComp = ph.flux * (1 - x.distance(ph.position_) / config.r); // Non-Constant density estimation

            // cout << "D: " << x.distance(ph.position_) << endl;
            // cout << "P: " << distance << endl;

            kernel_dens += leftComp * rightComp / (PI * config.r * config.r);
        }

        direct_light(objs, ld, x, w0, lights, n, material.kd, config.shadow_bias, material, brdf);
        // cout << ld << "--" << kernel_dens << "--" << brdf << endl;
        return ld + kernel_dens * brdf;

    }else if (material_type == SPECULAR)
    {
        w0 = (w0 - (n * 2 * (w0.dotProd(n)))).normalize();
    }
    else // REFRACTION
    {
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
            w0 = Direction(0, 0, 0);
        else
            w0 = (w0 * nf + auxN * (nf * angI - sqrtf(k))).normalize();
    }
    
    Ray ray(w0, x);
    if (hitPosition(objs, ray, n, x, material))
    {
        return emission_ph(objs, lights, config, map, x, w0, n, material);   
    }
    else
    {
        return Vect3(0,0,0);
    }

}

void Camera::renderPhoton_thread(int id, vector<Primitive *> objs, vector<Light *> lights, render_config &config, atomic_int &num_tile, atomic_int &max_emission, PhotonMap map)
{

    srand(time(NULL));

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
                    Point pixel = topLeft - pixelSize_x * j - pixelSize_y * i - variation_x - variation_y;
                    ray.d = (pixel - origin).normalize();

                    // check intersections

                    Direction w0;
                    Direction closest_normal;
                    string closest_name;
                    Point closest_x;
                    Material closest_material;
                    bool closest_light = false;
                    Vect3 closest_power;

                    materialType material_type;

                    closest_emission = Vect3(0, 0, 0);

                    for (Primitive *i : objs)
                    {

                        if (i->intersect(ray, t1, sur_normal, x))
                        {
                            if (t1 < lowest_t1)
                            {
                                lowest_t1 = t1;

                                if (i->isLight())
                                {
                                    closest_light = true;
                                    closest_power = i->power();
                                }
                                else
                                    closest_light = false;

                                closest_material = i->material;
                                closest_name = i->name;
                                closest_normal = sur_normal;
                                closest_x = x;

                                w0 = (x - origin).normalize();
                                intersected = true;
                            }
                        }
                    }
                    lowest_t1 = numeric_limits<float>::infinity();
                    if (intersected)
                    {
                        closest_emission = emission_ph(objs, lights, config, map, closest_x, w0, closest_normal, closest_material);

                        intersections++;
                        // cout << closest_emission << endl;
                        total_emission += closest_emission;
                        intersected = false;
                    }
                }

                if (intersections > 0)
                {
                    total_emission /= (float)config.rays;

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