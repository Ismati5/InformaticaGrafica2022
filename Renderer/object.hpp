/**
 * @file object.hpp
 * @author Adrián Yago & Ismael Tienda
 * @brief
 * @version 0.1
 * @date 2022-10-29
 *
 * @copyright Copyright (c) 2022
 *
 */
#pragma once

#include <iostream>
#include <fstream>
#include <vector>
#include <string>

#include "triangle.hpp"
#include "vect4.hpp"
#include "matrix4.hpp"

#define MAX_POLYGON 20000

using namespace std;

/**
 * @brief Object class
 *
 */
class Object
{
public:
    int numPolygons;
    string name;
    Material material;
    Triangle *triangles[MAX_POLYGON];

    Point c;

    Object(string _name, string obj, Material mat)
    {

        name = _name;

        c = Point(0, 0, 0);

        ifstream file(obj);

        if (!file.is_open())
        {
            cout << "[!] Error openning " << obj << " file!" << endl;
            exit(1);
        }

        vector<Point> vertexes;
        vector<Vect3> faces;

        string line;
        while (getline(file, line))
        {
            if (line.find("v ", 0) == 0) // Vertexes
            {
                Point p;
                sscanf(line.c_str(), "v %f %f %f", &p.x, &p.y, &p.z);
                vertexes.push_back(p);
            }
            else if (line.find("f ", 0) == 0) // Faces
            {
                Vect3 v;
                float w;
                float trash;
                int matches = sscanf(line.c_str(), "f %f %f %f %f", &v.x, &v.y, &v.z, &w);
                if (matches == 3)
                {
                    faces.push_back(v);
                }
                else if (matches == 4)
                {
                    faces.push_back(v);
                    Vect3 v2(v.x, v.z, w);
                    faces.push_back(v2);
                }
            }
        }

        int e = 0;
        for (Vect3 i : faces)
        {
            triangles[e] = new Triangle();

            triangles[e]->p1 = vertexes.at(i.x - 1);
            triangles[e]->p2 = vertexes.at(i.y - 1);
            triangles[e]->p3 = vertexes.at(i.z - 1);
            triangles[e]->sertNormal();
            triangles[e]->setMaterial(mat);
            e++;
        }

        file.close();

        numPolygons = faces.size();
    }

    int getPolygons()
    {
        return numPolygons;
    }

    Triangle *getTriangles(int i)
    {
        return triangles[i];
    }

    void scale(float s)
    {
        if (s < 0)
        {
            s = 1 / abs(s);
        }
        for (int i = 0; i < numPolygons; i++)
        {
            triangles[i]->p1 = triangles[i]->p1 * s;
            triangles[i]->p2 = triangles[i]->p2 * s;
            triangles[i]->p3 = triangles[i]->p3 * s;
            triangles[i]->sertNormal();
        }
    }

    void translate(Direction dir)
    {

        c = c + dir;

        for (int i = 0; i < numPolygons; i++)
        {
            triangles[i]->p1 = triangles[i]->p1 + dir;
            triangles[i]->p2 = triangles[i]->p2 + dir;
            triangles[i]->p3 = triangles[i]->p3 + dir;
            triangles[i]->sertNormal();
        }
    }

    /**
     * @brief Rotates th radians given
     *          an axis "a"(0=x, 1=y, 2=z)
     *
     * @param th
     * @param a
     */
    void rotate(float th, int a)
    {
        for (int i = 0; i < numPolygons; i++)
        {
            Vect4 aux_vect4(triangles[i]->p1);
            Matrix4 T = tm_rotation(th, a);
            aux_vect4 = T * aux_vect4;
            triangles[i]->p1 = aux_vect4.toPoint();

            aux_vect4 = Point(triangles[i]->p2);
            T = tm_rotation(th, a);
            aux_vect4 = T * aux_vect4;
            triangles[i]->p2 = aux_vect4.toPoint();

            aux_vect4 = Point(triangles[i]->p3);
            T = tm_rotation(th, a);
            aux_vect4 = T * aux_vect4;
            triangles[i]->p3 = aux_vect4.toPoint();

            triangles[i]->sertNormal();
        }
    }
};
