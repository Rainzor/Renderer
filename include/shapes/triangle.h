#ifndef TRIANGLE_H
#define TRIANGLE_H

#include "common.h"
#include "hittable.h"

class triangle : public hittable {
public:
    triangle() {}
    triangle(const pointf3& a, const pointf3& b, const pointf3& c, shared_ptr<material> m);
    triangle(const std::vector<pointf3>& vertexes, shared_ptr<material> m);
    triangle(const std::vector<pointf3>& vertexes,const std::vector<texf2>& textures, shared_ptr<material> m);
    triangle(const std::vector<pointf3>& vertexes,
             const std::vector<texf2>& textures,
             const std::vector<vecf3>& normals,
             shared_ptr<material> m);
    virtual bool hit(const ray& r, double t_min, double t_max, hit_record& rec) const override;
    virtual bool bounding_box(double time0, double time1, aabb& output_box) const override;

    virtual void getMaterial(shared_ptr<material>& mptr) const override{
        mptr = mat_ptr;
    }
private:
    //获取三角形的纹理坐标
    void get_triangle_uv(const vecf3& p, double& u, double& v) const {
        // p: weight of three vertex
        u = p.x()*vt0.x() + p.y()*vt1.x() + p.z()*vt2.x();
        v = p.x()*vt0.y() + p.y()*vt1.y() + p.z()*vt2.y();
    }
public:
    pointf3 v0, v1, v2; // 三角形的三个顶点
    texf2 vt0, vt1, vt2; // 三角形的三个顶点的纹理坐标
    vecf3 vn1,vn2,vn3; // 三角形的法向量
    shared_ptr<material> mat_ptr; // 材料属性
};


#endif
