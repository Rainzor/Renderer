#ifndef BVH_H
#define BVH_H

#include <algorithm>
#include "rtweekend.h"
#include "hittable.h"
#include "hittable_list.h"

// 按照盒子的axis轴中最小值来比较两个盒子的度量大小
inline bool box_compare(const shared_ptr<hittable> a, const shared_ptr<hittable> b, int axis) {
    aabb box_a;
    aabb box_b;

    if (!a->bounding_box(0, 0, box_a) || !b->bounding_box(0, 0, box_b))
        std::cerr << "No bounding box in bvh_node constructor.\n";
    return box_a.min().e[axis] < box_b.min().e[axis];
}

bool box_x_compare(const shared_ptr<hittable> a, const shared_ptr<hittable> b) {
    return box_compare(a, b, 0);
}

bool box_y_compare(const shared_ptr<hittable> a, const shared_ptr<hittable> b) {
    return box_compare(a, b, 1);
}

bool box_z_compare(const shared_ptr<hittable> a, const shared_ptr<hittable> b) {
    return box_compare(a, b, 2);
}

class bvh_node:public hittable{
    public:
        bvh_node(){}
        bvh_node(const hittable_list&list,double time0,double time1)
            : bvh_node(list.objects,0,list.objects.size(),time0,time1)
        {}

        bvh_node(const std::vector<shared_ptr<hittable>>& src_objects,
                 size_t start,
                 size_t end,
                 double time0,
                 double time1);
        virtual bool hit(const ray&r,double t_min,double t_max,hit_record&re)const override;
        virtual bool bounding_box(double time0, double time1, aabb& output_box)const override;
       public:
        shared_ptr<hittable> left;//由于指针指向的是虚拟基类，所以可以指向任何派生类
        shared_ptr<hittable> right;
        aabb box;//当前结点的包围盒
};

bvh_node::bvh_node(
        const std::vector<shared_ptr<hittable>>& src_objects,
        size_t start,size_t end, double time0,double time1){
    auto objects = src_objects;//创建一个副本，对副本进行操作，不改变原始数据
    int axis = random_int(0,2);//随机选择的一个轴来进行排序
    //比较函数器！
    auto comparator = (axis == 0) ? box_x_compare
                    : (axis == 1) ? box_y_compare
                    : box_z_compare;
    size_t object_span = end - start;
    if(object_span == 1){//如果只有一个物体，将复制一份，避免空节点
        left = right = objects[start];
    }else if(object_span == 2){//如果只有两个物体，直接比较
        if(comparator(objects[start],objects[start+1])){
            left = objects[start];
            right = objects[start+1];
        }else{
            left = objects[start+1];
            right = objects[start];
        }
    }else{//如果有多个物体，将物体按照axis轴排序，然后取中间的物体作为分割点
        std::sort(objects.begin()+start,objects.begin()+end,comparator);//时间复杂度为O(nlogn)
        auto mid = start + object_span/2;//中位数
        left = make_shared<bvh_node>(objects,start,mid,time0,time1);//将排序好的Objs再次进行递归
        right = make_shared<bvh_node>(objects,mid,end,time0,time1);
    }
    aabb box_left,box_right;
    if (!left->bounding_box(time0, time1, box_left) || !right->bounding_box(time0, time1, box_right)) {
        std::cerr << "No bounding box in bvh_node constructor.\n";
    }
    box = surrounding_box(box_left,box_right);
}


bool bvh_node::bounding_box(double time0, double time1, aabb& output_box)const{
    output_box = box;
    return true;
}
//判断光线是否与当前结点的包围盒相交，如果相交，再判断是否与左右子树相交，直到叶子结点
bool bvh_node::hit(const ray&r,double t_min,double t_max,hit_record&rec)const{
    // 中序遍历
    //如果当前结点的包围盒没有被击中，直接返回false,避免无效的搜索
    if(!box.hit(r,t_min,t_max)) return false;
    bool hit_left = left->hit(r,t_min,t_max,rec);
    bool hit_right = right->hit(r,t_min,hit_left ? rec.t : t_max,rec);
    return hit_left || hit_right;
}



#endif