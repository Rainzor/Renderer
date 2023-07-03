#ifndef BVH_H
#define BVH_H

#include <algorithm>
#include "common.h"
#include "hittable.h"
#include "hittable_list.h"
#include "empty.h"
#include <stack>
// 按照盒子的axis轴中最小值来比较两个盒子的度量大小
inline bool box_compare(const shared_ptr<hittable>& a, const shared_ptr<hittable>& b, int axis) {
    aabb box_a;
    aabb box_b;

    if (!a->bounding_box(0, 0, box_a) || !b->bounding_box(0, 0, box_b))
        std::cerr << "No bounding box in bvh_node constructor.\n";
    return box_a.min().e[axis] < box_b.min().e[axis];
}

inline bool box_x_compare(const shared_ptr<hittable>& a, const shared_ptr<hittable>& b) {
    return box_compare(a, b, 0);
}

inline bool box_y_compare(const shared_ptr<hittable>& a, const shared_ptr<hittable>& b) {
    return box_compare(a, b, 1);
}

inline bool box_z_compare(const shared_ptr<hittable>& a, const shared_ptr<hittable>& b) {
    return box_compare(a, b, 2);
}

int partition(std::vector<shared_ptr<hittable>>& objects,int start,int end,const int& axis);
int getMidNumber(std::vector<shared_ptr<hittable>> &objects, int start, int end, int pos,const int& axis);
class bvh_node:public hittable{
public:
        bvh_node(){}
        bvh_node(hittable_list&list,double time0,double time1)
            : bvh_node(list.objects,0,list.objects.size(), time0,time1)
        {}
//        bvh_node(const std::vector<shared_ptr<hittable>>& src_objects,
//                 size_t start, size_t end, double time0, double time1, shared_ptr<hittable> parent);

        bvh_node(std::vector<shared_ptr<hittable>>& src_objects,
                 size_t start,
                 size_t end,
                 double time0,
                 double time1);

//    bvh_node(const std::vector<shared_ptr<hittable>>& src_objects,
//             double time0,
//             double time1);

        virtual bool hit(const ray&r,double t_min,double t_max,hit_record&re)const override;
        virtual bool bounding_box(double time0, double time1, aabb& output_box)const override;
public:
        aabb box;//当前结点的包围盒
        shared_ptr<hittable> left;//由于指针指向的是虚拟基类，所以可以指向任何派生类
        shared_ptr<hittable> right;
};




#endif