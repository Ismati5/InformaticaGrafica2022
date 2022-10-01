#pragma once
/**
 * @file sphere.hpp
 * @author Adrián Yago & Ismael Tienda
 * @brief
 * @version 0.1
 * @date 2022-10-01
 *
 * @copyright Copyright (c) 2022
 *
 */
#include <iostream>
#include "direction.hpp"
#include "point.hpp"

using namespace std;

struct Reference
{
    float angle_g, angle_t;
};

/**
 * @brief Sphere represents a 3D sphere
 *
 */
class Sphere
{

public:
    Direction axis;
    Point center;

    Reference ref;

    /**
     * @brief Construct a new Sphere object
     *
     * @param axis
     * @param center
     * @param reference
     */
    Sphere(Direction axis, Point center)
        : axis(axis), center(center) {}

    /**
     * @brief Set the Reference object
     *
     * @param _ref
     */
    void setReference(Reference _ref);
};