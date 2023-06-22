#ifndef AARECT_H
#define AARECT_H

#include "rtweekend.h"

#include "hittable.h"

class xy_rect : public hittable {//法向量为(0,0,1)
    public:
        xy_rect(){}

        xy_rect(double _x0, double _x1, double _y0, double _y1, double _k, shared_ptr<material> mat)
            : x0(_x0), x1(_x1), y0(_y0), y1(_y1), k(_k),mp(mat){};

        virtual bool hit(const ray& r, double t_min, double t_max, hit_record& rec) const override;

        virtual bool bounding_box(double time0, double time1, aabb& output_box) const override {
            // The bounding box must have non-zero width in each dimension, so pad the Z
            // dimension a small amount.
            output_box = aabb(pointf3(x0, y0, k - 0.0001), pointf3(x1, y1, k + 0.0001));
            return true;
        }

    public:
        double x0, x1, y0, y1, k;
        shared_ptr<material> mp;
};

bool xy_rect::hit(const ray& r, double t_min, double t_max, hit_record& rec) const {
        auto t = (k - r.origin().z()) / r.direction().z();//根据纵坐标计算z
        if (t < t_min || t > t_max)
            return false;
        auto x = r.origin().x() + t * r.direction().x();
        auto y = r.origin().y() + t * r.direction().y();
        if (x < x0 || x > x1 || y < y0 || y > y1)
            return false;
        rec.u = (x - x0) / (x1 - x0);
        rec.v = (y - y0) / (y1 - y0);
        rec.t = t;

        auto outward_normal = vecf3(0, 0, 1);  // 法线
        // 修正朝向，使得与入射光线方向相反
        // 修改rec的 front_face判定，判断该面是否朝向光线
        rec.set_face_normal(r, outward_normal);
        rec.mat_ptr = mp;
        rec.p = r.at(t);
        return true;
}

class xz_rect : public hittable {//法向量为(0,1,0)
       public:
        xz_rect() {}

        xz_rect(double _x0, double _x1, double _z0, double _z1, double _k, shared_ptr<material> mat)
            : x0(_x0), x1(_x1), z0(_z0), z1(_z1), k(_k), mp(mat){};

        virtual bool hit(const ray& r, double t_min, double t_max, hit_record& rec) const override;

        virtual bool bounding_box(double time0, double time1, aabb& output_box) const override {
            // The bounding box must have non-zero width in each dimension, so pad the Y
            // dimension a small amount.
            output_box = aabb(pointf3(x0, k - 0.0001, z0), pointf3(x1, k + 0.0001, z1));
            return true;
        }

       public:
        double x0, x1, z0, z1, k;
        shared_ptr<material> mp;
};
bool xz_rect::hit(const ray& r, double t_min, double t_max, hit_record& rec) const {
        auto t = (k - r.origin().y()) / r.direction().y();
        if (t < t_min || t > t_max)
            return false;
        auto x = r.origin().x() + t * r.direction().x();
        auto z = r.origin().z() + t * r.direction().z();
        if (x < x0 || x > x1 || z < z0 || z > z1)
            return false;
        rec.u = (x - x0) / (x1 - x0);
        rec.v = (z - z0) / (z1 - z0);
        rec.t = t;
        auto outward_normal = vecf3(0, 1, 0);
        rec.set_face_normal(r, outward_normal);
        rec.mat_ptr = mp;
        rec.p = r.at(t);
        return true;
}

class yz_rect : public hittable {//法向量为(1,0,0)
       public:
        yz_rect() {}

        yz_rect(double _y0, double _y1, double _z0, double _z1, double _k, shared_ptr<material> mat)
            : y0(_y0), y1(_y1), z0(_z0), z1(_z1), k(_k), mp(mat){};

        virtual bool hit(const ray& r, double t_min, double t_max, hit_record& rec) const override;

        virtual bool bounding_box(double time0, double time1, aabb& output_box) const override {
            // The bounding box must have non-zero width in each dimension, so pad the X
            // dimension a small amount.
            output_box = aabb(pointf3(k - 0.0001, y0, z0), pointf3(k + 0.0001, y1, z1));
            return true;
        }

       public:
        double y0, y1, z0, z1, k;
        shared_ptr<material> mp;
};

bool yz_rect::hit(const ray& r, double t_min, double t_max, hit_record& rec) const {
        auto t = (k - r.origin().x()) / r.direction().x();
        if (t < t_min || t > t_max)
            return false;
        auto y = r.origin().y() + t * r.direction().y();
        auto z = r.origin().z() + t * r.direction().z();
        if (y < y0 || y > y1 || z < z0 || z > z1)
            return false;
        rec.u = (y - y0) / (y1 - y0);
        rec.v = (z - z0) / (z1 - z0);
        rec.t = t;
        auto outward_normal = vecf3(1, 0, 0);
        rec.set_face_normal(r, outward_normal);
        rec.mat_ptr = mp;
        rec.p = r.at(t);
        return true;
}
#endif