#ifndef MOVING_SPHERE_H
#define MOVING_SPHERE_H

#include "rtweekend.h"

#include "hittable.h"

class moving_sphere: public hittable {
    public:
        moving_sphere() {}
        moving_sphere(
            pointf3 cen0, pointf3 cen1, double _time0, double _time1, double r, shared_ptr<material> m)
            : center0(cen0), center1(cen1), time0(_time0), time1(_time1), radius(r), mat_ptr(m)
        {};

        virtual bool hit(
            const ray& r, double t_min, double t_max, hit_record& rec) const override;

        virtual bool bounding_box(double _time0, double _time1, aabb& output_box) const override;

        pointf3 center(double time) const;

    public:
        pointf3 center0, center1;
        double time0, time1;
        double radius;
        shared_ptr<material> mat_ptr;
};

pointf3 moving_sphere::center(double time) const {
    return center0 + ((time - time0) / (time1 - time0)) * (center1 - center0);
}

bool moving_sphere::hit(const ray& r, double t_min, double t_max, hit_record& rec) const {
    //根据入射光线的时间，计算当前球心的位置
    vecf3 oc = r.origin() - center(r.time());//oc:origin-center change with time
    auto a = r.direction().length_squared();
    auto half_b = dot(oc, r.direction());
    auto c = oc.length_squared() - radius * radius;

    auto discriminant = half_b * half_b - a * c;
    if (discriminant < 0) return false;
    auto sqrtd = sqrt(discriminant);

    // Find the nearest root that lies in the acceptable range.
    auto root = (-half_b - sqrtd) / a;
    if (root < t_min || t_max < root) {
        root = (-half_b + sqrtd) / a;
        if (root < t_min || t_max < root)
            return false;
    }

    rec.t = root;
    rec.p = r.at(rec.t);
    vecf3 outward_normal = (rec.p - center(r.time())) / radius;//normal vector change with time
    rec.set_face_normal(r, outward_normal);
    rec.mat_ptr = mat_ptr;

    return true;
}

bool moving_sphere::bounding_box(double _time0, double _time1, aabb& output_box) const {
    aabb box0(
        center(_time0) - vecf3(radius, radius, radius),
        center(_time0) + vecf3(radius, radius, radius)
    );
    aabb box1(
        center(_time1) - vecf3(radius, radius, radius),
        center(_time1) + vecf3(radius, radius, radius)
    );
    output_box = surrounding_box(box0, box1);
    return true;
}

#endif