#ifndef CAMERA_H
#define CAMERA_H

#include "rtweekend.h"

class camera{
    public:
        camera(
            double vfov,//垂直视角
            double aspect_ratio
        ){
            auto theta = degrees_to_radians(vfov);
            auto h = tan(theta / 2);
            auto viewport_height = 2.0 * h;
            auto viewport_width = aspect_ratio * viewport_height;
            
            auto focal_length = 1.0;

            origin = pointf3(0,0,0);
            horizontal = vecf3(viewport_width,0.0,0.0);
            vertical = vecf3(0.0, viewport_height, 0.0);
            lower_left_corner = origin - horizontal / 2 - vertical / 2 - vecf3(0, 0, focal_length);
        }

        ray get_ray(double u, double v)const{
            return ray(origin, lower_left_corner + u * horizontal + v * vertical - origin);
        }
    private:
        pointf3 origin;
        pointf3 lower_left_corner;
        vecf3 horizontal;
        vecf3 vertical;

};
#endif 