//
// Created by Runze on 01/07/2023.
//

#ifndef RENDER_MESH_TRIANGLE_H
#define RENDER_MESH_TRIANGLE_H
#include "common.h"
#include "triangle.h"
#include "hittable_list.h"

class mesh_triangle: public hittable{
public:
    mesh_triangle(){}
    mesh_triangle(const std::vector<pointf3>& vertexes,const std::vector<int> faces,shared_ptr<material> m);
    virtual bool hit(const ray& r, double t_min, double t_max, hit_record& rec) const override;

    virtual bool bounding_box(double time0, double time1, aabb& output_box) const override;
    virtual void getMaterial(shared_ptr<material>& mptr) const override{
        mptr = mat_ptr;
    }
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
