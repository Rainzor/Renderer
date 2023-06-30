

#include "common.h"
#include "aabb.h"
bool aabb::hit(const ray& r, double tmin, double tmax) const{
    for(int a = 0; a < 3; a++){
        auto invD = 1.0f / r.direction()[a];
        auto t0 = (min()[a] - r.origin()[a]) * invD;
        auto t1 = (max()[a] - r.origin()[a]) * invD;
        if (invD < 0.0f)  // 处理负向光线
            std::swap(t0, t1);
        tmin = t0 > tmin ? t0 : tmin;
        tmax = t1 < tmax ? t1 : tmax;
        if(tmax <= tmin)//如果tmax小于tmin，说明光线不会与包围盒相交
            return false;
    }
    return true;
}
aabb surrounding_box(const aabb& box0,const aabb& box1){
    pointf3 small(fmin(box0.min().x(),box1.min().x()),
                  fmin(box0.min().y(),box1.min().y()),
                  fmin(box0.min().z(),box1.min().z()));
    pointf3 big(fmax(box0.max().x(),box1.max().x()),
                fmax(box0.max().y(),box1.max().y()),
                fmax(box0.max().z(),box1.max().z()));
    return aabb(small,big);
}