#ifndef MATERIAL_H
#define MATERIAL_H

#include "rtweekend.h"
#include "hittable.h"
#include "texture.h"
/* Material类是一个抽象类，它的子类有lambertian，metal，dielectric，diffuse_light
   它主要用来决定光线的散射方向和吸收系数
*/
class material {
   public:
    //材料本身具有自发光的属性，可以认为是一种特殊的纹理
    virtual color emitted(double u, double v, const pointf3& p) const {
        return color(0, 0, 0);
    }
    //不同的材料对光线有不同的散射方程和颜色吸收系数
    virtual bool scatter(
        const ray& r_in,
        const hit_record& rec,//击中点
        rgbf& attenuation,//衰减系数,对不同的颜色吸收不一样，则物体展现出不同的颜色
        ray& scattered//散射光方向
        ) const = 0;
};

class lambertian:public material{
    public:
        lambertian(shared_ptr<texture> a):albedo(a){}
        lambertian(const color& a): albedo(make_shared<solid_color>(a)) {}

        virtual bool scatter(
            const ray& r_in,
            const hit_record& rec,
            rgbf& attenuation,
            ray& scattered) const override{
                auto scatter_direction = rec.normal + random_unit_vector();
                // Catch degenerate scatter direction
                if (scatter_direction.near_zero())
                    scatter_direction = rec.normal;
                scattered = ray(rec.p, scatter_direction,r_in.time());//光速很快，可以认为光线在同一时刻发出，所以在一个光线弹射时为固定时刻
                attenuation = albedo->value(rec.u, rec.v, rec.p);//根据击中点的纹理坐标和坐标，从纹理类中获取当前的纹理颜色
                return true;
            }
    public:
     shared_ptr<texture> albedo;
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
                scattered = ray(rec.p, reflected+fuzz*random_in_unit_sphere(),r_in.time());
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
                scattered = ray(rec.p, direction,r_in.time());
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


class diffuse_light:public material{
    public:
        diffuse_light(shared_ptr<texture> a):emit(a){}
        diffuse_light(color c):emit(make_shared<solid_color>(c)){}

        virtual bool scatter(
            const ray& r_in,
            const hit_record& rec,
            rgbf& attenuation,
            ray& scattered) const override{
                return false;
            }

        virtual rgbf emitted(double u,double v,const pointf3& p)const override{
            return emit->value(u,v,p);
        }

    public:
        shared_ptr<texture> emit;
};

#endif