#include "rtweekend.h"

#include "color.h"
#include "hittable_list.h"
#include "sphere.h"
#include "camera.h"
#include "material.h"
#include <iostream>
#include <omp.h>    // OpenMP

#define NUM_THREADS 16

rgbf ray_color(const ray& r, const hittable& world);
rgbf ray_color(const ray&r, const hittable&world, int depth);

hittable_list random_scene() {
    hittable_list world;

    auto ground_material = make_shared<lambertian>(color(0.5, 0.5, 0.5));
    world.add(make_shared<sphere>(pointf3(0, -1000, 0), 1000, ground_material));

    for (int a = -11; a < 11; a++) {
        for (int b = -11; b < 11; b++) {
            auto choose_mat = random_double();
            pointf3 center(a + 0.9 * random_double(), 0.2, b + 0.9 * random_double());

            if ((center - pointf3(4, 0.2, 0)).length() > 0.9) {
                shared_ptr<material> sphere_material;

                if (choose_mat < 0.8) {
                    // diffuse
                    auto albedo = color::random() * color::random();
                    sphere_material = make_shared<lambertian>(albedo);
                    world.add(make_shared<sphere>(center, 0.2, sphere_material));
                } else if (choose_mat < 0.95) {
                    // metal
                    auto albedo = color::random(0.5, 1);
                    auto fuzz = random_double(0, 0.5);
                    sphere_material = make_shared<metal>(albedo, fuzz);
                    world.add(make_shared<sphere>(center, 0.2, sphere_material));
                } else {
                    // glass
                    sphere_material = make_shared<dielectric>(1.5);
                    world.add(make_shared<sphere>(center, 0.2, sphere_material));
                }
            }
        }
    }

    auto material1 = make_shared<dielectric>(1.5);
    world.add(make_shared<sphere>(pointf3(0, 1, 0), 1.0, material1));

    auto material2 = make_shared<lambertian>(color(0.4, 0.2, 0.1));
    world.add(make_shared<sphere>(pointf3(-4, 1, 0), 1.0, material2));

    auto material3 = make_shared<metal>(color(0.7, 0.6, 0.5), 0.0);
    world.add(make_shared<sphere>(pointf3(4, 1, 0), 1.0, material3));

    return world;
}
int main() {

    // Parallel
    omp_set_num_threads(NUM_THREADS);  // 设置线程

    // Image
    const auto aspect_ratio = 3.0 / 2.0;
    const int image_width = 1200;
    const int image_height = static_cast<int>(image_width / aspect_ratio);
    const int samples_per_pixel = 500;
    const int max_depth = 50;
    // World
    auto world = random_scene();
    // Camera
    pointf3 lookfrom(13, 2, 3);
    pointf3 lookat(0, 0, 0);
    vecf3 vup(0, 1, 0);
    auto dist_to_focus = 10.0;
    auto aperture = 0.1;

    camera cam(lookfrom, lookat, vup, 20, aspect_ratio, aperture, dist_to_focus);

    std::cout << "P3\n" << image_width << ' ' << image_height << "\n255\n";

    int i,j,k,s;
    double u,v;
    ray r;
#pragma omp parallel for// OpenMP
    for(k=0;k<image_height*image_width;k++){
        j=k/image_width;
        i=k%image_width;
        if(i==0)
            std::cerr << "\rScanlines remaining: " << j << ' ' << std::flush;
        rgbf pixel_rgbf(0,0,0);
        for (s = 0; s < samples_per_pixel; ++s) {
            u = (i + random_double()) / (image_width - 1);
            v = (j + random_double()) / (image_height - 1);
            r = cam.get_ray(u, v);
            pixel_rgbf += ray_color(r, world,max_depth);
        }
        write_color(std::cout, pixel_rgbf, samples_per_pixel);
    }

    // for (j = image_height-1; j >= 0; --j) {
    //     std::cerr << "\rScanlines remaining: " << j << ' ' << std::flush;
    //     for (i = 0; i < image_width; ++i) {
    //         rgbf pixel_rgbf(0,0,0);
    //         for (int s = 0; s < samples_per_pixel; ++s) {
    //             u = (i + random_double()) / (image_width - 1);
    //             v = (j + random_double()) / (image_height - 1);
    //             r = cam.get_ray(u, v);
    //             pixel_rgbf += ray_color(r, world,max_depth);
    //         }
    //         write_color(std::cout, pixel_rgbf, samples_per_pixel);
    //     }
    // }

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
