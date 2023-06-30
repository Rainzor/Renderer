#include "hittable_list.h"
bool hittable_list::hit(const ray& r, double t_min, double t_max, hit_record& rec) const {
    hit_record temp_rec;
    bool hit_anything = false;
    double closest_so_far = t_max;

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

double hittable_list::pdf_value(const vecf3 &o, const vecf3 &v) const {
    auto size = objects.size();
    auto sum = 0.0;
    double pdf_value = 0.0;
    for (const auto& object : objects) {
        pdf_value = object->pdf_value(o, v);
        sum += pdf_value;
    }
    return sum/size;
}

vecf3 hittable_list::random(const vecf3 &o) const {
    auto int_size = static_cast<int>(objects.size());
    do{
       vecf3 rand_dir = objects[random_int(0, int_size - 1)]->random(o);
       if(rand_dir.length()!=0) return rand_dir;
    } while (true);
}