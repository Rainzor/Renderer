#include "bvh.h"


int partition(std::vector<shared_ptr<hittable>> &objects, int start, int end,const int& axis) {

    int idx = random_int(start, end - 1);

    int l = start, r = end - 1;
    shared_ptr<hittable> pivot = objects[idx];

    while (l<idx && box_compare(objects[l], pivot, axis))
        l++;
    if(l<idx)
        std::swap(objects[l], objects[idx]);

    while (l < r) {
        while (l < r && !box_compare(objects[r], pivot, axis))r--;
        if (l < r) {
            std::swap(objects[l], objects[r]);
            l++;
        }
        while (l < r && box_compare(objects[l], pivot, axis))l++;
        if (l < r) {
            std::swap(objects[l], objects[r]);
            r--;
        }
    }
    objects[l] = pivot;
    return l;
}
int getMidNumber(std::vector<shared_ptr<hittable>> &objects, int start, int end, int pos,const int& axis){
    while (true){
        int index = partition(objects, start, end, axis); // 获得基准数的位置
        if (index == pos){
            return index;
        }else if (index > pos){ // 只需要在[l, index-1]区间内找pos位置即可
            end = index;
        }else { // 只需要在[index, r]区间内找pos位置即可
            start = index + 1;
        }
    }
    return -1; // 一般程序不会到这里
}

bvh_node::bvh_node(
        std::vector<shared_ptr<hittable>> &src_objects,
        size_t start, size_t end, double time0, double time1) {
    //创建一个副本，对副本进行操作，不改变原始数据
    int axis = random_int(0, 2);//随机选择的一个轴来进行排序
    //比较函数器！
    auto comparator = (axis == 0) ? box_x_compare
                                  : (axis == 1) ? box_y_compare
                                                : box_z_compare;
    size_t object_span = end - start;
    if (object_span == 1) {//如果只有一个物体，将复制一份，避免空节点
        left = right = src_objects[start];
    } else if (object_span == 2) {//如果只有两个物体，直接比较
        if (comparator(src_objects[start], src_objects[start + 1])) {
            left = src_objects[start];
            right = src_objects[start + 1];
        } else {
            left = src_objects[start + 1];
            right = src_objects[start];
        }
    } else {//如果有多个物体，将物体按照axis轴排序，然后取中间的物体作为分割点

        //O(n)
        int mid = getMidNumber(src_objects, start, end, start + object_span / 2, axis);
//        std::sort(src_objects.begin() + start, src_objects.begin() + end, comparator);//时间复杂度为O(nlogn)
//        auto mid = start + object_span / 2;//中位数
        left = make_shared<bvh_node>(src_objects, start, mid, time0, time1);//将排序好的Objs再次进行递归
        right = make_shared<bvh_node>(src_objects, mid, end, time0, time1);
    }
    aabb box_left, box_right;
    if (!left->bounding_box(time0, time1, box_left) || !right->bounding_box(time0, time1, box_right)) {
        std::cerr << "No bounding box in bvh_node constructor.\n";
    }
    box = surrounding_box(box_left, box_right);
}

bool bvh_node::bounding_box(double time0, double time1, aabb &output_box) const {
    output_box = box;
    return true;
}

//判断光线是否与当前结点的包围盒相交，如果相交，再判断是否与左右子树相交，直到叶子结点
bool bvh_node::hit(const ray &r, double t_min, double t_max, hit_record &rec) const {
    // 中序遍历
    //如果当前结点的包围盒没有被击中，直接返回false,避免无效的搜索
    if (!box.hit(r, t_min, t_max)) return false;
    bool hit_left = left->hit(r, t_min, t_max, rec);
    bool hit_right = right->hit(r, t_min, hit_left ? rec.t : t_max, rec);
    return hit_left || hit_right;
}
