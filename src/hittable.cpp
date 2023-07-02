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
rotate::rotate(shared_ptr<hittable> p, double angle, Axis axis)
        : ptr(p), axis(axis) {
    auto radians = degrees_to_radians(angle);
    sin_theta = sin(radians);
    cos_theta = cos(radians);
    hasbox = ptr->bounding_box(0, 1, bbox);

    pointf3 min(infinity, infinity, infinity);
    pointf3 max(-infinity, -infinity, -infinity);

    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < 2; j++) {
            for (int k = 0; k < 2; k++) {
                vecf3 tester(
                        i * bbox.max().x() + (1 - i) * bbox.min().x(),
                        j * bbox.max().y() + (1 - j) * bbox.min().y(),
                        k * bbox.max().z() + (1 - k) * bbox.min().z());

                for (size_t c = 0; c < 3; ++c) {
                    if (c == static_cast<size_t>(axis)) {
                        auto c1 = (c + 1) % 3;
                        auto c2 = (c + 2) % 3;
                        float tmp1 = tester[c1];
                        float tmp2 = tester[c2];
                        tester[c1] = cos_theta * tmp1 - sin_theta * tmp2;
                        tester[c2] = sin_theta * tmp1 + cos_theta * tmp2;
                    }
                }

                for (int c = 0; c < 3; c++) {
                    min[c] = fmin(min[c], tester[c]);
                    max[c] = fmax(max[c], tester[c]);
                }
            }
        }
    }

    bbox = aabb(min, max);
}

bool rotate::hit(const ray& r, double t_min, double t_max, hit_record& rec) const {
    auto origin = r.origin();
    auto direction = r.direction();

    for (size_t c = 0; c < 3; ++c) {
        if (c == static_cast<size_t>(axis)) {
            auto c1 = (c + 1) % 3;
            auto c2 = (c + 2) % 3;
            origin[c1] = cos_theta * r.origin()[c1] + sin_theta * r.origin()[c2];
            origin[c2] = -sin_theta * r.origin()[c1] + cos_theta * r.origin()[c2];

            direction[c1] = cos_theta * r.direction()[c1] + sin_theta * r.direction()[c2];
            direction[c2] = -sin_theta * r.direction()[c1] + cos_theta * r.direction()[c2];
        }
    }

    ray rotated_r(origin, direction, r.time());

    if (!ptr->hit(rotated_r, t_min, t_max, rec))
        return false;

    auto p = rec.p;
    auto normal = rec.normal;

    for (size_t c = 0; c < 3; ++c) {
        if (c == static_cast<size_t>(axis)) {
            auto c1 = (c + 1) % 3;
            auto c2 = (c + 2) % 3;
            p[c1] = cos_theta * rec.p[c1] - sin_theta * rec.p[c2];
            p[c2] = sin_theta * rec.p[c1] + cos_theta * rec.p[c2];

            normal[c1] = cos_theta * rec.normal[c1] - sin_theta * rec.normal[c2];
            normal[c2] = sin_theta * rec.normal[c1] + cos_theta * rec.normal[c2];
        }
    }

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