//
// Created by Runze on 01/07/2023.
//
#include "mesh_triangle.h"

mesh_triangle::mesh_triangle(const std::vector<pointf3> &vertexes, const std::vector<int> faces,shared_ptr<material> m) {
    this->vertexes = vertexes;
    this->faces = faces;
    this->aabb_min = vertexes[0];
    this->aabb_max = vertexes[0];
    for (int i = 0; i < vertexes.size(); i++) {
        if(vertexes[i].x() < aabb_min.x()) aabb_min.e[0] = vertexes[i].x();
        if(vertexes[i].y() < aabb_min.y()) aabb_min.e[1] = vertexes[i].y();
        if(vertexes[i].z() < aabb_min.z()) aabb_min.e[2] = vertexes[i].z();
        if(vertexes[i].x() > aabb_max.x()) aabb_max.e[0] = vertexes[i].x();
        if(vertexes[i].y() > aabb_max.y()) aabb_max.e[1] = vertexes[i].y();
        if(vertexes[i].z() > aabb_max.z()) aabb_max.e[2] = vertexes[i].z();
    }
    for (int i = 0; i < faces.size(); i += 3) {
        pointf3 a = vertexes[faces[i]];
        pointf3 b = vertexes[faces[i + 1]];
        pointf3 c = vertexes[faces[i + 2]];
        meshes.add(make_shared<triangle>(a, b, c, m));
    }
    mat_ptr = m;
}

bool mesh_triangle::hit(const ray &r, double t_min, double t_max, hit_record &rec) const {
    return meshes.hit(r, t_min, t_max, rec);
}

bool mesh_triangle::bounding_box(double time0, double time1, aabb &output_box) const {
    output_box = aabb(aabb_min, aabb_max);
    return true;
}