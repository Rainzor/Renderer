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

bool sphere::hit(const ray& r, double t_min, double t_max, hit_record& rec)const{
    //求解一元二次方程，根据光线的方向和原点，求解光线与球的交点
    //判断光线在给定范围内是否与球相交
    vecf3 oc = r.origin() - center;//A-C
    auto a = r.direction().length_squared();
    if(a==0) return false;//光线方向为0，不相交
    auto half_b = dot(oc, r.direction());
    auto c = oc.length_squared() - radius * radius;

    auto discriminant = half_b*half_b - a*c;
    if(discriminant<0) return false;
    auto sqrtd = sqrt(discriminant);

    // Find the nearest root that lies in the acceptable range.
    auto root = (-half_b - sqrtd) / a;//靠近正向传播光源的交点，或者在物体内部光源后方
    if (root < t_min || t_max < root) {//如果光源在内部
        root = (-half_b + sqrtd) / a;
        if (root < t_min || t_max < root)
            return false;
    }
    rec.t = root;
    rec.p = r.at(rec.t);
    vecf3 outward_normal = (rec.p-center)/radius;
    rec.set_face_normal(r,outward_normal);
    get_sphere_uv(outward_normal, rec.u, rec.v);//根据三维位置获取球面的纹理坐标
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

double sphere::pdf_value(const pointf3& o, const vecf3& v) const {
    vecf3 direction = center - o;
    auto distance_squared = direction.length_squared();
    if(distance_squared<radius*radius) {
        //如果光源在球内部
        return 1 / (4*pi);
    }
    if(distance_squared==radius*radius){
        //如果光源在球外部
        return 0;
    }
    hit_record rec;
    if (!this->hit(ray(o, v), 0.001, infinity, rec))
        return 0;

    auto cos_theta_max = sqrt(1 - radius * radius / (center - o).length_squared());
    auto solid_angle = 2 * pi * (1 - cos_theta_max);

    return  1 / solid_angle;
}

vecf3 sphere::random(const vecf3& o) const {
    vecf3 direction = center - o;
    auto distance_squared = direction.length_squared();
    if(distance_squared<radius*radius){
        //如果光源在球内部
        return random_on_unit_sphere();
    }else if(distance_squared-radius*radius==0) {
        //光源在球上
        return vecf3 (0,0,0);
    }
    else{
        onb uvw;
        uvw.build_from_w(direction);
        return uvw.local(random_to_sphere(radius, distance_squared));
    }

}

#endif