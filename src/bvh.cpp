#include "bvh.h"

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
