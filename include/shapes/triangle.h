#ifndef TRIANGLE_H
#define TRIANGLE_H

#include "common.h"
#include "hittable.h"

class triangle : public hittable {
public:
    triangle() {}
    triangle(const pointf3& a, const pointf3& b, const pointf3& c, shared_ptr<material> m);
    triangle(const std::vector<pointf3>& vertexes,const std::vector<texf2>& textures, shared_ptr<material> m);

    virtual bool hit(const ray& r, double t_min, double t_max, hit_record& rec) const override;
    virtual bool bounding_box(double time0, double time1, aabb& output_box) const override;

    virtual void getMaterial(shared_ptr<material>& mptr) const override{
        mptr = mat_ptr;
    }
private:
    //获取三角形的纹理坐标
    void get_triangle_uv(const vecf3& p, double& u, double& v) const {
        // p: weight of three vertex
        u = p.x()*uv0.x() + p.y()*uv1.x() + p.z()*uv2.x();
        v = p.x()*uv0.y() + p.y()*uv1.y() + p.z()*uv2.y();
    }
public:
    pointf3 v0, v1, v2; // 三角形的三个顶点
    texf2 uv0, uv1, uv2; // 三角形的三个顶点的纹理坐标
    vecf3 normal; // 三角形的法向量
    vecf3 tangent; // 三角形的切向量
    shared_ptr<material> mat_ptr; // 材料属性
};


#endif
