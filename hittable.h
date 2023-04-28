#ifndef HITTABLE_H
#define HITTABLE_H
#include "rtweekend.h"
#include "aabb.h"
class material;  // alert the compiler that the pointer is to a class
struct hit_record {
    pointf3 p;//交点世界坐标
    vecf3 normal;//法向量
    double t;//光线参数
    double u,v;//纹理坐标
    bool front_face;//是否正面朝向
    shared_ptr<material> mat_ptr;//交点材质

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
    
    //返回物体的包围盒 output_box. bool值表示是否有包围盒 比如无限大平面就没有
    virtual bool bounding_box(double time0, double time1, aabb& output_box) const = 0;
};

#endif