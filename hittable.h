#ifndef HITTABLE_H
#define HITTABLE_H
#include "rtweekend.h"
class material;  // alert the compiler that the pointer is to a class
struct hit_record {
    pointf3 p;
    vecf3 normal;
    double t;
    bool front_face;
    shared_ptr<material> mat_ptr;

    inline void set_face_normal(const ray& r, const vecf3& outward_normal) {
        front_face = dot(r.direction(), outward_normal) < 0;
        normal = front_face ? outward_normal : -outward_normal;
    }
};

class hittable {
   public:

   //给定一条光线，判断是否与物体相交，若相交，将修改hit_record
   //t_min和t_max是光线的参数范围，只判断在这个范围内的交点
    virtual bool hit(const ray& r, double t_min, double t_max, hit_record& rec) const = 0;

};

#endif