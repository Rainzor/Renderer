//
// Created by Runze on 01/07/2023.
//

#ifndef RENDER_MESH_TRIANGLE_H
#define RENDER_MESH_TRIANGLE_H
#include "common.h"
#include "triangle.h"
#include "hittable_list.h"

class mesh_triangle:hittable{
public:
    mesh_triangle(){}
    mesh_triangle(const std::vector<pointf3>& vertexes,const std::vector<int> faces);

public:
    pointf3 aabb_min;
    pointf3 aabb_max;
    hittable_list meshes;
    std::vector<pointf3> vertexes;
    std::vector<int> faces;
    std::vector<texf2> texCoords;
    shared_ptr<material> mat_ptr;
};





#endif //RENDER_MESH_TRIANGLE_H
