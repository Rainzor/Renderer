#ifndef AABB_H
#define AABB_H

#include "common.h"

class aabb{
    public:
        aabb()= default;
        aabb(const pointf3& a, const pointf3& b):minimum(a),maximum(b){}
        pointf3 min() const {return minimum;}
        pointf3 max() const {return maximum;}

        //aabb hit:判断光线是否与包围盒相交
        bool hit(const ray& r, double tmin, double tmax) const;
    public:
    //box可以看成是两个点的集合，一个最小点，一个最大点，描述了一个长方体
        pointf3 minimum;
        pointf3 maximum;
};

aabb surrounding_box(const aabb& box0,const aabb& box1);

#endif