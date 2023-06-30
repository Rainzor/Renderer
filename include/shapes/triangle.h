#ifndef TRIANGLE_H
#define TRIANGLE_H

#include "common.h"
#include "hittable.h"
#include "vec3.h"

class triangle : public hittable {
public:
    triangle() {}
    triangle(const pointf3& a, const pointf3& b, const pointf3& c, shared_ptr<material> m)
            : v0(a), v1(b), v2(c), mat_ptr(m) {};

    virtual bool hit(const ray& r, double t_min, double t_max, hit_record& rec) const override;
    virtual bool bounding_box(double time0, double time1, aabb& output_box) const override;

    virtual void getMaterial(shared_ptr<material>& mptr) const override{
        mptr = mat_ptr;
    }

public:
    pointf3 v0, v1, v2; // 三角形的三个顶点
    shared_ptr<material> mat_ptr; // 材料属性
};
#endif
