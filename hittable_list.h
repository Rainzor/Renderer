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

        void clear() { objects.clear(); }
        void add(shared_ptr<hittable> object) { objects.push_back(object); }

        virtual bool hit(const ray& r, double t_min, double t_max, hit_record& rec) const override;
        virtual bool bounding_box(double time0, double time1, aabb& output_box) const override;

       public:
        // 采用共享指针指向hittable对象，这样可以避免对象的拷贝，提高效率
        // 使用共享指针的好处是，能够在多个地方共享同一个对象，而不用担心对象的生命周期问题
        std::vector<shared_ptr<hittable>> objects;
};

bool hittable_list::hit(const ray& r, double t_min, double t_max, hit_record& rec) const {
    hit_record temp_rec;
    bool hit_anything = false;
    auto closest_so_far = t_max;

    for (const auto& object : objects) {
        if (object->hit(r, t_min, closest_so_far, temp_rec)) {
            hit_anything = true;
            closest_so_far = temp_rec.t;
            rec = temp_rec;
        }
    }
    return hit_anything;
}

bool hittable_list::bounding_box(double time0, double time1, aabb&ouput_box)const{
    //总包围盒:根结点
    if(objects.empty()) return false;
    aabb temp_box;
    bool first_box = true;
    for(const auto& object : objects){
        if(!object->bounding_box(time0,time1,temp_box)) return false;//存在一个物体没有包围盒，就返回false
        ouput_box = first_box ? temp_box : surrounding_box(ouput_box,temp_box);//上一个物体所在盒子和当前物体所在盒子的包围盒的并集
        first_box = false;
    }
    return true;
}

#endif