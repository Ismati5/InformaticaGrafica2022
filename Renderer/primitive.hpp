/**
 * @file primitive.hpp
 * @author Adrián Yago & Ismael Tienda
 * @brief
 * @version 0.1
 * @date 2022-10-29
 *
 * @copyright Copyright (c) 2022
 *
 */
#pragma once

#include "vect3.hpp"
#include "ray.hpp"
#include "material.hpp"

using namespace std;

/**
 * @brief Primitive class
 *
 */
class Primitive
{
public:
    // emission RGB tuple
    Direction normal;
    Material material;
    int type;

    string name = "none";

    /**
     * @brief intersect interface
     *
     * @param ray
     * @param t
     * @param sur_normal
     * @return true
     * @return false
     */
    virtual bool intersect(Ray ray, float &t, Direction &sur_normal, Point &x) = 0;

    void setMaterial(Material mat)
    {
        material = mat;
    }

    bool isLight()
    {
        return material.isLight();
    }

    Vect3 power()
    {
        return material.ke;
    }

    Vect3 color(Point x)
    {

        Vect3 kd;

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

        return kd;
    }
};
