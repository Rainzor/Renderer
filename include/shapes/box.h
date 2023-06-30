#ifndef BOX_H
#define BOX_H

#include "common.h"

#include "aarect.h"
#include "hittable_list.h"

class box : public hittable {
   public:
    box() {}
    box(const pointf3& p0, const pointf3& p1, shared_ptr<material> m);

    virtual bool hit(const ray& r, double t_min, double t_max, hit_record& rec) const override;

    virtual bool bounding_box(double time0, double time1, aabb& output_box) const override;
    virtual void getMaterial(shared_ptr<material>& mptr) const override{
        mptr = mat_ptr;
    }
   public:
    pointf3 box_min;
    pointf3 box_max;
    hittable_list sides;
    shared_ptr<material> mat_ptr;//材料性质
};



#endif