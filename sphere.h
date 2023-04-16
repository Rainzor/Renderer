#ifndef SPHERE_H
#define SPHERE_H

#include "hittable.h"
#include "vec3.h"

class sphere : public hittable {
    public:
        sphere() {}
        sphere(pointf3 cen, float r) : center(cen), radius(r) {};

        virtual bool hit(const ray& r, double t_min, double t_max, hit_record& rec) const override;

    public:
        pointf3 center;//球心
        float radius;//半径
};

bool sphere::hit(const ray& r, double t_min, double t_max, hit_record& rec)const{
    //求解一元二次方程，判断光线在给定范围内是否与球相交
    vecf3 oc = r.origin() - center;//A-C
    auto a = r.direction().length_squared();
    auto half_b = dot(oc, r.direction());
    auto c = oc.length_squared() - radius * radius;

    auto discriminant = half_b*half_b - a*c;
    if(discriminant<0) return false;
    auto sqrtd = sqrt(discriminant);

    // Find the nearest root that lies in the acceptable range.
    auto root = (-half_b - sqrtd) / a;//靠近相机的交点
    if (root < t_min || t_max < root) {
        root = (-half_b + sqrtd) / a;//远离相机的交点
        if (root < t_min || t_max < root)
            return false;
    }
    rec.t = root;
    rec.p = r.at(rec.t);
    vecf3 outward_normal = (rec.p-center)/radius;
    rec.set_face_normal(r,outward_normal);
    return true;
}

#endif