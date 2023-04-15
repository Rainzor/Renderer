#ifndef RAY_H
#define RAY_H

#include "vec3.h"

class ray {
    public:
        ray() {}
        ray(const pointf3& origin, const vecf3& direction)
            : orig(origin), dir(direction)
        {}

        pointf3 origin() const  { return orig; }
        vecf3 direction() const { return dir; }

        pointf3 at(double t) const {
            return orig + t*dir;
        }

    public:
        pointf3 orig;
        vecf3 dir;
};

#endif