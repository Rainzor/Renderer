#include "sphere.h"

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
