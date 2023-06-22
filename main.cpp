#include <chrono>
#include <iostream>
#include <omp.h> // OpenMP
#include <vector>
#include "rtweekend.h"
#include "aarect.h"
#include "box.h"
#include "bvh.h"
#include "camera.h"
#include "color.h"
#include "constant_medium.h"
#include "hittable_list.h"
#include "material.h"
#include "moving_sphere.h"
#include "sphere.h"

#define NUM_THREADS 8 // 线程数

color ray_color(const ray &r, const color &background, const hittable &world);
color ray_color(const ray &r, const color &background, const hittable &world, int depth);
hittable_list random_scene();
hittable_list two_spheres();
hittable_list two_perlin_spheres();
bvh_node world2bvh_scene(hittable_list &world);
hittable_list earth();
hittable_list simple_light();
hittable_list cornell_box();
hittable_list cornell_smoke();
hittable_list final_scene();
int main() {
    using namespace std::chrono;
    // Parallel
    omp_set_num_threads(NUM_THREADS); // 设置线程

    // Image
    auto aspect_ratio = 3.0 / 2.0;
    int image_width = 400;
    int max_depth = 50;
    int samples_per_pixel = 100;
    // World
    hittable_list world;
    bvh_node bvh_world;

    pointf3 lookfrom;
    pointf3 lookat;
    auto vfov = 40.0;    // 视野
    auto aperture = 0.0; // 光圈
    color background(0, 0, 0);
    switch (6) {         // 这种用法是为了方便调试，可以直接切换场景
    case 1:
        world = random_scene();
        // bvh_world = random_bvh_scene();
        background = color(0.70, 0.80, 1.00);
        lookfrom = pointf3(13, 2, 3);
        lookat = pointf3(0, 0, 0);
        vfov = 20.0;
        aperture = 0.1;
        break;
    case 2:
        world = two_spheres();
        background = color(0.70, 0.80, 1.00);
        lookfrom = pointf3(13, 2, 3);
        lookat = pointf3(0, 0, 0);
        vfov = 20.0;
        break;
    case 3:
        world = two_perlin_spheres();
        background = color(0.70, 0.80, 1.00);
        lookfrom = pointf3(13, 2, 3);
        lookat = pointf3(0, 0, 0);
        vfov = 20.0;
        break;
    case 4:
        world = earth();
        background = color(0.70, 0.80, 1.00);
        lookfrom = pointf3(13, 2, 3);
        lookat = pointf3(0, 0, 0);
        vfov = 20.0;
        break;
    case 5:
        world = simple_light();
        samples_per_pixel = 400;
        background = color(0, 0, 0);
        lookfrom = pointf3(26, 3, 6);
        lookat = pointf3(0, 2, 0);
        vfov = 20.0;
        max_depth = 100;
        break;
    case 6:
        world = cornell_box();
        aspect_ratio = 1.0;
        image_width = 600;
        samples_per_pixel = 100;
        background = color(0, 0, 0);
        lookfrom = pointf3(278, 278, -800);
        lookat = pointf3(278, 278, 0);
        vfov = 40.0;
        break;
    case 7:
        world = cornell_smoke();
        aspect_ratio = 1.0;
        image_width = 600;
        samples_per_pixel = 200;
        lookfrom = pointf3(278, 278, -800);
        lookat = pointf3(278, 278, 0);
        vfov = 40.0;
        break;
    default:
    case 8:
        world = final_scene();
        aspect_ratio = 1.0;
        image_width = 800;
        samples_per_pixel = 10000;
        background = color(0, 0, 0);
        lookfrom = pointf3(478, 278, -600);
        lookat = pointf3(278, 278, 0);
        vfov = 40.0;
        break;
    }

    // Camera
    vecf3 vup(0, 1, 0);
    auto dist_to_focus = 10.0;
    int image_height = static_cast<int>(image_width / aspect_ratio);

    camera cam(lookfrom, lookat, vup, vfov, aspect_ratio, aperture, dist_to_focus, 0.0, 1.0);

    // Render
    std::vector<std::vector<color>> image(image_height, std::vector<color>(image_width, color(0, 0, 0))); // img

    std::cout << "P3\n"
              << image_width << ' ' << image_height << "\n255\n";
    int i, j, k, s;
    double u, v;
    ray r;
    color pixel_color, image_color;
    auto start = high_resolution_clock::now();
    int sum = 0;

#pragma omp parallel for private(k, i, j, s, u, v, r, pixel_color) shared(image, sum) // OpenMP
    for (k = 0; k < image_height * image_width; k++) {
        j = k / image_width;
        i = k % image_width;
        // std::cerr << "\r(i,j): " << i << ' ' << j << std::flush;
        pixel_color = color(0, 0, 0);
        for (s = 0, image_color = color(0, 0, 0); s < samples_per_pixel; s += 1) {
            // 制造随机数，使得每个像素点的采样来自周围点的平均值，从而消除锯齿
            u = (i + random_double()) / (image_width - 1);
            v = (j + random_double()) / (image_height - 1);
            r = cam.get_ray(u, v);
            pixel_color += ray_color(r, background, world, max_depth);
        }
        image[j][i] = pixel_color;
#pragma omp critical
        {
            if (i == 0) {
                sum++;
                std::cerr << "\rScanlines remaining: " << image_height - sum << ' ' << std::flush;
            }
        }
    }

    // 按顺序写入图片，结果导出到 .ppm 格式文件中
    for (j = image_height - 1; j >= 0; j--) {
        for (i = 0; i < image_width; i++) {
            write_color(std::cout, image[j][i], samples_per_pixel); // 这里根据采样次数来计算平均颜色RGB
        }
    }

    auto end = high_resolution_clock::now();
    auto duration = duration_cast<seconds>(end - start);

    std::cerr << std::endl
              << "Time Cost:" << duration.count() << "s" << std::endl;

    /*
    No Parallel
    */
    // for (int j = image_height - 1; j >= 0; --j) {
    //     std::cerr << "\rScanlines remaining: " << j << ' ' << std::flush;
    //     for (int i = 0; i < image_width; ++i) {
    //         color pixel_color(0, 0, 0);
    //         for (int s = 0; s < samples_per_pixel; ++s) {
    //             auto u = (i + random_double()) / (image_width - 1);
    //             auto v = (j + random_double()) / (image_height - 1);
    //             ray r = cam.get_ray(u, v);
    //             pixel_color += ray_color(r, background, world, max_depth);
    //         }
    //         write_color(std::cout, pixel_color, samples_per_pixel);
    //     }
    // }

    // std::cerr << "\nDone.\n";
}

color ray_color(const ray &r, const color &background, const hittable &world) {
    struct hit_record rec;
    float p_RR = 0.95;                        // 概率反射系数

    if (!world.hit(r, 0.0001, infinity, rec)) // 在0-infinity范围内找最近邻的表面
        return background;

    ray scattered;
    color attenuation; // 吸收系数,相交处的颜色的吸收
    double pdf;
    color albedo;
    color emitted = rec.mat_ptr->emitted(rec.u, rec.v, rec.p);  // 发射光线的颜色
    if (rec.mat_ptr->scatter(r, rec, albedo, scattered, pdf)) { // 如果是可以反射的材料
        if (random_double() < p_RR)
            return emitted + attenuation * rec.mat_ptr->scattering_pdf(r, rec, scattered) * ray_color(scattered, background, world) / (p_RR * pdf);
        else
            return color(0, 0, 0); // 递归到一定深度后，返回黑色（在之后应该改为直接光照的颜色）
    } else {                       // 否则，光线追踪到光源处，返回发射光线的颜色（光源）递归结束
        return emitted;
    }
}

color ray_color(const ray &r, const color &background, const hittable &world, int depth) {
    struct hit_record rec;
    // 如果递归深度达到最大值，则返回黑色,意味着光线已经衰减为0了
    if (depth <= 0)
        return color(0, 0, 0);

    // 如果光线没有撞击到物体，则返回背景颜色
    if (!world.hit(r, 0.001, infinity, rec)) // 在0-infinity范围内找最近邻的表面
        return background;

    ray scattered;
    color attenuation; // 吸收系数,相交处的颜色的吸收
    double pdf;
    color albedo;
    color emitted = rec.mat_ptr->emitted(rec.u, rec.v, rec.p); // 发射光线的颜色

    if (rec.mat_ptr->scatter(r, rec, albedo, scattered, pdf))  // 如果是可以反射的材料
        // 返回光源+反射光线的颜色（递归）
        return emitted + albedo * rec.mat_ptr->scattering_pdf(r, rec, scattered) * ray_color(scattered, background, world, depth - 1) / pdf;
    // 否则，光线追踪到光源处，返回发射光线的颜色（光源）递归结束
    else
        return emitted;
}

hittable_list random_scene() {
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
hittable_list two_perlin_spheres() {
    hittable_list objects;

    auto pertext = make_shared<noise_texture>(4);
    objects.add(make_shared<sphere>(pointf3(0, -1000, 0), 1000, make_shared<lambertian>(pertext)));
    objects.add(make_shared<sphere>(pointf3(0, 2, 0), 2, make_shared<lambertian>(pertext)));

    return objects;
}
bvh_node world2bvh_scene(hittable_list &world) {
    bvh_node objs(world, 0, 1);
    return objs;
}
hittable_list earth() {
    auto earth_texture = make_shared<image_texture>("img/earthmap.jpg");
    auto earth_surface = make_shared<lambertian>(earth_texture);
    auto globe = make_shared<sphere>(pointf3(0, 0, 0), 2, earth_surface);

    return hittable_list(globe);
}
hittable_list simple_light() {
    hittable_list objects;
    auto pertext = make_shared<noise_texture>(4);
    objects.add(make_shared<sphere>(pointf3(0, -1000, 0), 1000, make_shared<lambertian>(pertext)));
    objects.add(make_shared<sphere>(pointf3(0, 2, 0), 2, make_shared<lambertian>(pertext)));

    auto difflight = make_shared<diffuse_light>(color(5, 5, 5));
    objects.add(make_shared<xy_rect>(3, 5, 1, 3, -2, difflight));
    // objects.add(make_shared<sphere>(pointf3(1,7,0),2,difflight));

    return objects;
}
hittable_list cornell_box() {
    hittable_list objects;

    auto red = make_shared<lambertian>(color(.65, .05, .05));
    auto white = make_shared<lambertian>(color(.73, .73, .73));
    auto green = make_shared<lambertian>(color(.12, .45, .15));
    auto light = make_shared<diffuse_light>(color(15, 15, 15));

    objects.add(make_shared<yz_rect>(0, 555, 0, 555, 555, green));
    objects.add(make_shared<yz_rect>(0, 555, 0, 555, 0, red));
    objects.add(make_shared<xz_rect>(213, 343, 227, 332, 554, light));
    objects.add(make_shared<xz_rect>(0, 555, 0, 555, 0, white));
    objects.add(make_shared<xz_rect>(0, 555, 0, 555, 555, white));
    objects.add(make_shared<xy_rect>(0, 555, 0, 555, 555, white));
    shared_ptr<hittable> box1 = make_shared<box>(pointf3(0, 0, 0), pointf3(165, 330, 165), white);
    box1 = make_shared<rotate_y>(box1, 15);
    box1 = make_shared<translate>(box1, vecf3(265, 0, 295));
    objects.add(box1);

    shared_ptr<hittable> box2 = make_shared<box>(pointf3(0, 0, 0), pointf3(165, 165, 165), white);
    box2 = make_shared<rotate_y>(box2, -18);
    box2 = make_shared<translate>(box2, vecf3(130, 0, 65));
    objects.add(box2);
    return objects;
}
hittable_list cornell_smoke() {
    hittable_list objects;

    auto red = make_shared<lambertian>(color(.65, .05, .05));
    auto white = make_shared<lambertian>(color(.73, .73, .73));
    auto green = make_shared<lambertian>(color(.12, .45, .15));
    auto light = make_shared<diffuse_light>(color(7, 7, 7));

    objects.add(make_shared<yz_rect>(0, 555, 0, 555, 555, green));
    objects.add(make_shared<yz_rect>(0, 555, 0, 555, 0, red));
    objects.add(make_shared<xz_rect>(113, 443, 127, 432, 554, light));
    objects.add(make_shared<xz_rect>(0, 555, 0, 555, 555, white));
    objects.add(make_shared<xz_rect>(0, 555, 0, 555, 0, white));
    objects.add(make_shared<xy_rect>(0, 555, 0, 555, 555, white));

    shared_ptr<hittable> box1 = make_shared<box>(pointf3(0, 0, 0), pointf3(165, 330, 165), white);
    box1 = make_shared<rotate_y>(box1, 15);
    box1 = make_shared<translate>(box1, vecf3(265, 0, 295));

    shared_ptr<hittable> box2 = make_shared<box>(pointf3(0, 0, 0), pointf3(165, 165, 165), white);
    box2 = make_shared<rotate_y>(box2, -18);
    box2 = make_shared<translate>(box2, vecf3(130, 0, 65));

    objects.add(make_shared<constant_medium>(box1, 0.01, color(0, 0, 0)));
    objects.add(make_shared<constant_medium>(box2, 0.01, color(1, 1, 1)));

    return objects;
}
hittable_list final_scene() {
    hittable_list boxes1;
    auto ground = make_shared<lambertian>(color(0.48, 0.83, 0.53));

    const int boxes_per_side = 20;
    for (int i = 0; i < boxes_per_side; i++) {
        for (int j = 0; j < boxes_per_side; j++) {
            auto w = 100.0;
            auto x0 = -1000.0 + i * w;
            auto z0 = -1000.0 + j * w;
            auto y0 = 0.0;
            auto x1 = x0 + w;
            auto y1 = random_double(1, 101);
            auto z1 = z0 + w;

            boxes1.add(make_shared<box>(pointf3(x0, y0, z0), pointf3(x1, y1, z1), ground));
        }
    }

    hittable_list objects;

    objects.add(make_shared<bvh_node>(boxes1, 0, 1));

    auto light = make_shared<diffuse_light>(color(7, 7, 7));
    objects.add(make_shared<xz_rect>(123, 423, 147, 412, 554, light));

    auto center1 = pointf3(400, 400, 200);
    auto center2 = center1 + vecf3(30, 0, 0);
    auto moving_sphere_material = make_shared<lambertian>(color(0.7, 0.3, 0.1));
    objects.add(make_shared<moving_sphere>(center1, center2, 0, 1, 50, moving_sphere_material));

    objects.add(make_shared<sphere>(pointf3(260, 150, 45), 50, make_shared<dielectric>(1.5)));
    objects.add(make_shared<sphere>(
        pointf3(0, 150, 145), 50, make_shared<metal>(color(0.8, 0.8, 0.9), 1.0)));

    auto boundary = make_shared<sphere>(pointf3(360, 150, 145), 70, make_shared<dielectric>(1.5));
    objects.add(boundary);
    objects.add(make_shared<constant_medium>(boundary, 0.2, color(0.2, 0.4, 0.9)));
    boundary = make_shared<sphere>(pointf3(0, 0, 0), 5000, make_shared<dielectric>(1.5));
    objects.add(make_shared<constant_medium>(boundary, .0001, color(1, 1, 1)));

    auto emat = make_shared<lambertian>(make_shared<image_texture>("img/earthmap.jpg"));
    objects.add(make_shared<sphere>(pointf3(400, 200, 400), 100, emat));
    auto pertext = make_shared<noise_texture>(0.1);
    objects.add(make_shared<sphere>(pointf3(220, 280, 300), 80, make_shared<lambertian>(pertext)));

    hittable_list boxes2;
    auto white = make_shared<lambertian>(color(.73, .73, .73));
    int ns = 1000;
    for (int j = 0; j < ns; j++) {
        boxes2.add(make_shared<sphere>(pointf3::random(0, 165), 10, white));
    }

    objects.add(make_shared<translate>(
        make_shared<rotate_y>(
            make_shared<bvh_node>(boxes2, 0.0, 1.0), 15),
        vecf3(-100, 270, 395)));

    return objects;
}