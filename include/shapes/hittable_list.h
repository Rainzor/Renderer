#ifndef HITTABLE_LIST_H
#define HITTABLE_LIST_H

#include "hittable.h"

#include <memory>
#include <vector>

using std::make_shared;
using std::shared_ptr;

class hittable_list:public hittable{
    public:
        hittable_list() {}
        hittable_list(shared_ptr<hittable> object) { add(object); }
        hittable_list(const std::vector<shared_ptr<hittable>>& objects) { this->objects = objects; }
        void clear() { objects.clear(); }
        void add(shared_ptr<hittable> object) { objects.push_back(object); }

        //寻找与光线相交的最近的物体交点
        virtual bool hit(const ray& r, double t_min, double t_max, hit_record& rec) const override;
        virtual bool bounding_box(double time0, double time1, aabb& output_box) const override;

        virtual double pdf_value(const vecf3 &o, const vecf3 &v) const override;
        virtual vecf3 random(const vecf3 &o) const override;

       public:
        // 采用共享指针指向hittable对象，这样可以避免对象的拷贝，提高效率
        // 使用共享指针的好处是，能够在多个地方共享同一个对象，而不用担心对象的生命周期问题
        std::vector<shared_ptr<hittable>> objects;
};

#endif