#ifndef PDF_H
#define PDF_H
#include "rtweekend.h"
#include "onb.h"
#include "hittable.h"
class pdf {
public:
    virtual ~pdf() {
    }

    virtual double value(const vecf3 &direction) const = 0;
    virtual vecf3 generate() const = 0;
};
class cosin_pdf : public pdf {
public:
    cosin_pdf(const vecf3 &w) {
        uvw.build_from_w(w);
    }

    virtual double value(const vecf3 &direction) const override {
        auto cosine = dot(unit_vector(direction), uvw.w());
        return (cosine <= 0) ? 0 : cosine / pi;
    }

    virtual vecf3 generate() const override {
        return uvw.local(random_cosine_direction());
    }
    public:
        onb uvw;
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