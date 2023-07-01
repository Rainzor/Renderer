#ifndef RAYTRACER_PARTICIPATE_MEDIUM_H
#define RAYTRACER_PARTICIPATE_MEDIUM_H
#include "common.h"

#include "hittable.h"
#include "material.h"
#include "texture.h"

//参与介质（适用于凸边界）
class participate_medium : public hittable {
public:
    participate_medium(shared_ptr<hittable> b,shared_ptr<texture> a, double sa=0.5, double ss=0.5, double d=1.0)
            : boundary(b),
              phase_function(make_shared<isotropic>(a)),
              sigma_a(sa),
              sigma_s(ss),
              density(d)
              {}

    participate_medium(shared_ptr<hittable> b,color c,double sa=0.5, double ss=0.5, double d=1.0)
            : boundary(b),
              phase_function(make_shared<isotropic>(c)),
              sigma_a(sa),
              sigma_s(ss),
              density(d)
              {}


    virtual bool hit(
            const ray& r,
            double t_min,
            double t_max,
            hit_record& rec) const override;

    virtual bool bounding_box(double time0, double time1, aabb& output_box) const override {
        return boundary->bounding_box(time0, time1, output_box);
    }
    virtual void getMaterial(shared_ptr<material>& mptr) const override{
        mptr = phase_function;
    }

    void getDensity(double& d) const{
        d = density;
    }

    void getSigma_a(double& sa) const{
        sa = sigma_a;
    }

    void getSigma_s(double& ss) const{
        ss = sigma_s;
    }

public:
    shared_ptr<hittable> boundary;
    shared_ptr<material> phase_function;
    double sigma_a;
    double sigma_s;
    double density;
};
#endif //RAYTRACER_PARTICIPATE_MEDIUM_H
