#ifndef CAMERA_H
#define CAMERA_H

#include "common.h"
#include "pdf.h"
class camera{
    public:
     camera(
         pointf3 lookfrom,
         pointf3 lookat,
         vecf3 vup,    // 垂直向量,view up：the rotation around the lookat-lookfrom axis.
         double vfov,  // 垂直视角
         double aspect_ratio,
         double aperture,// 光圈:the size of the lens
         double focus_dist,//焦距
         double _time0 = 0,
         double _time1 = 0
         ) {
            auto theta = degrees_to_radians(vfov);
            auto h = tan(theta / 2);
            auto viewport_height = 2.0 * h;
            auto viewport_width = aspect_ratio * viewport_height;

            w = unit_vector(lookfrom - lookat);
            u = unit_vector(cross(vup, w));
            v = cross(w, u);

            origin = lookfrom;
            horizontal = focus_dist*viewport_width * u;
            vertical = focus_dist* viewport_height * v;
            lower_left_corner = origin - horizontal / 2 - vertical / 2 - focus_dist* w;

            lens_radius = aperture / 2;
            time0 = _time0;
            time1 = _time1;
        }

        ray get_ray(double s, double t)const{
            vecf3 rd = lens_radius * random_in_unit_disk();
            vecf3 offset = u * rd.x() + v * rd.y();
            //返回从相机出发的光线，光线射出的时间是0-1内随机的
            return ray( origin+offset, 
                        lower_left_corner + s* horizontal + t * vertical - origin - offset,random_double(time0, time1));
        }
    private:
        pointf3 origin;
        pointf3 lower_left_corner;
        vecf3 horizontal;
        vecf3 vertical;
        vecf3 u, v, w;
        double lens_radius;
        double time0, time1;// shutter open/close times
};
#endif 