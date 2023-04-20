#ifndef CAMERA_H
#define CAMERA_H

#include "rtweekend.h"

class camera{
    public:
     camera(
         pointf3 lookfrom,
         pointf3 lookat,
         vecf3 vup,    // 垂直向量,view up：the rotation around the lookat-lookfrom axis.
         double vfov,  // 垂直视角
         double aspect_ratio) {
            auto theta = degrees_to_radians(vfov);
            auto h = tan(theta / 2);
            auto viewport_height = 2.0 * h;
            auto viewport_width = aspect_ratio * viewport_height;

            auto w = unit_vector(lookfrom - lookat);
            auto u = unit_vector(cross(vup, w));
            auto v = cross(w, u);

            origin = lookfrom;
            horizontal = viewport_width * u;
            vertical = viewport_height * v;
            lower_left_corner = origin - horizontal / 2 - vertical / 2 - w;
        }

        ray get_ray(double s, double t)const{
            return ray(origin, lower_left_corner + s* horizontal + t * vertical - origin);
        }
    private:
        pointf3 origin;
        pointf3 lower_left_corner;
        vecf3 horizontal;
        vecf3 vertical;

};
#endif 