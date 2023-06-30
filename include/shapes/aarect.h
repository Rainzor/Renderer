#ifndef AARECT_H
#define AARECT_H

#include "common.h"

#include "hittable.h"

class xy_rect : public hittable {//法向量为(0,0,1)
    public:
        xy_rect(){}

        xy_rect(double _x0, double _x1, double _y0, double _y1, double _k, shared_ptr<material> mat)
            : x0(_x0), x1(_x1), y0(_y0), y1(_y1), k(_k),mp(mat){};

        virtual bool hit(const ray& r, double t_min, double t_max, hit_record& rec) const override;

        virtual bool bounding_box(double time0, double time1, aabb& output_box) const override;

        virtual double pdf_value(const pointf3& o, const vecf3& v) const override;

        virtual vecf3 random(const vecf3& o) const override;
        virtual void getMaterial(shared_ptr<material>& mptr) const override{
            mptr = mp;
        }
    public:
        double x0, x1, y0, y1, k;
        shared_ptr<material> mp;
};

class xz_rect : public hittable {//法向量为(0,1,0)
       public:
        xz_rect() {}

        xz_rect(double _x0, double _x1, double _z0, double _z1, double _k, shared_ptr<material> mat)
            : x0(_x0), x1(_x1), z0(_z0), z1(_z1), k(_k), mp(mat){};

        virtual bool hit(const ray& r, double t_min, double t_max, hit_record& rec) const override;

        virtual bool bounding_box(double time0, double time1, aabb& output_box) const override;

        virtual double pdf_value(const pointf3& o, const vecf3& v) const override;
        virtual vecf3 random(const vecf3& o) const override;
        virtual void getMaterial(shared_ptr<material>& mptr) const override{
            mptr = mp;
        }
       public:
        double x0, x1, z0, z1, k;
        shared_ptr<material> mp;
};


class yz_rect : public hittable {//法向量为(1,0,0)
       public:
        yz_rect() {}

        yz_rect(double _y0, double _y1, double _z0, double _z1, double _k, shared_ptr<material> mat)
            : y0(_y0), y1(_y1), z0(_z0), z1(_z1), k(_k), mp(mat){};

        virtual bool hit(const ray& r, double t_min, double t_max, hit_record& rec) const override;

        virtual bool bounding_box(double time0, double time1, aabb& output_box) const override;

        virtual double pdf_value(const pointf3& o, const vecf3& v) const override;

        virtual vecf3 random(const vecf3& o) const override;
        virtual void getMaterial(shared_ptr<material>& mptr) const override{
            mptr = mp;
        }
       public:
        double y0, y1, z0, z1, k;
        shared_ptr<material> mp;
};
#endif