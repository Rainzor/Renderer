#include "hittable.h"

//平移
bool translate::hit(const ray &r, double t_min, double t_max, hit_record &rec) const {
    //物体的平移相当于光线的反方向平移
    ray moved_r(r.origin() - offset, r.direction(), r.time());
    if (!ptr->hit(moved_r, t_min, t_max, rec))
        return false;
    rec.p += offset;
    rec.set_face_normal(moved_r, rec.normal);

    return true;
}

bool translate::bounding_box(double time0, double time1, aabb &output_box) const {
    //调用时才计算包围盒
    if (!ptr->bounding_box(time0, time1, output_box))
        return false;
    //包围盒平移
    output_box = aabb(
            output_box.min() + offset,
            output_box.max() + offset);

    return true;
}

//旋转
rotate_y::rotate_y(shared_ptr<hittable> p, double angle) : ptr(p) {
    //初始化时修正包围盒
    auto radians = degrees_to_radians(angle);
    sin_theta = sin(radians);
    cos_theta = cos(radians);
    hasbox = ptr->bounding_box(0, 1, bbox);

    pointf3 min(infinity, infinity, infinity);
    pointf3 max(-infinity, -infinity, -infinity);

    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < 2; j++) {
            for (int k = 0; k < 2; k++) {
                auto x = i * bbox.max().x() + (1 - i) * bbox.min().x();
                auto y = j * bbox.max().y() + (1 - j) * bbox.min().y();
                auto z = k * bbox.max().z() + (1 - k) * bbox.min().z();

                auto newx = cos_theta * x + sin_theta * z;
                auto newz = -sin_theta * x + cos_theta * z;

                vecf3 tester(newx, y, newz);

                for (int c = 0; c < 3; c++) {
                    min[c] = fmin(min[c], tester[c]);
                    max[c] = fmax(max[c], tester[c]);
                }
            }
        }
    }

    bbox = aabb(min, max);
}

bool rotate_y::hit(const ray &r, double t_min, double t_max, hit_record &rec) const {
    auto origin = r.origin();
    auto direction = r.direction();
    //光线的反向旋转
    origin[0] = cos_theta * r.origin()[0] - sin_theta * r.origin()[2];
    origin[2] = sin_theta * r.origin()[0] + cos_theta * r.origin()[2];

    direction[0] = cos_theta * r.direction()[0] - sin_theta * r.direction()[2];
    direction[2] = sin_theta * r.direction()[0] + cos_theta * r.direction()[2];

    ray rotated_r(origin, direction, r.time());

    if (!ptr->hit(rotated_r, t_min, t_max, rec))
        return false;

    auto p = rec.p;
    auto normal = rec.normal;
    //交点的旋转
    p[0] = cos_theta * rec.p[0] + sin_theta * rec.p[2];
    p[2] = -sin_theta * rec.p[0] + cos_theta * rec.p[2];
    //法线的旋转
    normal[0] = cos_theta * rec.normal[0] + sin_theta * rec.normal[2];
    normal[2] = -sin_theta * rec.normal[0] + cos_theta * rec.normal[2];
    //修改hit_record
    rec.p = p;
    rec.set_face_normal(rotated_r, normal);

    return true;
}

//翻转：改变材料的内外朝向，最直接的应用是对于无限大平面，可以通过翻转法向量来改变平面的朝向
bool flip_face::hit(
        const ray &r,
        double t_min,
        double t_max,
        hit_record &rec) const {
//交点的法线反向
    if (!ptr->hit(r, t_min, t_max, rec))
        return false;

    rec.front_face = !rec.front_face;//交点的法线反向
    return true;
}