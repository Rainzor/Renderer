#ifndef MATERIAL_H
#define MATERIAL_H

#include "rtweekend.h"
#include "hittable.h"
#include "texture.h"
#include "onb.h"
#include "pdf.h"

/*
Scatter_record结构体用来记录材料的散射信息，包括：
1. 散射光线
2. 是否为镜面反射
3. 吸收系数
4. 采样出射方向时的概率密度函数类，它也可以用来采样散射方向
*/
struct scatter_record {
    ray specular_ray;
    bool is_specular;
    color attenuation;
    shared_ptr<pdf> pdf_ptr;
};

/* Material类是一个抽象类，它的子类有lambertian，metal，dielectric，diffuse_light
   它主要用来决定光线的散射方向和吸收系数
*/
class material {
public:
    // 材料本身具有自发光的属性，可以认为是一种特殊的纹理
    virtual color emitted(
        const ray &r_in, const hit_record &rec, double u, double v, const pointf3 &p) const {
        return color(0, 0, 0);
    }
    // 不同的材料对光线有不同的散射方程和颜色吸收系数, 二者组成了BRDF
    virtual bool scatter(
        const ray &r_in,
        const hit_record &rec, // 击中点
        scatter_record &srec // 散射记录
    ) const {
        return false;
    }

    virtual double scattering_pdf( // 返回散射光方向的概率密度函数
        const ray &r_in,
        const hit_record &rec,
        const ray &scattered) const {
        return 0;
    }
};
// 漫反射粗糙材质
class lambertian : public material {
    // 粗糙的表面,其光反射分布接近漫反射，所以往往与入射光无关
public:
    lambertian(shared_ptr<texture> a) :
        albedo(a) {
    }
    lambertian(const color &a) :
        albedo(make_shared<solid_color>(a)) {
    }
    virtual bool scatter(
        const ray &r_in,
        const hit_record &rec,
        scatter_record &srec
        ) const override {
        srec.is_specular = false;
        srec.attenuation = albedo->value(rec.u, rec.v, rec.p);
        srec.pdf_ptr = make_shared<cosin_pdf>(rec.normal);
        return true;
    }

    double scattering_pdf(
        const ray &r_in,
        const hit_record &rec,
        const ray &scattered) const override {
        auto cosine = dot(rec.normal, unit_vector(scattered.direction()));
        return cosine < 0 ? 0 : cosine / pi;
    }

public:
    shared_ptr<texture> albedo;
};
// 反射金属材质
class metal : public material {
public:
    metal(const color &a, double f) :
        albedo(a), fuzz(f < 1 ? f : 1) {
    }
    virtual bool scatter( 
        const ray &r_in,
        const hit_record &rec,
        scatter_record &srec
        ) const override {
        vecf3 reflected = reflect(unit_vector(r_in.direction()), rec.normal);
        srec.specular_ray = ray(rec.p, reflected + fuzz * random_in_unit_sphere(), r_in.time());
        srec.attenuation = albedo;
        srec.is_specular = true;
        srec.pdf_ptr = nullptr;
        return true;
    }

public:
    color albedo;
    double fuzz;
};

// 折射透明材质
class dielectric : public material {
public:
    dielectric(double index_of_refraction) :
        ir(index_of_refraction) {
    }

    virtual bool scatter(
        const ray &r_in,
        const hit_record &rec,
        scatter_record &srec
        ) const override {
        srec.attenuation = color(1.0, 1.0, 1.0); // 透明
        srec.is_specular = true;
        srec.pdf_ptr = nullptr;
        double refraction_ratio = rec.front_face ? (1.0 / ir) : ir;  // 判断是进入还是离开物体
        vecf3 unit_direction = unit_vector(r_in.direction());
        double cos_theta = fmin(dot(-unit_direction, rec.normal), 1.0);  // cosθ
        double sin_theta = sqrt(1.0 - cos_theta * cos_theta);   // sinθ
        bool cannot_refract = refraction_ratio * sin_theta > 1.0;// 不能折射
        vecf3 direction;
        if (cannot_refract || reflectance(cos_theta, refraction_ratio) > random_double()) // 不能折射
            direction = reflect(unit_direction, rec.normal);
        else
            direction = refract(unit_direction, rec.normal, refraction_ratio);
        srec.specular_ray = ray(rec.p, direction, r_in.time());
        return true;
    }

private:
    static double reflectance(double cosine, double ref_idx) {
        // Use Schlick's approximation for reflectance.
        auto r0 = (1 - ref_idx) / (1 + ref_idx);
        r0 = r0 * r0;
        return r0 + (1 - r0) * pow((1 - cosine), 5);
    }

public:
    double ir; // 折射率
};

// 自发光材质
class diffuse_light : public material {
public:
    diffuse_light(shared_ptr<texture> a) :
        emit(a) {
    }
    diffuse_light(color c) :
        emit(make_shared<solid_color>(c)) {
    }

    virtual bool scatter(
        const ray &r_in,
        const hit_record &rec,
        scatter_record &srec
        ) const override {
        return false;
    }

    virtual color emitted(
        const ray &r_in, const hit_record &rec, double u, double v, const pointf3 &p) const override {
        if (rec.front_face)//只有材料的正面法向与入射光线方向相反时，才会发光
            return emit->value(u, v, p);
        return color(0, 0, 0); 
    }

public:
    shared_ptr<texture> emit;
};

// 各向同性的散射材质
class isotropic : public material {
public:
    isotropic(color c) :
        albedo(make_shared<solid_color>(c)) {
    }
    isotropic(shared_ptr<texture> a) :
        albedo(a) {
    }

    virtual bool scatter(
        const ray &r_in,
        const hit_record &rec,
        scatter_record &srec
        ) const override {
        // 散射方向为随机的单位球内的随机点，保持散射方向的各向同性
        srec.specular_ray = ray(rec.p, random_in_unit_sphere(), r_in.time());
        srec.attenuation = albedo->value(rec.u, rec.v, rec.p);
        return true;
    }

public:
    shared_ptr<texture> albedo;
};

#endif