#include "rtweekend.h"

#include "color.h"
#include "hittable_list.h"
#include "sphere.h"
#include "camera.h"
#include <iostream>

rgbf ray_color(const ray& r, const hittable& world);
rgbf ray_color(const ray&r, const hittable&world, int depth);
int main() {

    // Image
    const auto aspect_ratio = 16.0f / 9.0f;//长宽比
    const int image_width = 400;
    const int image_height = static_cast<int>(image_width / aspect_ratio);
    const int sample_per_pixel = 100;
    const int max_depth = 50;
    // World
    hittable_list world;
    world.add(make_shared<sphere>(pointf3(0, 0, -1), 0.5)); //通过make_shared创建共享指针
    world.add(make_shared<sphere>(pointf3(0, -100.5, -1), 100));


    // Camera
    camera cam;
 
    // Render

    std::cout << "P3\n" << image_width << ' ' << image_height << "\n255\n";

    for (int j = image_height-1; j >= 0; --j) {
        std::cerr << "\rScanlines remaining: " << j << ' ' << std::flush;
        for (int i = 0; i < image_width; ++i) {
            rgbf pixel_color(0,0,0);
            for(int s=0; s<sample_per_pixel;++s){
                auto u = (i + random_double()) / (image_width - 1);
                auto v = (j + random_double()) / (image_height - 1);
                ray r = cam.get_ray(u, v);
                pixel_color += ray_color(r, world);
            }
            write_color(std::cout,pixel_color,sample_per_pixel);
        }
    }

    std::cerr << "\nDone.\n";
}



rgbf ray_color(const ray&r, const hittable& world) {
    struct hit_record rec;
    float p_RR=0.8;//概率反射系数
    if(world.hit(r, 0.0001, infinity, rec)) {//在0-infinity范围内找最近邻的表面
        pointf3 direc = random_in_unit_sphere() + rec.normal;
        if(random_double()<p_RR)
            //0.5 是吸收系数
            return 0.5 * ray_color(ray(rec.p, direc), world) / p_RR;  // 光线递归
        else
            return rgbf(0,0,0);
    }
    // 背景颜色
    vecf3 unit_direction = unit_vector(r.direction());  // 单位化方向向量
    auto t = 0.5 * (unit_direction.y() + 1.0);          // y值在-1到1之间，重定义t在0到1之间
    return (1.0 - t) * rgbf(1.0, 1.0, 1.0) + t * rgbf(0.5, 0.7, 1.0);
}


rgbf ray_color(const ray&r, const hittable&world, int depth){
    struct hit_record rec;
    if(depth<=0)
        return rgbf(0,0,0);
    if (world.hit(r, 0.0001, infinity, rec)) {  // 在0-infinity范围内找最近邻的表面
        pointf3 target = rec.p + random_in_unit_hemisphere(rec.normal);
        return 0.5 * ray_color(ray(rec.p, target - rec.p), world, depth - 1);  // 光线递归
    }
    // 背景颜色
    vecf3 unit_direction = unit_vector(r.direction());  // 单位化方向向量
    auto t = 0.5 * (unit_direction.y() + 1.0);          // y值在-1到1之间，重定义t在0到1之间
    return (1.0 - t) * rgbf(1.0, 1.0, 1.0) + t * rgbf(0.5, 0.7, 1.0);
}
