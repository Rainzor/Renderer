#ifndef PDF_H
#define PDF_H
#include "common.h"
#include "onb.h"
#include "hittable.h"

inline vecf3 random_in_unit_sphere() {
    while (true) {
        auto p = vecf3::random(-1, 1);
        if (p.length_squared() >= 1)
            continue;
        return p;
    }
}
inline vecf3 random_in_unit_hemisphere(const vecf3 &normal) {
    vecf3 in_unit_sphere = random_in_unit_sphere();
    if (dot(in_unit_sphere, normal) > 0.0) // In the same hemisphere as the normal
        return in_unit_sphere;
    else
        return -in_unit_sphere;
}
inline vecf3 random_on_unit_sphere() {
    while (true) {
        float u = random_double() * 2 - 1;
        float v = random_double() * 2 - 1;
        float r2 = u * u + v * v;
        if (r2 >= 1)
            continue;
        return vecf3(2 * u * sqrt(1 - r2), 2 * v * sqrt(1 - r2), 1 - 2 * r2);
    }
}
inline vecf3 random_unit_vector() {
    // return unit_vector(random_in_unit_sphere());
    return random_on_unit_sphere();
}

inline vecf3 random_in_unit_disk() {
    while (true) {
        auto p = vecf3(random_double(-1, 1), random_double(-1, 1), 0);
        if (p.length_squared() >= 1)
            continue;
        return p;
    }
}

inline vecf3 random_on_unit_hemisphere() {
    while (true) {
        float u = random_double() * 2 - 1;
        float v = random_double() * 2 - 1;
        float r2 = u * u + v * v;
        if (r2 >= 1)
            continue;
        return vecf3(2 * u * sqrt(1 - r2), 2 * v * sqrt(1 - r2), abs(1 - 2 * r2));
    }
}
inline vecf3 random_on_unit_hemisphere(const vecf3 &normal) {
    // 构造在normal方向上的半球面上的随机向量

    // 1. 构造正交基，并将随机向量转换到世界坐标系下
    vecf3 local_up = abs(normal.y()) < 0.999 ? vecf3(0, 1, 0) : vecf3(1, 0, 0);
    vecf3 local_w = unit_vector(normal);
    vecf3 local_u = unit_vector(cross(local_w, local_up));
    vecf3 local_v = cross(local_w, local_u);
    vecf3 local_p = random_on_unit_hemisphere();

    // 将随机向量从局部坐标系转换到世界坐标系
    vecf3 world_p = local_p.x() * local_u + local_p.y() * local_v + local_p.z() * local_w;

    // 2. 将随机向量旋转到 normal 的方向
    if (dot(world_p, normal) < 0.0) {
        world_p = -world_p;
    }

    return world_p;
}

inline vecf3 random_cosine_direction() {
    float r1 = random_double();
    float r2 = random_double();
    float z = sqrt(1 - r2);
    float phi = 2 * pi * r1;
    float x = cos(phi) * sqrt(r2);
    float y = sin(phi) * sqrt(r2);
    return vecf3(x, y, z);
}
inline vecf3 random_to_sphere(double radius, double distance_squared) {
    auto r1 = random_double();
    auto r2 = random_double();
    auto z = 1 + r2 * (sqrt(1 - radius * radius / distance_squared) - 1);

    auto phi = 2 * pi * r1;
    auto x = cos(phi) * sqrt(1 - z * z);
    auto y = sin(phi) * sqrt(1 - z * z);

    return vecf3(x, y, z);
}

class pdf {
public:
    virtual ~pdf() {
    }
    //根据出射方向计算pdf值
    virtual double value(const vecf3 &direction) const = 0;
    //返回采样光线方向
    virtual vecf3 generate() const = 0;
};
class cosin_pdf : public pdf {
public:
    cosin_pdf(const vecf3 &w) {
        uvw.build_from_w(w);
    }

    virtual double value(const vecf3 &direction) const override {
        auto cosine = dot(uvw.w(),unit_vector(direction));
        return (cosine <= 0) ? 0 : cosine / pi;
    }

    virtual vecf3 generate() const override {
        return uvw.local(random_cosine_direction());
    }
    public:
        onb uvw;
};
class uniform_pdf:public pdf{
public:
    uniform_pdf(){}
    virtual double value(const vecf3 &direction) const override {
        return 1/(4*pi);
    }
    virtual vecf3 generate() const override {
        return random_on_unit_sphere();
    }
};

class hittable_pdf : public pdf {
public:
    hittable_pdf(shared_ptr<hittable> p, const pointf3 &origin) : ptr(p), o(origin) {
    }
    //输入光线起点o和方向v，计算与物体相交点的pdf值
    virtual double value(const vecf3 &direction) const override {
        return ptr->pdf_value(o, direction);
    }
    //输入光线起点o，返回从物体表面采样的点与光线起点连接的出射方向
    virtual vecf3 generate() const override {
        return ptr->random(o);
    }

public:
    shared_ptr<hittable> ptr;
    pointf3 o; // 光线起点
};

class light_pdf : public pdf {
public:
    light_pdf(shared_ptr<hittable> p, const pointf3 &origin) : light_ptr(p), o(origin) {
    }
    //输入光线起点o和方向v，计算与物体相交点的pdf值
    virtual double value(const vecf3 &direction) const override {
        return light_ptr->pdf_value(o, direction);
    }
    //输入光线起点o，返回从物体表面采样的点与光线起点连接的出射方向
    virtual vecf3 generate() const override {
        return light_ptr->random(o);
    }
public:
    shared_ptr<hittable> light_ptr;
    pointf3 o; // 光线起点
};


class mixture_pdf : public pdf {
public:
    mixture_pdf(shared_ptr<pdf> p0, shared_ptr<pdf> p1) {
        p[0] = p0;
        p[1] = p1;
    }
    virtual double value(const vecf3 &direction) const override {
        return 0.5 * p[0]->value(direction) + 0.5 * p[1]->value(direction);
    }
    virtual vecf3 generate() const override {
        if (random_double() < 0.5)
            return p[0]->generate();
        else
            return p[1]->generate();
    }
public:
    shared_ptr<pdf> p[2];
};





#endif