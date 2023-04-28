#ifndef SPHERE_H
#define SPHERE_H

#include "hittable.h"
#include "vec3.h"

class sphere : public hittable {
    public:
        sphere() {}
        sphere(pointf3 cen, float r, shared_ptr<material> m)
            : center(cen), radius(r),mat_ptr(m){};

        virtual bool hit(const ray& r, double t_min, double t_max, hit_record& rec) const override;
        virtual bool bounding_box(double time0, double time1, aabb& output_box) const override;
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
    get_sphere_uv(outward_normal, rec.u, rec.v);//获取纹理坐标
    rec.mat_ptr = mat_ptr;//返回材料性质
    return true;
}

bool sphere::bounding_box(double time0, double time1, aabb& output_box) const {
    output_box = aabb(
        center - vecf3(radius, radius, radius),
        center + vecf3(radius, radius, radius)
    );
    return true;
}

#endif