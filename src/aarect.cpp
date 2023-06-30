#include "aarect.h"


bool xy_rect::hit(const ray& r, double t_min, double t_max, hit_record& rec) const {
    auto t = (k - r.origin().z()) / r.direction().z();//根据纵坐标计算z
    if (t < t_min || t > t_max || t != t)
        return false;
    auto x = r.origin().x() + t * r.direction().x();
    auto y = r.origin().y() + t * r.direction().y();
    if (x < x0 || x > x1 || y < y0 || y > y1)
        return false;
    rec.u = (x - x0) / (x1 - x0);
    rec.v = (y - y0) / (y1 - y0);
    rec.t = t;

    auto outward_normal = vecf3(0, 0, 1);  // 法线
    // 修正朝向，使得与入射光线方向相反
    // 修改rec的 front_face判定，判断该面是否朝向光线
    rec.set_face_normal(r, outward_normal);
    rec.mat_ptr = mp;
    rec.p = r.at(t);
    return true;
}

bool xy_rect::bounding_box(double time0, double time1, aabb& output_box) const {
    // The bounding box must have non-zero width in each dimension, so pad the Z
    // dimension a small amount.
    output_box = aabb(pointf3(x0, y0, k - 0.0001), pointf3(x1, y1, k + 0.0001));
    return true;
}

double xy_rect::pdf_value(const pointf3& o, const vecf3& v) const {
    hit_record rec;
    if (!this->hit(ray(o, v), 0.001, infinity, rec))
        return 0;

    auto area = (x1 - x0) * (y1 - y0);
    auto distance_squared = rec.t * rec.t * v.length_squared();
    auto cosine = fabs(dot(v, rec.normal) / v.length());

    return distance_squared / (cosine * area);
}

vecf3 xy_rect::random(const vecf3& o) const {
    auto random_point = pointf3(random_double(x0, x1), random_double(y0, y1), k);
    return random_point - o;
}

bool xz_rect::hit(const ray& r, double t_min, double t_max, hit_record& rec) const {
    auto t = (k - r.origin().y()) / r.direction().y();
    if (t < t_min || t > t_max|| t != t)
        return false;
    auto x = r.origin().x() + t * r.direction().x();
    auto z = r.origin().z() + t * r.direction().z();
    if (x < x0 || x > x1 || z < z0 || z > z1)
        return false;
    rec.u = (x - x0) / (x1 - x0);
    rec.v = (z - z0) / (z1 - z0);
    rec.t = t;
    auto outward_normal = vecf3(0, 1, 0);
    rec.set_face_normal(r, outward_normal);
    rec.mat_ptr = mp;
    rec.p = r.at(t);
    return true;
}

bool xz_rect::bounding_box(double time0, double time1, aabb& output_box) const {
    // The bounding box must have non-zero width in each dimension, so pad the Y
    // dimension a small amount.
    output_box = aabb(pointf3(x0, k - 0.0001, z0), pointf3(x1, k + 0.0001, z1));
    return true;
}

double xz_rect::pdf_value(const pointf3& o, const vecf3& v) const {
    hit_record rec;
    if (!this->hit(ray(o, v), 0.001, infinity, rec))
        return 0;

    auto area = (x1 - x0) * (z1 - z0);
    auto distance_squared = rec.t * rec.t * v.length_squared();
    auto cosine = fabs(dot(v, rec.normal) / v.length());

    return distance_squared / (cosine * area);
}

vecf3 xz_rect::random(const vecf3& o)const {
    auto random_point = pointf3(random_double(x0, x1), k, random_double(z0, z1));
    return random_point - o;
}


bool yz_rect::hit(const ray& r, double t_min, double t_max, hit_record& rec) const {
    auto t = (k - r.origin().x()) / r.direction().x();
    if (t < t_min || t > t_max|| t != t)
        return false;
    auto y = r.origin().y() + t * r.direction().y();
    auto z = r.origin().z() + t * r.direction().z();
    if (y < y0 || y > y1 || z < z0 || z > z1)
        return false;
    rec.u = (y - y0) / (y1 - y0);
    rec.v = (z - z0) / (z1 - z0);
    rec.t = t;
    auto outward_normal = vecf3(1, 0, 0);
    rec.set_face_normal(r, outward_normal);
    rec.mat_ptr = mp;
    rec.p = r.at(t);
    return true;
}

bool yz_rect::bounding_box(double time0, double time1, aabb& output_box) const {
    // The bounding box must have non-zero width in each dimension, so pad the X
    // dimension a small amount.
    output_box = aabb(pointf3(k - 0.0001, y0, z0), pointf3(k + 0.0001, y1, z1));
    return true;
}

double yz_rect::pdf_value(const pointf3& o, const vecf3& v) const {
    hit_record rec;
    //寻找交点
    if (!this->hit(ray(o, v), 0.001, infinity, rec))
        return 0;

    auto area = (y1 - y0) * (z1 - z0);
    auto distance_squared = rec.t * rec.t * v.length_squared();
    auto cosine = fabs(dot(v, rec.normal) / v.length());

    return distance_squared / (cosine * area);
}

vecf3 yz_rect::random(const vecf3& o) const {
    auto random_point = pointf3(k, random_double(y0, y1), random_double(z0, z1));
    return random_point - o;
}
