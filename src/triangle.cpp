#include "triangle.h"
bool triangle::hit(const ray& r, double t_min, double t_max, hit_record& rec) const {
    // 使用 Möller-Trumbore 算法计算三角形与光线的交点
    vecf3 edge1 = v1 - v0;
    vecf3 edge2 = v2 - v0;
    vecf3 h = cross(r.direction(), edge2);
    auto a = dot(edge1, h);

    // 判断光线是否与三角形平行
    if (a == 0)
        return false;

    auto f = 1.0 / a;
    vecf3 s = r.origin() - v0;
    auto u = f * dot(s, h);

    // 判断交点是否在三角形内部
    if (u < 0.0 || u > 1.0)
        return false;

    vecf3 q = cross(s, edge1);
    auto v = f * dot(r.direction(), q);

    if (v < 0.0 || u + v > 1.0)
        return false;

    //计算 t 值
    auto t = f * dot(edge2, q);

    if (t < t_min || t > t_max)
        return false;

    // 记录击中信息
    rec.t = t;
    rec.p = r.at(t);
    rec.mat_ptr = mat_ptr;
    rec.set_face_normal(r, cross(edge1, edge2));

    return true;
}

bool triangle::bounding_box(double time0, double time1, aabb& output_box) const {
    pointf3 min_point = pointf3(
            std::min(v0.x(), std::min(v1.x(), v2.x())),
            std::min(v0.y(), std::min(v1.y(), v2.y())),
            std::min(v0.z(), std::min(v1.z(), v2.z()))
    );
    pointf3 max_point = pointf3(
            std::max(v0.x(), std::max(v1.x(), v2.x())),
            std::max(v0.y(), std::max(v1.y(), v2.y())),
            std::max(v0.z(), std::max(v1.z(), v2.z()))
    );
    output_box = aabb(min_point, max_point);
    return true;
}

