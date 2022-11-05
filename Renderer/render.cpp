/**
 * @file render.cpp
 * @author Adrián Yago & Ismael Tienda
 * @brief
 * @version 0.1
 * @date 2022-10-28
 *
 * @copyright Copyright (c) 2022
 *
 */
#include <iostream>
#include <fstream>
#include <string>
#include "point.hpp"
#include "direction.hpp"
#include "vect3.hpp"
#include "vect4.hpp"
#include "matrix4.hpp"
#include "sphere.hpp"
#include "station.hpp"
#include "camera.hpp"
#include "light.hpp"

using namespace std;

Vect3 red = Vect3(255, 109, 106);
Vect3 yellow = Vect3(233, 236, 107);
Vect3 blue = Vect3(139, 211, 230);
Vect3 green = Vect3(119, 221, 119);
Vect3 gray = Vect3(207, 207, 196);
Vect3 purple = Vect3(177, 162, 202);
Vect3 orange = Vect3(239, 190, 125);
Vect3 white = Vect3(255, 255, 255);


int loadOBJfile(Triangle triangles[10000], string fileName, Vect3 emi) {
    
    ifstream file(fileName);

    if (!file.is_open())
    {
        cout << "Error opening de OBJ file" << endl;
        return -1;
    }

    vector<Point> vertices;
    vector<Vect3> faces;

    string line;
    while (getline(file, line))
    {
        if (line.find("v ", 0) == 0) //Vertices
        {
            Point p;
            sscanf(line.c_str(), "v %f %f %f", &p.x, &p.y, &p.z);
            vertices.push_back(p);
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
            else if(matches == 4)
            {
                faces.push_back(v);
                Vect3 v2(v.x, v.z, w);
                faces.push_back(v2);
            }

        }       
    }

    // for (Vect3 i : faces)
    // {
    //     cout << i << endl;
    // }
    

    int e = 0;
    for (Vect3 i : faces)
    {
        triangles[e].p1 = vertices.at(i.x - 1);
        triangles[e].p2 = vertices.at(i.y - 1);
        triangles[e].p3 = vertices.at(i.z - 1);
        triangles[e].setEmission(emi);     
        triangles[e].sertNormal();
        //cout << triangles[e].normal << endl;
        // Triangle triangle(
        //     vertices.at(i.x - 1),
        //     vertices.at(i.y - 1),
        //     vertices.at(i.z - 1),
        //     emi);
        // objs.push_back(&triangles[e]);
        e++;
    }

    file.close();

    return faces.size();

}

void createRender(string file, int rays)
{
    Point o(0, 35, 0);
    Direction l(1, 0, 0);
    Direction u(0, 0, 1);
    Direction f(0, -1, 0);
    int size[2] = {400, 400};

    Camera camera(l, u, f, o, size);

    vector<Light *> lights;
    Point l_c1(0, 30, 0);
    Vect3 l_p1 = Vect3(5000, 5000, 5000);
    Light light1(l_c1, l_p1);
    lights.push_back(&light1);

    vector<Object *> objs;
    Triangle triangles[30000];

    int numPolygons = loadOBJfile(triangles, "objs/Sword.obj", red);

    for (int i = 0; i < numPolygons; i++)
    {
        objs.push_back(&triangles[i]);
        // cout << triangles[i].p3 << endl;
    }

    Direction n(1, 0, 0);
    Plane left_plane(n, 1, orange);
    //  objs.push_back(&left_plane);
    Direction n1(-1, 0, 0);
    Plane right_plane(n1, 1, yellow);
    //  objs.push_back(&right_plane);
    Direction n2(0, 0, 1);
    Plane floor_plane(n2, 1, gray);
    // objs.push_back(&floor_plane);
    Direction n3(0, -1, 0);
    Plane ceiling_plane(n3, 1, red);
    //  objs.push_back(&ceiling_plane);
    Direction n4(0, 0, -1);
    Plane back_plane(n4, 1, purple);
    //  objs.push_back(&back_plane);

    Point c(-0.5, -0.7, 0.25);
    Sphere left_sphere(c, 0.3, blue);
    // objs.push_back(&left_sphere);
    Point c1(0.5, -0.7, -0.25);
    Sphere right_sphere(c1, 0.3, green);
    // objs.push_back(&right_sphere);

    Point p1(-0.5, -0.7, 0.25);
    Point p2(0, -0.7, 0.25);
    Point p3(-0.25, 0, 0.25);
    Triangle triangle(p1, p2, p3, red);
    //objs.push_back(&triangle);

    camera.render(file, objs, rays, lights);
}

int main(int argc, char *argv[])
{

    if (argc == 3)
    {
        createRender(argv[1], stoi(argv[2]));
    }
    else
    {
        cout << "[!] Usage: renderer <filename.ppm> <rays per pixel>" << endl;
    }

    return 0;
}