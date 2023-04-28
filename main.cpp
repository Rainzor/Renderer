#include "rtweekend.h"

#include "color.h"
#include "hittable_list.h"
#include "sphere.h"
#include "camera.h"
#include "material.h"
#include "moving_sphere.h"
#include "bvh.h"
#include <iostream>
#include <omp.h>    // OpenMP
#include <chrono>
#include <vector>

#define NUM_THREADS 8

rgbf ray_color(const ray& r, const hittable& world);
rgbf ray_color(const ray&r, const hittable&world, int depth);
hittable_list random_scene();
hittable_list two_spheres();
hittable_list two_perlin_spheres();
bvh_node random_bvh_scene();
int main() {
    using namespace std::chrono;
    // Parallel
    omp_set_num_threads(NUM_THREADS);  // 设置线程

    // Image
    const auto aspect_ratio = 3.0 / 2.0;
    const int image_width = 400;
    const int image_height = static_cast<int>(image_width / aspect_ratio);
    const int samples_per_pixel = 100;
    const int max_depth = 50;

    std::vector<std::vector<rgbf>> image(image_height,std::vector<rgbf>(image_width,rgbf(0,0,0)));

    // World
    hittable_list world;
    bvh_node bvh_world;

    pointf3 lookfrom;
    pointf3 lookat;
    auto vfov = 40.0;
    auto aperture = 0.0;

    switch (0) {//这种用法是为了方便调试，可以直接切换场景
        case 1:
            world = random_scene();
            //bvh_world = random_bvh_scene();
            lookfrom = pointf3(13, 2, 3);
            lookat = pointf3(0, 0, 0);
            vfov = 20.0;
            aperture = 0.1;
            break;
        case 2:
            world = two_spheres();
            lookfrom = pointf3(13, 2, 3);
            lookat = pointf3(0, 0, 0);
            vfov = 20.0;
            break;
        default:
        case 3:
            world = two_perlin_spheres();
            lookfrom = pointf3(13, 2, 3);
            lookat = pointf3(0, 0, 0);
            vfov = 20.0;
            break;
    }
    // Camera
    vecf3 vup(0, 1, 0);
    auto dist_to_focus = 10.0;

    camera cam(lookfrom, lookat, vup, vfov, aspect_ratio, aperture, dist_to_focus,0.0,1.0);

    std::cout << "P3\n" << image_width << ' ' << image_height << "\n255\n";

    int i,j,k,s;
    double u,v;
    ray r;
    rgbf pixel_color,image_color;
    auto start = high_resolution_clock::now();
    int sum=0;
#pragma omp parallel for private(k,i,j,s,u,v,r,pixel_color) shared(image,sum)  // OpenMP
    for(k=0;k<image_height*image_width;k++){
        j=k/image_width;
        i=k%image_width;
        pixel_color = rgbf(0, 0, 0);
        for (s = 0, image_color = rgbf(0, 0, 0); s < samples_per_pixel; s += 1) {
            u = (i + random_double()) / (image_width - 1);
            v = (j + random_double()) / (image_height - 1);
            r = cam.get_ray(u, v);
            pixel_color += ray_color(r, world, max_depth);
        }
        image[j][i]=pixel_color;
#pragma omp critical 
        {
            if (i == 0){
                sum++;
                std::cerr << "\rScanlines remaining: " << image_height - sum << ' ' << std::flush;
            }
        }
    }
    for(j=image_height-1;j>=0;j--){
        for(i=0;i<image_width;i++){
            write_color(std::cout, image[j][i], samples_per_pixel);
        }
    }

    auto end = high_resolution_clock::now();
    auto duration = duration_cast<milliseconds>(end - start);
    std::cerr << std::endl
              << "Time Cost:" << duration.count() << " ms" << std::endl;
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

hittable_list random_scene(){
    {
        hittable_list world;

        auto checker = make_shared<checker_texture>(color(0.2, 0.3, 0.1), color(0.9, 0.9, 0.9));
        world.add(make_shared<sphere>(pointf3(0, -1000, 0), 1000, make_shared<lambertian>(checker)));
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
                        auto center2 = center + vecf3(0, random_double(0, .5), 0);
                        world.add(make_shared<moving_sphere>(
                            center, center2, 0.0, 1.0, 0.2, sphere_material));
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


        hittable_list objs;
        objs.add(make_shared<bvh_node>(world, 0, 1));

        return objs;
    }
}

hittable_list two_spheres() {
    hittable_list objects;

    auto checker = make_shared<checker_texture>(color(0.2, 0.3, 0.1), color(0.9, 0.9, 0.9));

    objects.add(make_shared<sphere>(pointf3(0, -10, 0), 10, make_shared<lambertian>(checker)));
    objects.add(make_shared<sphere>(pointf3(0, 10, 0), 10, make_shared<lambertian>(checker)));

    return objects;
}

hittable_list two_perlin_spheres(){
    hittable_list objects;

    auto pertext = make_shared<noise_texture>(4);
    objects.add(make_shared<sphere>(pointf3(0,-1000,0),1000,make_shared<lambertian>(pertext)));
    objects.add(make_shared<sphere>(pointf3(0,2,0),2,make_shared<lambertian>(pertext)));

    return objects;
}
bvh_node random_bvh_scene() {
    {
        hittable_list world;

        auto checker = make_shared<checker_texture>(color(0.2, 0.3, 0.1), color(0.9, 0.9, 0.9));
        world.add(make_shared<sphere>(pointf3(0, -1000, 0), 1000, make_shared<lambertian>(checker)));
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
                        auto center2 = center + vecf3(0, random_double(0, .5), 0);
                        world.add(make_shared<moving_sphere>(
                            center, center2, 0.0, 1.0, 0.2, sphere_material));
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

        bvh_node objs(world, 0, 1);
        return objs;
    }
}