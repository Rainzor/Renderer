//
// Created by Runze on 01/07/2023.
//

#ifndef RENDER_MESH_TRIANGLE_H
#define RENDER_MESH_TRIANGLE_H
#include "common.h"
#include "triangle.h"
#include "hittable_list.h"
#include "model.h"
#include "bvh.h"
#include <map>
class mesh_triangle: public hittable{
public:
    mesh_triangle(){}
    mesh_triangle(const std::vector<pointf3>& vertices,const std::vector<int>& faces,shared_ptr<material> m);
    mesh_triangle(const std::string& filename, shared_ptr<material> m,int scale = 1);
    virtual bool hit(const ray& r, double t_min, double t_max, hit_record& rec) const override;

    virtual bool bounding_box(double time0, double time1, aabb& output_box) const override;
    virtual void getMaterial(shared_ptr<material>& mptr) const override{
        mptr = mat_ptr;
    }
private:
    void Init(const std::vector<pointf3>& vertices,const std::vector<int>& faces,shared_ptr<material> m);
public:
    size_t num;
    pointf3 aabb_min;
    pointf3 aabb_max;
    hittable_list triangles;
    shared_ptr<bvh_node> bvh;
//    std::vector<pointf3> vertices;
//    std::vector<texf2> texCoords;
//    std::vector<vecf3> normals;
//    std::vector<int> vertInds;
//    std::vector<int> stInds;
//    std::vector<int> normInds;
    shared_ptr<material> mat_ptr;
};





#endif //RENDER_MESH_TRIANGLE_H
