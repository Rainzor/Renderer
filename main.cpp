#include "rtweekend.h"

#include "color.h"
#include "hittable_list.h"
#include "sphere.h"
#include "camera.h"
#include "material.h"
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
    auto R = cos(pi / 4);
    hittable_list world;

    auto material_ground = make_shared<lambertian>(color(0.8, 0.8, 0.0));
    auto material_center = make_shared<lambertian>(color(0.1, 0.2, 0.5));
    auto material_left = make_shared<dielectric>(1.5);
    auto material_right = make_shared<metal>(color(0.8, 0.6, 0.2), 0.0);

    world.add(make_shared<sphere>(pointf3(0.0, -100.5, -1.0), 100.0, material_ground));
    world.add(make_shared<sphere>(pointf3(0.0, 0.0, -1.0), 0.5, material_center));
    world.add(make_shared<sphere>(pointf3(-1.0, 0.0, -1.0), 0.5, material_left));
    world.add(make_shared<sphere>(pointf3(-1.0, 0.0, -1.0), -0.45, material_left));
    world.add(make_shared<sphere>(pointf3(1.0, 0.0, -1.0), 0.5, material_right));


    // Camera
    pointf3 lookfrom(3, 3, 2);
    pointf3 lookat(0, 0, -1);
    vecf3 vup(0, 1, 0);
    auto dist_to_focus = (lookfrom - lookat).length();
    auto aperture = 2.0;

    camera cam(lookfrom, lookat, vup, 20, aspect_ratio, aperture, dist_to_focus);

    std::cout << "P3\n" << image_width << ' ' << image_height << "\n255\n";

    for (int j = image_height-1; j >= 0; --j) {
        std::cerr << "\rScanlines remaining: " << j << ' ' << std::flush;
        for (int i = 0; i < image_width; ++i) {
            rgbf pixel_rgbf(0,0,0);
            for(int s=0; s<sample_per_pixel;++s){
                auto u = (i + random_double()) / (image_width - 1);
                auto v = (j + random_double()) / (image_height - 1);
                ray r = cam.get_ray(u, v);
                pixel_rgbf += ray_color(r, world,max_depth);
            }
            write_color(std::cout,pixel_rgbf,sample_per_pixel);
        }
    }

    std::cerr << "\nDone.\n";
}



rgbf ray_color(const ray&r, const hittable& world) {
    struct hit_record rec;
    float p_RR=0.95;//概率反射系数
    if(world.hit(r, 0.0001, infinity, rec)) {//在0-infinity范围内找最近邻的表面
        ray scattered;
        rgbf attenuation;
        if (random_double() < p_RR && (rec.mat_ptr->scatter(r, rec, attenuation, scattered)))
            // attenuation 是吸收系数,反应处颜色的变化
            return attenuation * ray_color(scattered, world) / p_RR;  // 光线递归
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
        ray scattered;
        rgbf attenuation;
        if (rec.mat_ptr->scatter(r, rec, attenuation, scattered))//这里用上了指针的相互引用
            return attenuation * ray_color(scattered, world, depth - 1);
        return rgbf(0, 0, 0);
    }
    // 背景颜色
    vecf3 unit_direction = unit_vector(r.direction());  // 单位化方向向量
    auto t = 0.5 * (unit_direction.y() + 1.0);          // y值在-1到1之间，重定义t在0到1之间
    return (1.0 - t) * rgbf(1.0, 1.0, 1.0) + t * rgbf(0.5, 0.7, 1.0);
}
