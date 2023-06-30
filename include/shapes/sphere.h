#ifndef SPHERE_H
#define SPHERE_H
#include "common.h"
#include "hittable.h"
#include "onb.h"
#include "pdf.h"
class sphere : public hittable {
    public:
        sphere() {}
        sphere(pointf3 cen, float r, shared_ptr<material> m)
            : center(cen), radius(r),mat_ptr(m){};

        virtual bool hit(const ray& r, double t_min, double t_max, hit_record& rec) const override;
        virtual bool bounding_box(double time0, double time1, aabb& output_box) const override;

        virtual double pdf_value(const pointf3& o, const vecf3& v) const override;
        //按立体角均匀采样
        virtual vecf3 random(const vecf3& o) const override;

        virtual void getMaterial(shared_ptr<material>& mptr) const override{
            mptr = mat_ptr;
        }

    public:
        pointf3 center;//球心
        float radius;//半径
        shared_ptr<material> mat_ptr;//材料性质
    private:
        //获取纹理坐标
     static void get_sphere_uv(const vecf3& p, double& u, double& v) {
         // p: a given point on the sphere of radius one, centered at the origin.
         // u: returned value [0,1] of angle around the Y axis from X=-1.
         // v: returned value [0,1] of angle from Y=-1 to Y=+1.
         //     <1 0 0> yields <0.50 0.50>       <-1  0  0> yields <0.00 0.50>
         //     <0 1 0> yields <0.50 1.00>       < 0 -1  0> yields <0.50 0.00>
         //     <0 0 1> yields <0.25 0.50>       < 0  0 -1> yields <0.75 0.50>

         auto theta = acos(-p.y());
         auto phi = atan2(-p.z(), p.x()) + pi;

         //用极坐标作为纹理坐标   
         u = phi / (2 * pi);
         v = theta / pi;
     }
};
#endif