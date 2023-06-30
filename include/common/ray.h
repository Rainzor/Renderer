#ifndef RAY_H
#define RAY_H

#include "vec3.h"

class ray {
    public:
        ray() {}
        ray(const pointf3& origin, const vecf3& direction, double time = 0.0)
            : orig(origin), dir(direction), tm(time)
        {}
        ray(const ray& r){
            orig = r.orig;
            dir = r.dir;
            tm = r.tm;
        }

        pointf3 origin() const  { return orig; }
        vecf3 direction() const { return dir; }
        double time() const     { return tm; }

        pointf3 at(double t) const {
            return orig + t*dir;
        }

    public://qaaa
        pointf3 orig;
        vecf3 dir;
        double tm;//time of ray moving
};

#endif