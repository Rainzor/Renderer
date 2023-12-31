#ifndef MOVING_SPHERE_H
#define MOVING_SPHERE_H

#include "common.h"

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

        pointf3 center(double time) const {
            return center0 + ((time - time0) / (time1 - time0)) * (center1 - center0);
        }

        virtual void getMaterial(shared_ptr<material>& mptr) const override{
            mptr = mat_ptr;
        }
    public:
        pointf3 center0, center1;
        double time0, time1;
        double radius;
        shared_ptr<material> mat_ptr;
};

#endif