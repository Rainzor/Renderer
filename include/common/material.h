#ifndef MATERIAL_H
#define MATERIAL_H

#include "common.h"
#include "hittable.h"
#include "texture.h"
#include "onb.h"
#include "pdf.h"

/*
Scatter_record结构体用来记录材料的散射信息
*/
struct scatter_record {
    ray scatter_ray;//光线
    bool is_specular= false;//是否为镜面反射
    bool is_medium = false;
    bool is_refract = false;
    color attenuation;//材质的吸收系数
    shared_ptr<pdf> pdf_ptr;//散射光线的概率分布函数
};

enum class material_type {
    Lambertian,
    Metal,
    Glass,
    DiffuseLight,
    Isotropic
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
    // 返回该材质散射光方向的概率密度函数，代表了BRDF的一项
    virtual double scattering_pdf(
        const ray &r_in,
        const hit_record &rec,
        const ray &scattered) const {
        return 0;
    }

    virtual void getType(material_type &m) const {
        return;
    }

public:
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

    virtual void getType(material_type &m) const override {
        m = material_type::Lambertian;
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
        srec.scatter_ray = ray(rec.p, reflected + fuzz * random_in_unit_sphere(), r_in.time());
        srec.attenuation = albedo;
        srec.is_specular = true;
        srec.pdf_ptr = nullptr;
        return true;
    }

    virtual void getType(material_type &m) const override {
        m = material_type::Metal;
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
        srec.pdf_ptr = nullptr;
        double refraction_ratio = rec.front_face ? (1.0 / ir) : ir;  // 判断是进入还是离开物体
        vecf3 unit_direction = unit_vector(r_in.direction());
        double cos_theta = fmin(dot(-unit_direction, rec.normal), 1.0);  // cosθ
        double sin_theta = sqrt(1.0 - cos_theta * cos_theta);   // sinθ
        bool cannot_refract = refraction_ratio * sin_theta > 1.0;// 不能折射
        vecf3 direction;
        if (cannot_refract || reflectance(cos_theta, refraction_ratio) > random_double()) {
            //不能折射
            direction = reflect(unit_direction, rec.normal);
            srec.is_refract=false;
            srec.is_specular = true;
        }
        else {// 可以折射
            direction = refract(unit_direction, rec.normal, refraction_ratio);
            srec.is_refract=true;
            srec.is_specular = true;
        }
        srec.scatter_ray = ray(rec.p, direction, r_in.time());
        return true;
    }

    virtual void getType(material_type &m) const override {
        m = material_type::Glass;
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
        emitt(a) {
    }
    diffuse_light(color c) :
        emitt(make_shared<solid_color>(c)) {
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
            return emitt->value(u, v, p);
        return color(0, 0, 0); 
    }

    virtual void getType(material_type &m) const override {
        m = material_type::DiffuseLight;
    }

public:
    shared_ptr<texture> emitt;
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
        srec.attenuation = albedo->value(rec.u, rec.v, rec.p);
        srec.pdf_ptr = make_shared<uniform_pdf>();
        srec.is_specular= false;
        srec.is_medium = true;
        srec.scatter_ray = ray(r_in);
        return true;
    }

    virtual void getType(material_type &m) const override {
        m = material_type::Isotropic;
    }

    double scattering_pdf(
            const ray &r_in,
            const hit_record &rec,
            const ray &scattered) const override {
        return 1/(4*pi);
    }

public:
    shared_ptr<texture> albedo;
};


// 参与介质，phase_function为散射函数
// 各向同性的散射材质
class phase : public material {
public:
    phase(color c) :
            albedo(make_shared<solid_color>(c)) {
    }
    phase(shared_ptr<texture> a) :
            albedo(a) {
    }

    virtual bool scatter(
            const ray &r_in,
            const hit_record &rec,
            scatter_record &srec
    ) const override {
        // 散射方向为随机的单位球内的随机点，保持散射方向的各向同性
        srec.attenuation = albedo->value(rec.u, rec.v, rec.p);
        srec.pdf_ptr = make_shared<uniform_pdf>();
        return true;
    }

    virtual void getType(material_type &m) const override {
        m = material_type::Isotropic;
    }

public:
    shared_ptr<texture> albedo;
};

#endif