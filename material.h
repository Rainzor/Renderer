#ifndef MATERIAL_H
#define MATERIAL_H

#include "rtweekend.h"
#include "hittable.h"

class material {
   public:
    virtual bool scatter(
        const ray& r_in,
        const hit_record& rec,//击中点
        rgbf& attenuation,//衰减系数,对不同的颜色吸收不一样，则物体展现出不同的颜色
        ray& scattered//散射光方向
        ) const = 0;
};

class lambertian:public material{
    public:
        lambertian(const rgbf& a):albedo(a){}
        virtual bool scatter(
            const ray& r_in,
            const hit_record& rec,
            rgbf& attenuation,
            ray& scattered) const override{
                auto scatter_direction = rec.normal + random_unit_vector();
                // Catch degenerate scatter direction
                if (scatter_direction.near_zero())
                    scatter_direction = rec.normal;
                scattered = ray(rec.p, scatter_direction);
                attenuation = albedo;
                return true;
            }
    public:
        rgbf albedo;
};

class metal:public material{
    public:
        metal(const rgbf&a,double f):albedo(a),fuzz(f<1?f:1){}
        virtual bool scatter(//输出吸收系数和散射方向
            const ray& r_in,
            const hit_record& rec,
            rgbf& attenuation,
            ray& scattered) const override {
                vecf3 reflected = reflect(unit_vector(r_in.direction()), rec.normal);
                scattered = ray(rec.p, reflected+fuzz*random_in_unit_sphere());
                attenuation = albedo;
                return (dot(scattered.direction(), rec.normal) > 0);
        }

    public:
        rgbf albedo;
        double fuzz;
};


class dielectric:public material{
    public:
        dielectric(double index_of_refraction):ir(index_of_refraction){}

        virtual bool scatter(
            const ray& r_in,
            const hit_record& rec,
            rgbf& attenuation,
            ray& scattered) const override{
                attenuation = rgbf(1.0, 1.0, 1.0);//透明
                double refraction_ratio = rec.front_face ? (1.0 / ir) : ir;//判断是进入还是离开物体
                vecf3 unit_direction = unit_vector(r_in.direction());
                double cos_theta = fmin(dot(-unit_direction, rec.normal), 1.0);//cosθ
                double sin_theta = sqrt(1.0 - cos_theta * cos_theta);//sinθ
                bool cannot_refract = refraction_ratio * sin_theta > 1.0;//不能折射
                vecf3 direction;
                if (cannot_refract || reflectance(cos_theta, refraction_ratio) > random_double())//不能折射
                    direction = reflect(unit_direction, rec.normal);
                else
                    direction = refract(unit_direction, rec.normal, refraction_ratio);
                scattered = ray(rec.p, direction);
                return true;
            }

    private:
        static double reflectance(double cosine, double ref_idx) {
            // Use Schlick's approximation for reflectance.
            auto r0 = (1-ref_idx) / (1+ref_idx);
            r0 = r0*r0;
            return r0 + (1-r0)*pow((1 - cosine),5);
        }


    public:
        double ir;//折射率


};

#endif