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

#define MAX_POLYGON 20000

using namespace std;

/**
 * @brief Object class
 *
 */
class Object
{
public:
    Vect3 emission;
    int numPolygons;
    string name;
    Triangle triangles[MAX_POLYGON];

    Point c;

    Object(string _name, string obj, Vect3 color)
    {
        emission = color;
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
            triangles[e].p1 = vertexes.at(i.x - 1);
            triangles[e].p2 = vertexes.at(i.y - 1);
            triangles[e].p3 = vertexes.at(i.z - 1);
            triangles[e].setEmission(emission);
            triangles[e].sertNormal();
            e++;
        }

        file.close();

        numPolygons = faces.size();
    }

    int getPolygons()
    {
        return numPolygons;
    }

    Triangle *getTriangles()
    {
        return triangles;
    }

    void scale(float s)
    {
        if (s < 0)
        {
            s = 1 / abs(s);
        }
        for (int i = 0; i < numPolygons; i++)
        {
            triangles[i].p1 = triangles[i].p1 * s;
            triangles[i].p2 = triangles[i].p2 * s;
            triangles[i].p3 = triangles[i].p3 * s;
            triangles[i].sertNormal();
        }
        cout << " >> Se ha escalado el objeto " << name << "." << endl;
    }

    void translate(Direction dir)
    {

        c = c + dir;

        for (int i = 0; i < numPolygons; i++)
        {
            triangles[i].p1 = triangles[i].p1 + dir;
            triangles[i].p2 = triangles[i].p2 + dir;
            triangles[i].p3 = triangles[i].p3 + dir;
            triangles[i].sertNormal();
        }

        cout << " >> Se ha trasladado el objeto " << name << "." << endl;
    }
};
