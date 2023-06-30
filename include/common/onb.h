#ifndef ONB_H
#define ONB_H

#include "common.h"

class onb {
public:
    onb() {}

    inline vecf3 operator[](int i) const { return axis[i]; }

    vecf3 u() const { return axis[0]; }

    vecf3 v() const { return axis[1]; }

    vecf3 w() const { return axis[2]; }

    vecf3 local(double a, double b, double c) const {
        return a * u() + b * v() + c * w();
    }

    vecf3 local(const vecf3 &a) const {
        return a.x() * u() + a.y() * v() + a.z() * w();
    }

    void build_from_w(const vecf3 & n) {
        axis[2] = unit_vector(n);
        vecf3 a = (fabs(w().x()) > 0.9) ? vecf3(0, 1, 0) : vecf3(1, 0, 0);
        axis[1] = unit_vector(cross(w(), a));
        axis[0] = cross(w(), v());
    }

public:
    vecf3 axis[3];
};
#endif