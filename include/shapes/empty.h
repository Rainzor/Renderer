//
// Created by Runze on 03/07/2023.
//

#ifndef RENDER_EMPTY_H
#define RENDER_EMPTY_H
#include "common.h"
#include "hittable.h"
class empty : public hittable {
public:
    empty() = default;

    virtual bool hit(const ray &r, double t_min, double t_max, hit_record &rec) const override {
        return false;
    }

    virtual bool bounding_box(double time0, double time1, aabb &output_box) const override {
        return false;
    }
};


#endif //RENDER_EMPTY_H
