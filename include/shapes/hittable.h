#ifndef HITTABLE_H
#define HITTABLE_H
#include "common.h"
#include "aabb.h"
#include "onb.h"
class material;  // alert the compiler that the pointer is to a class
class hittable;
//hit_record:记录光线与物体的交点信息，包含交点坐标，法向量，光线参数t，是否正面朝向，交点材质
struct hit_record {
    pointf3 p;//交点世界坐标
    vecf3 normal;//法向量
    double t;//光线走过的距离参数t: p = O + t*D
    double u,v;//纹理坐标
    bool front_face;//是否正面朝向
    double density;//体密度
    shared_ptr<material> mat_ptr;//交点材质
    shared_ptr<hittable> boundary_ptr= nullptr;//交点物体
    inline void set_face_normal(const ray& r_in, const vecf3& outward_normal) {
        front_face = dot(r_in.direction(), outward_normal) < 0;
        normal = front_face ? outward_normal : -outward_normal;
    }
};

class hittable {
   public:

    /*  hit:
    
        给定一条光线，判断是否与该物体相交，若相交，将修改hit_record

        t_min和t_max是光线可以遍历的参数范围，只判断在这个范围内的交点

        不考虑物体的前后遮挡关系，只考虑是否相交
    */
    virtual bool hit(const ray& r, double t_min, double t_max, hit_record& rec) const = 0;
    
    //返回物体的包围盒 output_box. bool值表示是否有包围盒 比如无限大平面就没有
    virtual bool bounding_box(double time0, double time1, aabb& output_box) const = 0;
    
    //输入光线的起始点o和方向v，返回光线与物体相交点的pdf值
    //它表示从物体表面采样的概率密度函数
    virtual double pdf_value(const pointf3& o, const vecf3& v) const {
        return 0.0;
    }
    //输入光线的起点，返回从该点出发到物体表面采样点的方向
    virtual vecf3 random(const vecf3& o) const {
        return vecf3(1, 0, 0);
    }

    virtual void getMaterial(shared_ptr<material>& mptr) const{
        return;
    }
};


//平移
class translate: public hittable {
   public:
    translate(shared_ptr<hittable> p, const vecf3& displacement)
        : ptr(p), offset(displacement) {}

    virtual bool hit(const ray& r, double t_min, double t_max, hit_record& rec) const override;

    virtual bool bounding_box(double time0, double time1, aabb& output_box) const override;

    virtual void getMaterial(shared_ptr<material>& mptr) const override{
         ptr->getMaterial(mptr);
    }

   public:
    shared_ptr<hittable> ptr;
    vecf3 offset;
};

//旋转
enum class Axis { X, Y, Z };

class rotate : public hittable {
public:
    rotate(shared_ptr<hittable> p, double angle, Axis axis);

    virtual bool hit(
            const ray& r,
            double t_min,
            double t_max,
            hit_record& rec) const override;

    virtual bool bounding_box(double time0, double time1, aabb& output_box) const override {
        output_box = bbox;
        return hasbox;
    }
    virtual void getMaterial(shared_ptr<material>& mptr) const override {
        ptr->getMaterial(mptr);
    }
public:
    shared_ptr<hittable> ptr;
    double sin_theta;
    double cos_theta;
    bool hasbox;
    aabb bbox;
    Axis axis;
};


//翻转：改变材料的内外朝向，最直接的应用是对于无限大平面，可以通过翻转法向量来改变平面的朝向
class flip_face : public hittable {
   public:
    flip_face(shared_ptr<hittable> p) : ptr(p) {}

    virtual bool hit(
        const ray& r,
        double t_min,
        double t_max,
        hit_record& rec) const override;

    virtual bool bounding_box(double time0, double time1, aabb& output_box) const override {
        return ptr->bounding_box(time0, time1, output_box);
    }
    virtual void getMaterial(shared_ptr<material>& mptr) const override{
        ptr->getMaterial(mptr);
    }
   public:
    shared_ptr<hittable> ptr;

};

#endif