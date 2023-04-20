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


#endif