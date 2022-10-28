#pragma once

#include <iostream>
#include <fstream>
#include "direction.hpp"
#include "sphere.hpp"
#include "plane.hpp"
#include "point.hpp"
#include "ray.hpp"
#include <cmath>
#include <vector>
#include <limits>

using namespace std;

class Camera{

public:
    Point origin;
    Direction L, U, F; //L = x, U = z, F = y
    int size[2];

    Point topLeft, topRight, botLeft, botRight;
    Direction pixelSize_x, pixelSize_y;

    Camera(Direction _L, Direction _U, Direction _F, Point O, int _size[2]): 
        L(_L), U(_U), F(_F), origin(O){

        size[0] = _size[0];
        size[1] = _size[1];

        topLeft = origin + U + L + F;
        topRight = origin + U - L + F;
        botLeft = origin - U + L + F;
        botRight = origin - U - L + F;

        pixelSize_x = (topLeft - topRight) / size[0];
        pixelSize_y = (topLeft - botLeft) / size[1];

    }

    void render(vector<Sphere> sphere_objs, vector<Plane> plane_objs) {

        //file header
        ofstream file("output.ppm");
        file << "P3" << endl;
        file << "# output.ppm" << endl;
        file << size[0] << " " << size[1] << endl;
        file << "15" << endl;
 
        Ray ray;
        ray.p = origin;

        float closest_emission[3];
        bool intersected = false;
        float t1,t2, lowest_t1 = numeric_limits<float>::infinity();
        Direction sur_normal;

        for (float i = 0; i < size[0]; i++){
           for (float j = 0; j < size[1]; j++){
                Point pixel = topLeft + pixelSize_x * i + pixelSize_y * j;
                ray.d = (pixel - origin).normalize();

                //check sphere intersections
                for(Sphere i : sphere_objs){
                    if (i.Intersect(ray, t1, sur_normal)){
                        if (t1 > 0 && t1 < lowest_t1) {
                            lowest_t1 = t1;
                            closest_emission[0] = i.emission[0];
                            closest_emission[1] = i.emission[1];
                            closest_emission[2] = i.emission[2];
                            intersected = true;
                        }
                    }
                }

                //check plane intersections
                for(Plane i : plane_objs){
                    if (i.Intersect(ray, t1, sur_normal)){
                        if (t1 > 0 && t1 < lowest_t1) {
                            lowest_t1 = t1;
                            closest_emission[0] = i.emission[0];
                            closest_emission[1] = i.emission[1];
                            closest_emission[2] = i.emission[2];
                            intersected = true;
                        }
                    }
                }
                
                //cout << "Intersected: " << intersected << endl;
                //cout << "Distance: " << lowest_t1 << endl;

                if (intersected) {
                    file << closest_emission[0] << " " << closest_emission[1] << " " << closest_emission[2] << "    ";
                    intersected = false;
                    lowest_t1 = numeric_limits<float>::infinity();
                }
                
            } 
            file << endl;
        }

        file.close();

    }


};