#include <chrono>
#include <iostream>
#include <memory>
#include <omp.h> // OpenMP
#include <vector>
#include "common.h"
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
#include "participate_medium.h"

#define NUM_THREADS 8 // 线程数

std::string img_name = "./output/img.png";
int max_depth = 50;

typedef struct Scene{
    color background;
    hittable_list world;
    shared_ptr<hittable_list> lights;
} Scene;

color ray_color(const ray &r, const Scene &scene,int choice);

color BRDF_ray_color(const ray &r, const Scene &scene);

color MIX_ray_color(const ray &r, const Scene & scene);

color NEE_ray_color(const ray &r, const Scene & scene, int depth,bool is_shadow= false);

color MIS_ray_color(const ray &r, const Scene & scene,int depth, double emitted_weight,bool is_shadow= false);

void balance_heuristic(double f_pdf, double g_pdf, double &weight_f, double &weight_g,int beta=1);

bvh_node world2bvh_scene(hittable_list &world);

hittable_list cornell_box();

hittable_list cornell_specular();

hittable_list cornell_glass();

hittable_list cornell_smoke();

hittable_list final_scene();

hittable_list sence_test();


int main() {
    using namespace std::chrono;
    // Parallel
    omp_set_num_threads(NUM_THREADS); // 设置线程

    // Image
    auto aspect_ratio = 3.0 / 2.0;
    int image_width = 400;
    int samples_per_pixel = 100;
    // World
    hittable_list world;
    bvh_node bvh_world;

    pointf3 lookfrom;
    pointf3 lookat;
    auto vfov = 40.0;    // 视野
    auto aperture = 0.0; // 光圈
    color background;
    //auto light_material = make_shared<diffuse_light>(color(15, 15, 15));
    auto lights = make_shared<hittable_list>();


    std::cout<<"Scene Options:"<<std::endl;
    std::cout<<"1. Cornell Box"<<std::endl;
    std::cout<<"2. Cornell Box with Specular"<<std::endl;
    std::cout<<"3. Cornell Box with Glass"<<std::endl;
    std::cout<<"4. Cornell Box with Smoke"<<std::endl;
    std::cout<<"5. Final Scene"<<std::endl;
    std::cout<<"6. Test Scene"<<std::endl;
    std::cout<<"Please choose a scene: ";
    int choice;
    std::cin>>choice;
    std::cout<<std::endl;
    switch (choice) {         // 这种用法是为了方便调试，可以直接切换场景
        case 1:
            world = cornell_box();
            aspect_ratio = 1.0;
            image_width = 600;
            samples_per_pixel = 32;
            background = color(0, 0, 0);
            lights->add(make_shared<xz_rect>(213, 343, 227, 332, 554, shared_ptr<material>()));
            lookfrom = pointf3(278, 278, -800);
            lookat = pointf3(278, 278, 0);
            vfov = 40.0;
            std::cout << "Cornell Box" << std::endl;
            img_name = std::string("./output/cornell-box.png");
            break;
        case 2:
            world = cornell_specular();
            aspect_ratio = 1.0;
            image_width = 600;
            samples_per_pixel = 512;
            background = color(0, 0, 0);
            lights->add(make_shared<xz_rect>(213, 343, 227, 332, 554, shared_ptr<material>()));
            lookfrom = pointf3(278, 278, -800);
            lookat = pointf3(278, 278, 0);
            vfov = 40.0;
            std::cout << "Cornell Specular Box" << std::endl;
            img_name = std::string("./output/cornell-specular-box.png");
            break;
        case 3:
            world = cornell_glass();
            aspect_ratio = 1.0;
            image_width = 600;
            samples_per_pixel = 32;
            background = color(0, 0, 0);
            //lights = make_shared<xz_rect>(213, 343, 227, 332, 554, shared_ptr<material>());
            lights->add(make_shared<xz_rect>(213, 343, 227, 332, 554, shared_ptr<material>()));
            //加入一个球面光源
            lights->add(make_shared<sphere>(pointf3(190, 90, 190), 90, shared_ptr<material>()));
            lookfrom = pointf3(278, 278, -800);
            lookat = pointf3(278, 278, 0);
            vfov = 40.0;
            std::cout << "Cornell Glassy Box" << std::endl;
            img_name = std::string("./output/cornell-glassy-box.png");
            break;
        case 4:
            world = cornell_smoke();
            aspect_ratio = 1.0;
            image_width = 600;
            samples_per_pixel = 32;
            background = color(0, 0, 0);
            lights->add(make_shared<xz_rect>(213, 343, 227, 332, 554, shared_ptr<material>()));
            lookfrom = pointf3(278, 278, -800);
            lookat = pointf3(278, 278, 0);
            vfov = 40.0;
            std::cout << "Cornell Smoke Box" << std::endl;
            img_name = std::string("./output/cornell-smoke-box.png");
            break;
        case 5:
            world = final_scene();
            aspect_ratio = 1.0;
            image_width = 800;
            samples_per_pixel = 256;
            background = color(0, 0, 0);
            lookfrom = pointf3(478, 278, -600);
            lookat = pointf3(278, 278, 0);
            lights->add(make_shared<xz_rect>(123, 423, 147, 412, 554, shared_ptr<material>()));
            vfov = 40.0;
            std::cout << "Final Scene" << std::endl;
            img_name = std::string("./output/final-scene.png");
            break;
        case 6:
            world = sence_test();
            aspect_ratio = 1.0;
            image_width = 600;
            samples_per_pixel = 32;
            background = color(0, 0, 0);
            //lights = make_shared<xz_rect>(213, 343, 227, 332, 554, shared_ptr<material>());
            lights->add(make_shared<xz_rect>(213, 343, 227, 332, 554, shared_ptr<material>()));
            lights->add(make_shared<sphere>(pointf3(190, 90, 190), 90, shared_ptr<material>()));
            lookfrom = pointf3(278, 278, -800);
            lookat = pointf3(278, 278, 0);
            vfov = 40.0;
            std::cout << "Test Scene" << std::endl;
            img_name = std::string("./output/cornell-test.png");
            break;
        default:
            world = cornell_box();
            aspect_ratio = 1.0;
            image_width = 600;
            samples_per_pixel = 10;
            background = color(0, 0, 0);
            lights->add(make_shared<xz_rect>(113, 443, 127, 432, 554, shared_ptr<material>()));
            lookfrom = pointf3(278, 278, -800);
            lookat = pointf3(278, 278, 0);
            vfov = 40.0;
            std::cout << "Cornell Box" << std::endl;
            break;
    }

    //Scene
    Scene scene{background,world,lights};

    // Camera
    vecf3 vup(0, 1, 0);
    auto dist_to_focus = 10.0;
    int image_height = static_cast<int>(image_width / aspect_ratio);
    camera cam(lookfrom, lookat, vup, vfov, aspect_ratio, aperture, dist_to_focus, 0.0, 1.0);

    // Render
    std::vector<color> img(image_height * image_width, color(0, 0, 0)); // img

    int i, j, k, s;
    double u, v;
    ray r;
    color pixel_color;
    auto start = high_resolution_clock::now();
    int sum = 0;

    std::cout<<std::endl<<"Render Methods:"<<std::endl;
    std::cout<<"1. BRDF Sample"<<std::endl;
    std::cout<<"2. Mixture Sample"<<std::endl;
    std::cout<<"3. NEE Sample"<<std::endl;
    std::cout<<"4. Multiple Importance Sample"<<std::endl;
    std::cout<<"Please choose a Method: ";
    int choice_method;
    std::cin>>choice_method;
    std::cout<<"Input spp: ";
    std::cin>>samples_per_pixel;
    std::cout<<std::endl;
    switch (choice_method)
    {
        case 1:
            std::cout<<"BRDF Sample"<<std::endl;
            break;
        case 2:
             std::cout<<"Mixture Sample"<<std::endl;
            break;
        case 3:
             std::cout<<"NEE Sample"<<std::endl;
            break;
        case 4:
             std::cout<<"Multiple Importance Sample"<<std::endl;
            break;
    }


#pragma omp parallel for private(k, i, j, s, u, v, r, pixel_color) shared(img, sum) // OpenMP
    for (k = 0; k < image_height * image_width; k++) {
        j = k / image_width;
        i = k % image_width;
        pixel_color = color(0, 0, 0);
        for (s = 0; s < samples_per_pixel; s += 1) {
            // 制造随机数，使得每个像素点的采样来自周围点的平均值，从而消除锯齿
            u = (i + random_double()) / (image_width - 1);
            v = (j + random_double()) / (image_height - 1);
            r = cam.get_ray(u, v);
            pixel_color += ray_color(r, scene,choice_method);
        }
        img[k] = pixel_color;
#pragma omp critical
        {
            if (i == 0) {
                sum++;
                std::cout << "\rScanlines remaining: " << image_height - sum << ' ' << std::flush;
            }
        }
    }

    //No Parallel
    /*
     for (int j = image_height - 1; j >= 0; --j) {
         std::cerr << "\rScanlines remaining: " << j << ' ' << std::flush;
         for (int i = 0; i < image_width; ++i) {
             color pixel_color(0, 0, 0);
             for (int s = 0; s < samples_per_pixel; ++s) {
                 auto u = (i + random_double()) / (image_width - 1);
                 auto v = (j + random_double()) / (image_height - 1);
                 ray r = cam.get_ray(u, v);
                 pixel_color += ray_color(r, background, world, lights, max_depth);
             }
             img[j * image_width + i] = pixel_color;
         }
     }
    */

    // 按顺序写入图片，结果导出到 .png 格式文件中
    write_img(img_name.c_str(), image_width, image_height, img, samples_per_pixel);


    auto end = high_resolution_clock::now();
    auto duration = duration_cast<seconds>(end - start);

    std::cout << std::endl << "Time Cost:"
              << duration.count()/60 << "min"
              << duration.count()%60 << "s" << std::endl;
    std::cout << "File output: " << img_name << std::endl;
    std::cout << "Done.\n";
}

color ray_color(const ray &r, const Scene &scene,int choice){
    color L(0,0,0);
    switch (choice) {
        case 1:
            L = BRDF_ray_color(r, scene);
            break;
        case 2:
            L = MIX_ray_color(r, scene);
            break;
        case 3:
            L = NEE_ray_color(r, scene, 0);
            break;
        case 4:
            L = MIS_ray_color(r, scene,  0, 1);
            break;
        default:
            L = BRDF_ray_color(r, scene);
            break;
    }
    if(isnan(L.x())) L[0] = 0;
    if(isnan(L.y())) L[1] = 0;
    if(isnan(L.z())) L[2] = 0;
    return L;
}

color BRDF_ray_color(const ray &r, const Scene &scene){
    hit_record rec;

    // If the ray hits nothing, return the background color.
    if (!scene.world.hit(r, 0.001, infinity, rec))
        return scene.background;

    scatter_record srec;
    color emitted = rec.mat_ptr->emitted(r, rec, rec.u, rec.v, rec.p);

    if (!rec.mat_ptr->scatter(r, rec, srec))
        return emitted;

    float p_RR = 0.9;     // 概率反射系数
    if (random_double() > p_RR)
        return color (0, 0, 0);

    if (srec.is_specular) {
        return srec.attenuation
               * BRDF_ray_color(srec.scatter_ray, scene)/p_RR;
    }

    auto light_ptr = make_shared<hittable_pdf>(scene.lights, rec.p);
    ray scatter_ray = ray(rec.p, srec.pdf_ptr->generate(), r.time());
    auto pdf_val = srec.pdf_ptr->value(scatter_ray.direction());

    return emitted
           + srec.attenuation * rec.mat_ptr->scattering_pdf(r, rec, scatter_ray)
             * BRDF_ray_color(scatter_ray, scene)/p_RR/ pdf_val;
}

color MIX_ray_color(const ray &r, const Scene &scene ){
    hit_record rec;

    // If we've exceeded the ray bounce limit, no more light is gathered.
//    if (depth <= 0)
//        return color(0,0,0);

    // If the ray hits nothing, return the background color.
    if (!scene.world.hit(r, 0.001, infinity, rec))
        return scene.background;

    scatter_record srec;
    color emitted = rec.mat_ptr->emitted(r, rec, rec.u, rec.v, rec.p);

    if (!rec.mat_ptr->scatter(r, rec, srec))
        return emitted;
    float p_RR = 0.9;     // 概率反射系数
    if (random_double() > p_RR)
        return color (0, 0, 0);

    if (srec.is_specular) {
        return srec.attenuation
               * MIX_ray_color(srec.scatter_ray, scene)/p_RR;
    }

    auto light_ptr = make_shared<hittable_pdf>(scene.lights, rec.p);
    mixture_pdf p(light_ptr, srec.pdf_ptr);
    ray scatter_ray = ray(rec.p, p.generate(), r.time());
    auto pdf_val = p.value(scatter_ray.direction());

    return emitted
           + srec.attenuation * rec.mat_ptr->scattering_pdf(r, rec, scatter_ray)
             * MIX_ray_color(scatter_ray, scene )
             / pdf_val/p_RR;
}

color NEE_ray_color(const ray &r, const Scene &scene, int depth, bool is_shadow) {

    struct hit_record rec;
    float p_RR = 0.95;                        // 概率反射系数

    if (!scene.world.hit(r, 0.0001, infinity, rec)) // 在0-infinity范围内找最近邻的表面
        return scene.background;
    //srec 用于记录该材质的散射信息，包括衰减系数，散射光线方向分布，是否为镜面反射
    struct scatter_record srec;

    //除了光源以外，自发光emitted都是黑色的(0,0,0)
    color emitted = rec.mat_ptr->emitted(r, rec, rec.u, rec.v, rec.p); // 发射光线的颜色
    if (!rec.mat_ptr->scatter(r, rec, srec)){
        if(is_shadow||depth == 0)
            return emitted;
        else
            return color(0, 0, 0);
    }else if(srec.is_medium&&is_shadow){
        struct hit_record rec_lgt;
        ray shadow_ray = ray(rec.p+r.direction()*0.001,r.direction(),r.time());
        if (!rec.boundary_ptr->hit(shadow_ray, 0.001, infinity, rec_lgt)) { // 在0-infinity范围内找到内表面位置
            return  NEE_ray_color(shadow_ray, scene, depth + 1,  true);
        }
        else{
            shadow_ray = ray(rec_lgt.p+r.direction()*0.001,r.direction(),r.time());
            double light_trans_distance = (rec_lgt.p-rec.p).length();
            double light_attenuation = exp(-rec.density *light_trans_distance);
            return srec.attenuation *light_attenuation*
                   NEE_ray_color(shadow_ray, scene, depth + 1,  true);
        }
    }

    if (random_double() > p_RR)
        return color (0, 0, 0);

    if (srec.is_specular) {
        return srec.attenuation * NEE_ray_color(srec.scatter_ray, scene,depth,is_shadow) / p_RR;
    }
    if(srec.is_refract){
        return srec.attenuation * NEE_ray_color(srec.scatter_ray, scene,depth+1,is_shadow) / p_RR;
    }
    //若光线没有追踪到光源，不是镜面反射，且是最后的shadow ray 则返回0
    if(is_shadow){
        return color (0, 0, 0);
    }

    //直接光照
    auto light_pdf_ptr = make_shared<hittable_pdf>(scene.lights, rec.p);
    vecf3 light_direction = unit_vector(light_pdf_ptr->generate());
    ray shadow_ray = ray(rec.p, light_direction, r.time());
    double p_dir = light_pdf_ptr->value(shadow_ray.direction());
    color direct_light,indirect_light;

    material_type m;
    rec.mat_ptr->getType(m);
    if(m != material_type::Isotropic){
        if(p_dir) {
            direct_light = srec.attenuation * rec.mat_ptr->scattering_pdf(r, rec, shadow_ray)
                           * NEE_ray_color(shadow_ray, scene, depth + 1,  true) / p_dir;
        }
    }else{//处理体渲染
        struct hit_record rec_lgt;
        if (!rec.boundary_ptr->hit(shadow_ray, 0.001, infinity, rec_lgt)) { // 在0-infinity范围内找到内表面位置
            if (p_dir)
                direct_light =srec.attenuation * rec.mat_ptr->scattering_pdf(r, rec, shadow_ray)*
                        NEE_ray_color(shadow_ray, scene, depth + 1,  true)
                        / p_dir;
        }
        else{
//            shadow_ray = ray(rec_lgt.p+light_direction*0.001,light_direction,r.time());
//            double light_trans_distance = (rec_lgt.p-rec.p).length();
//            double light_attenuation = exp(-rec.density*light_trans_distance);
//            if (p_dir)
//                direct_light =srec.attenuation *light_attenuation* rec.mat_ptr->scattering_pdf(r, rec, shadow_ray)*
//                              NEE_ray_color(shadow_ray, scene, depth + 1,  true)
//                              / p_dir;
            direct_light = color(0,0,0);
        }
    }

//    //如果光线与光源相交则有光线，否则light_color为(0,0,0), 所以这里隐含了Visibility Test
//    if(p_dir) {
//        direct_light = srec.attenuation * rec.mat_ptr->scattering_pdf(r, rec, shadow_ray)
//                             * NEE_ray_color(shadow_ray, scene, depth + 1, true) / p_dir;
//    }
    //间接光照
    vecf3 scattered_direction = unit_vector(srec.pdf_ptr->generate());
    ray scatter_ray = ray(rec.p, scattered_direction, r.time());
    double p_indir = srec.pdf_ptr->value(scattered_direction);
    indirect_light = srec.attenuation * rec.mat_ptr->scattering_pdf(r, rec, scatter_ray)
                           * NEE_ray_color(scatter_ray, scene, depth + 1)/p_indir;

    return emitted + (direct_light + indirect_light)/p_RR;
}

color MIS_ray_color(const ray &r, const Scene &scene,int depth,double emitted_weight,bool is_shadow) {

    //TODO:处理三角形面片

//    if(depth>=max_depth)
//        return color(0,0,0);
    struct hit_record rec;
    color result;
    if (!scene.world.hit(r, 0.001, infinity, rec)) { // 在0-infinity范围内找最近邻的表面
        return scene.background * emitted_weight;
    }
    //srec 用于记录该材质的散射信息，包括衰减系数，散射光线方向分布，是否为镜面反射
    struct scatter_record srec;
    //如果光线追踪到光源，则返回光源的颜色
    color emitted = rec.mat_ptr->emitted(r, rec, rec.u, rec.v, rec.p); // 发射光线的颜色
    if(!rec.mat_ptr->scatter(r, rec, srec)) {
        return emitted * emitted_weight;
    }else if(srec.is_medium&&is_shadow){
        struct hit_record rec_lgt;
        ray shadow_ray = ray(rec.p+r.direction()*0.001,r.direction(),r.time());
        if (!rec.boundary_ptr->hit(shadow_ray, 0.001, infinity, rec_lgt)) { // 在0-infinity范围内找到内表面位置
            return  MIS_ray_color(shadow_ray, scene, depth + 1, emitted_weight, true);
        }
        else{
            shadow_ray = ray(rec_lgt.p+r.direction()*0.001,r.direction(),r.time());
            double light_trans_distance = (rec_lgt.p-rec.p).length();
            double light_attenuation = exp(-0.01*light_trans_distance);
            return srec.attenuation *light_attenuation*
                   MIS_ray_color(shadow_ray, scene, depth + 1, emitted_weight, true);
        }
    }

    //Russia Rotate
    float p_RR = 0.9;     // 概率反射系数
    if (random_double() > p_RR)
        return color (0, 0, 0);

    //如果是镜面反射，直接返回镜面反射的颜色，不依赖于光源
    if(srec.is_specular){
        return srec.attenuation * MIS_ray_color(srec.scatter_ray, scene, depth, emitted_weight,is_shadow)/p_RR;
    }
    if(srec.is_refract){
        return srec.attenuation * MIS_ray_color(srec.scatter_ray, scene,depth+1,is_shadow) / p_RR;
    }
    //若光线没有追踪到光源，不是镜面反射，且是最后的shadow ray 则返回0
    if(is_shadow){
        return color (0, 0, 0);
    }
    shared_ptr<hittable_pdf> light_pdf_ptr;
    vecf3 light_direction;
    ray shadow_ray,scatter_ray;
    double mis_brdf_sample, mis_light_sample,mis_tmp;
    double p_dir, p_indir,p_dir_tmp,p_indir_tmp;
    color direct_light(0,0,0), indirect_light(0,0,0);

    //直接光照
    light_pdf_ptr = make_shared<hittable_pdf>(scene.lights, rec.p);
    light_direction = unit_vector(light_pdf_ptr->generate());
    shadow_ray = ray(rec.p, light_direction, r.time());
    p_dir = light_pdf_ptr->value(shadow_ray.direction());
    //间接光照
    scatter_ray = ray(rec.p, unit_vector( srec.pdf_ptr->generate()), r.time());
    p_indir = srec.pdf_ptr->value(scatter_ray.direction());
    //启发式平衡函数，这正是MIS的权重函数
    p_dir_tmp = light_pdf_ptr->value(scatter_ray.direction());
    p_indir_tmp = srec.pdf_ptr->value(shadow_ray.direction());
    balance_heuristic(p_indir, p_dir_tmp, mis_brdf_sample, mis_tmp, 2);
    balance_heuristic(p_indir_tmp, p_dir, mis_tmp, mis_light_sample, 2);


    //处理一般渲染
    material_type m;
    rec.mat_ptr->getType(m);
    if(m != material_type::Isotropic){
        if(p_dir) {
            direct_light = srec.attenuation * rec.mat_ptr->scattering_pdf(r, rec, shadow_ray)
                           * MIS_ray_color(shadow_ray, scene, depth + 1, mis_light_sample, true) / p_dir;
        }

        if(p_indir) {
            indirect_light = srec.attenuation * rec.mat_ptr->scattering_pdf(r, rec, scatter_ray)
                             * MIS_ray_color(scatter_ray, scene, depth + 1, mis_brdf_sample) / p_indir;
        }

    }else{
        //处理体渲染
        struct hit_record rec_lgt;
        if (!rec.boundary_ptr->hit(shadow_ray, 0.001, infinity, rec_lgt)) { // 在0-infinity范围内找到内表面位置
            if (p_dir)
                direct_light =srec.attenuation * rec.mat_ptr->scattering_pdf(r, rec, shadow_ray)*
                              MIS_ray_color(shadow_ray, scene, depth + 1, mis_light_sample, true)
                              / p_dir;
        }
        else{
//            shadow_ray = ray(rec_lgt.p+light_direction*0.001,light_direction,r.time());
//            double light_trans_distance = (rec_lgt.p-rec.p).length();
//            double light_attenuation = exp(-0.01*light_trans_distance);
//            if (p_dir)
//                direct_light =srec.attenuation *light_attenuation* rec.mat_ptr->scattering_pdf(r, rec, shadow_ray)*
//                              MIS_ray_color(shadow_ray, scene, depth + 1, mis_light_sample, true)
//                              / p_dir;
            direct_light = color(0,0,0);
        }

        if(p_indir) {
            indirect_light = srec.attenuation * rec.mat_ptr->scattering_pdf(r, rec, scatter_ray)
                             * MIS_ray_color(scatter_ray, scene, depth + 1, 1) / p_indir;
        }
    }


    result = emitted + (direct_light + indirect_light) / p_RR;
    return result;

}

void balance_heuristic(double f_pdf, double g_pdf, double &weight_f, double &weight_g, int beta){
    if(beta!=1) {
        f_pdf = pow(f_pdf, beta);
        g_pdf = pow(g_pdf, beta);
    }
    weight_f = f_pdf / (f_pdf + g_pdf);
    weight_g = g_pdf / (f_pdf + g_pdf);
}

bvh_node world2bvh_scene(hittable_list &world) {
    bvh_node objs(world, 0, 1);
    return objs;
}

hittable_list cornell_box() {
    hittable_list objects;

    auto red = make_shared<lambertian>(color(.65, .05, .05));
    auto white = make_shared<lambertian>(color(.73, .73, .73));
    auto green = make_shared<lambertian>(color(.12, .45, .15));
    auto light = make_shared<diffuse_light>(color(15, 15, 15));

    objects.add(make_shared<yz_rect>(0, 555, 0, 555, 555, green));
    objects.add(make_shared<yz_rect>(0, 555, 0, 555, 0, red));
    objects.add(make_shared<flip_face>(make_shared<xz_rect>(213, 343, 227, 332, 554, light)));
    objects.add(make_shared<xz_rect>(0, 555, 0, 555, 0, white));
    objects.add(make_shared<xz_rect>(0, 555, 0, 555, 555, white));
    objects.add(make_shared<xy_rect>(0, 555, 0, 555, 555, white));


    //粗糙材质
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

hittable_list cornell_specular() {
    hittable_list objects;

    auto red = make_shared<lambertian>(color(.65, .05, .05));
    auto white = make_shared<lambertian>(color(.73, .73, .73));
    auto green = make_shared<lambertian>(color(.12, .45, .15));
    auto light = make_shared<diffuse_light>(color(15, 15, 15));

    objects.add(make_shared<yz_rect>(0, 555, 0, 555, 555, green));
    objects.add(make_shared<yz_rect>(0, 555, 0, 555, 0, red));
    objects.add(make_shared<flip_face>(make_shared<xz_rect>(213, 343, 227, 332, 554, light)));
    objects.add(make_shared<xz_rect>(0, 555, 0, 555, 0, white));
    objects.add(make_shared<xz_rect>(0, 555, 0, 555, 555, white));
    objects.add(make_shared<xy_rect>(0, 555, 0, 555, 555, white));

//   金属材质
    shared_ptr<material> aluminum = make_shared<metal>(color(0.8, 0.85, 0.88), 0.0);
    shared_ptr<hittable> box1 = make_shared<box>(pointf3(0, 0, 0), pointf3(165, 330, 165), aluminum);
    box1 = make_shared<rotate_y>(box1, 15);
    box1 = make_shared<translate>(box1, vecf3(265, 0, 295));
    objects.add(box1);

    //粗糙材质
    shared_ptr<hittable> box2 = make_shared<box>(pointf3(0, 0, 0), pointf3(165, 165, 165), white);
    box2 = make_shared<rotate_y>(box2, -18);
    box2 = make_shared<translate>(box2, vecf3(130, 0, 65));
    objects.add(box2);

    return objects;
}

hittable_list cornell_glass() {
    hittable_list objects;

    auto red = make_shared<lambertian>(color(.65, .05, .05));
    auto white = make_shared<lambertian>(color(.73, .73, .73));
    auto green = make_shared<lambertian>(color(.12, .45, .15));
    auto light = make_shared<diffuse_light>(color(15, 15, 15));

    objects.add(make_shared<yz_rect>(0, 555, 0, 555, 555, green));
    objects.add(make_shared<yz_rect>(0, 555, 0, 555, 0, red));
    objects.add(make_shared<flip_face>(make_shared<xz_rect>(213, 343, 227, 332, 554, light)));
    objects.add(make_shared<xz_rect>(0, 555, 0, 555, 0, white));
    objects.add(make_shared<xz_rect>(0, 555, 0, 555, 555, white));
    objects.add(make_shared<xy_rect>(0, 555, 0, 555, 555, white));


    //粗糙材质
    shared_ptr<hittable> box1 = make_shared<box>(pointf3(0, 0, 0), pointf3(165, 330, 165), white);
    box1 = make_shared<rotate_y>(box1, 15);
    box1 = make_shared<translate>(box1, vecf3(265, 0, 295));
    objects.add(box1);

    //添加一个玻璃球
    auto glass = make_shared<dielectric>(1.5);
    objects.add(make_shared<sphere>(pointf3(190, 90, 190), 90, glass));

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
    objects.add(make_shared<flip_face>(make_shared<xz_rect>(113, 443, 127, 432, 554, light)));
//    objects.add(make_shared<flip_face>(make_shared<xz_rect>(213, 343, 227, 332, 554, light)));

    objects.add(make_shared<xz_rect>(0, 555, 0, 555, 555, white));
    objects.add(make_shared<xz_rect>(0, 555, 0, 555, 0, white));
    objects.add(make_shared<xy_rect>(0, 555, 0, 555, 555, white));

    shared_ptr<hittable> box1 = make_shared<box>(pointf3(0, 0, 0), pointf3(165, 330, 165), white);
    box1 = make_shared<rotate_y>(box1, 15);
    box1 = make_shared<translate>(box1, vecf3(265, 0, 295));

    shared_ptr<hittable> box2 = make_shared<box>(pointf3(0, 0, 0), pointf3(165, 165, 165), white);
    box2 = make_shared<rotate_y>(box2, -18);
    box2 = make_shared<translate>(box2, vecf3(130, 0, 65));

    objects.add(make_shared<constant_medium>(box1, 0.01, color(0.1,0.1,0.1)));
    objects.add(make_shared<constant_medium>(box2, 0.01, color(1,1,1)));

//    objects.add(make_shared<participate_medium>(box1, color(0.2, 0.3, 0)));
//    objects.add(make_shared<participate_medium>(box2,  color(1, 1, 1)));

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
    objects.add(make_shared<flip_face>(make_shared<xz_rect>(123, 423, 147, 412, 554, light)));


    auto center1 = pointf3(400, 400, 200);
    auto center2 = center1 + vecf3(30, 0, 0);
    auto moving_sphere_material = make_shared<lambertian>(color(0.7, 0.3, 0.1));
    objects.add(make_shared<sphere>(center1, 50, moving_sphere_material));

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
hittable_list sence_test() {
    hittable_list objects;

    auto red = make_shared<lambertian>(color(.65, .05, .05));
    auto white = make_shared<lambertian>(color(.73, .73, .73));
    auto green = make_shared<lambertian>(color(.12, .45, .15));
    auto light = make_shared<diffuse_light>(color(15, 15, 15));

    objects.add(make_shared<yz_rect>(0, 555, 0, 555, 555, green));
    objects.add(make_shared<yz_rect>(0, 555, 0, 555, 0, red));
    objects.add(make_shared<flip_face>(make_shared<xz_rect>(213, 343, 227, 332, 554, light)));
    objects.add(make_shared<xz_rect>(0, 555, 0, 555, 0, white));
    objects.add(make_shared<xz_rect>(0, 555, 0, 555, 555, white));
    objects.add(make_shared<xy_rect>(0, 555, 0, 555, 555, white));


    //粗糙材质
    shared_ptr<hittable> box1 = make_shared<box>(pointf3(0, 0, 0), pointf3(165, 330, 165), white);
    box1 = make_shared<rotate_y>(box1, 15);
    box1 = make_shared<translate>(box1, vecf3(265, 0, 295));
    objects.add(box1);

    //添加一个玻璃球
    auto glass = make_shared<dielectric>(1.5);
    auto boundary = make_shared<sphere>(pointf3(190, 90, 190), 90, glass);
    objects.add(boundary);
    objects.add(make_shared<constant_medium>(boundary, 0.2, color(0.2, 0.4, 0.9)));
    return objects;
}




//TODO:Ray Marching
//
//    shared_ptr<hittable> medium_ptr = rec.hit_ptr;
//
//    double t0, t1;
//    t0 = rec.t;
//    struct hit_record rec1;
//    vecf3 scattered_direction = unit_vector(r.direction());
//    scatter_ray = ray(rec.p+0.001*scattered_direction,scattered_direction , r.time());
//    if (!medium_ptr->hit(scatter_ray, 0.001, infinity, rec1)) // 在0-infinity范围内找最近邻的表面
//        return MIS_ray_color(scatter_ray, scene, depth + 1, emitted_weight, is_shadow);
//    t1 = rec1.t;
//    t0 = 0;
//    scatter_ray = ray(rec1.p+0.001*scattered_direction, unit_vector(r.direction()), r.time());
//    int ns = 10;
//    double sigma_t = 0.1;
//    double step = (t1 - t0) / ns;
//    double transparent=1;
//    float sample_attenuation = exp(-step * sigma_t*scatter_ray.direction().length());
//    color result(0,0,0);
//    for(int n=0;n<ns && transparent>0.0001; n++){
//        double t = t0 + (n+0.5) * step;
//        vecf3 x = rec.p + t * scattered_direction;
//        transparent *= sample_attenuation;
//        struct hit_record rec_lgt;
//        light_pdf_ptr = make_shared<hittable_pdf>(scene.lights, x);
//        light_direction = light_pdf_ptr->generate();
//        shadow_ray = ray(x, light_direction, r.time());
//        if (medium_ptr->hit(shadow_ray, 0.001, infinity, rec_lgt)) {
//            double light_trans_distance = (rec_lgt.p - x).length();
//            double light_attenuation = exp(-sigma_t * light_trans_distance);
//            shadow_ray = ray(rec_lgt.p+0.001* unit_vector(light_direction), light_direction, r.time());
//            result += transparent*light_attenuation*srec.attenuation*MIS_ray_color(shadow_ray, scene, depth + 1,1, true)*step;
//        }
//    }
//    if(transparent>0.0001)
//        result += transparent * MIS_ray_color(scatter_ray, scene, depth + 1, 1);
//    return result / p_RR;