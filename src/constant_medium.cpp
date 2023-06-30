#include "constant_medium.h"

bool constant_medium::hit(const ray& r, double t_min, double t_max, hit_record& rec) const {
    // Print occasional samples when debugging. To enable, set enableDebug true.
    const bool enableDebug = false;
    const bool debugging = enableDebug && random_double() < 0.00001;

    hit_record rec1, rec2;

    //  判断光线前后直线上是否能穿过边界
    //  如果穿过了物体，有三种情况：
    //  光源在物体内部，从物体内部穿出，则交点rec1.t在光线反方向上
    //  光源在物体外部，交点rec1.t在光线正方向,则是靠近光源的交点
    //  光源在物体外部，交点rec1.t在光线反方向,则是远离光源的交点
    if (!boundary->hit(r, -infinity, infinity, rec1))
        return false;

    //  判断从交点位置出发的光线向前是否能穿过物体，避免相切的情况
    //  如果与物体相交，则交点t有三种情况：
    //  光源在物体内部，从物体内部穿出，则交点rec2.t在光线正方向上  rec1.t -> origin -> rec2.t
    //  光源在物体外部，交点rec2.t在光线正方向，且是远离光源的交点  origin -> rec1.t -> boundary -> rec2.t
    //  光源在物体外部，交点rec2.t在光线反方向，且是靠近光源的交点  rec1.t -> boundary -> rec2.t -> origin
    if (!boundary->hit(r, rec1.t + 0.0001, infinity, rec2))
        return false;

    if (debugging)
        std::cerr << "\nt_min=" << rec1.t << ", t_max=" << rec2.t << '\n';

    if (rec1.t < t_min|| isnan(rec1.t))
        rec1.t = t_min;
    if (rec2.t > t_max|| isnan(rec2.t))
        rec2.t = t_max;

    if (rec1.t >= rec2.t)//如果是在光源反向，则return false
        return false;

    if (rec1.t < 0)//光线方向不能为负数
        rec1.t = 0;

    const auto ray_length = r.direction().length();
    // distance_inside_boundary: 如果光源在内部，则是光源正向到边界的距离；如果光源在外部，则是光线穿过物体的距离
    const auto distance_inside_boundary = (rec2.t - rec1.t) * ray_length;
    const auto hit_distance = neg_inv_density * log(random_double());//距离的采样遵循指数分布

    // 如果采样的距离大于光线穿过物体的距离，则return false
    // 这时可以认为光线已经穿出物体了，在hittable_list中，光线会继续向前寻找下一个物体
    if (hit_distance > distance_inside_boundary)
        return false;

    //更新交点信息
    rec.t = rec1.t + hit_distance / ray_length;
    rec.p = r.at(rec.t);

    if (debugging) {
        std::cerr << "hit_distance = " << hit_distance << '\n'
                  << "rec.t = " << rec.t << '\n'
                  << "rec.p = " << rec.p << '\n';
    }

    rec.normal = vecf3(1, 0, 0);  // arbitrary
    rec.front_face = true;       // also arbitrary
    rec.mat_ptr = phase_function;
    rec.boundary_ptr = boundary;
    rec.density = -1/neg_inv_density;
    return true;
}