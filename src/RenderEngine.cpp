#include "RenderEngine.h"

color RenderEngine::computePixelColor(int i, int j, int spp, SampleMethod method) const {
    double u, v;
    ray r;
    color pixel_color(0, 0, 0);
    for (int s = 0; s < spp; s += 1) {
        u = (i + random_double()) / (width - 1);
        v = (j + random_double()) / (height - 1);
        r = scene.cam->get_ray(u, v);
        pixel_color += ray_color(r, method);
    }
    return pixel_color;
}

void RenderEngine::render(int spp, SampleMethod method, const std::string &img_name, bool isOpenMP) {
    using namespace std::chrono;
    omp_set_num_threads(NUM_THREADS);

    auto start = high_resolution_clock::now();
    std::cout << "Rendering..." << std::endl;
    std::vector<color> img(width * height, color(0, 0, 0));

    int i, j, k;
    std::atomic<int> sum(0);

    auto reportProgress = [this, &sum]() {
        int progress = static_cast<int>(100.0 * sum / height);
        if (progressCallback) {
            progressCallback(progress);
        }
    };
    if (isOpenMP) {
        std::cout<<"OpenMP"<<std::endl;
#pragma omp parallel for private(k, i, j) shared(img, sum)
        for (k = 0; k < width * height; k++) {
            j = k / width;
            i = k % height;
            img[k] = computePixelColor(i, j, spp, method);

            if (i == 0) {
#pragma omp critical
                {
                    sum++;
                    std::cerr << "\rScanlines remaining: " << height - sum << ' ' << std::flush;
                    reportProgress();
                }
            }
        }
    }
    else {
        sum = 0;
        std::cout<<"No OpenMP"<<std::endl;
        for (k = 0; k < width * height; k++) {
            j = k / width;
            i = k % height;
            img[k] = computePixelColor(i, j, spp, method);
            if (i == 0) {
                sum++;
                std::cerr << "\rScanlines remaining: " << height - sum << ' ' << std::flush;
                reportProgress();
            }
        }
    }

    write_img(img_name.c_str(), width, height, img, spp);

    auto end = high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<seconds>(end - start);

    std::cerr << std::endl << "Time Cost:"
              << duration.count() / 60 << "min"
              << duration.count() % 60 << "s" << std::endl;
    std::cerr << "Writing to " << img_name << std::endl;
    std::cerr << "Done.\n" << std::endl;

}

color RenderEngine::ray_color(const ray &r, SampleMethod method) const {
    color L(0, 0, 0);
    switch (method) {
        case SampleMethod::BRDF:
            L = BRDF_sample(r);
            break;
        case SampleMethod::Light:
            L = light_sample(r);
            break;
        case SampleMethod::Mixture:
            L = Mixture_sample(r);
            break;
        case SampleMethod::NEE:
            L = NEE_sample(r, 0);
            break;
        case SampleMethod::MIS:
            L = Muliti_Importance_sample(r, 0, 1);
            break;
        default:
            L = BRDF_sample(r);
            break;
    }
    if (isnan(L.x())) L[0] = 0;
    if (isnan(L.y())) L[1] = 0;
    if (isnan(L.z())) L[2] = 0;
    return L;
}

color RenderEngine::BRDF_sample(const ray &r) const {
    hit_record rec;
    // If the ray hits nothing, return the background color.
    if (!scene.world.hit(r, 0.001, infinity, rec))
        return scene.background->value(r);

    scatter_record srec;
    color emitted = rec.mat_ptr->emitted(r, rec, rec.u, rec.v, rec.p);

    if (!rec.mat_ptr->scatter(r, rec, srec))
        return emitted;

    float p_RR = 0.9;     // 概率反射系数
    if (random_double() > p_RR)
        return color(0, 0, 0);

    if (srec.is_specular) {
        return srec.attenuation
               * BRDF_sample(srec.scatter_ray) / p_RR;
    }

    auto light_ptr = make_shared<hittable_pdf>(scene.lights, rec.p);
    ray scatter_ray = ray(rec.p, srec.pdf_ptr->generate(), r.time());
    auto pdf_val = srec.pdf_ptr->value(scatter_ray.direction());

    return emitted
           + srec.attenuation * rec.mat_ptr->scattering_pdf(r, rec, scatter_ray)
             * BRDF_sample(scatter_ray) / p_RR / pdf_val;
}

color RenderEngine::light_sample(const ray &r) const {
    hit_record rec;
    // If the ray hits nothing, return the background color.
    if (!scene.world.hit(r, 0.001, infinity, rec))
        return scene.background->value(r);

    scatter_record srec;
    color emitted = rec.mat_ptr->emitted(r, rec, rec.u, rec.v, rec.p);

    if (!rec.mat_ptr->scatter(r, rec, srec))
        return emitted;

    float p_RR = 0.9;     // 概率反射系数
    if (random_double() > p_RR)
        return color(0, 0, 0);

    if (srec.is_specular) {
        return srec.attenuation
               * light_sample(srec.scatter_ray) / p_RR;
    }

    auto light_ptr = make_shared<hittable_pdf>(scene.lights, rec.p);
    ray scatter_ray = ray(rec.p, light_ptr->generate(), r.time());
    auto pdf_val = light_ptr->value(scatter_ray.direction());
    if (pdf_val) {
        return emitted
               + srec.attenuation * rec.mat_ptr->scattering_pdf(r, rec, scatter_ray)
                 * light_sample(scatter_ray) / p_RR / pdf_val;
    } else {
        return emitted;
    }
}

color RenderEngine::Mixture_sample(const ray &r) const {
    hit_record rec;

    // If we've exceeded the ray bounce limit, no more light is gathered.
//    if (depth <= 0)
//        return color(0,0,0);

    // If the ray hits nothing, return the background color.
    if (!scene.world.hit(r, 0.001, infinity, rec))
        return scene.background->value(r);

    scatter_record srec;
    color emitted = rec.mat_ptr->emitted(r, rec, rec.u, rec.v, rec.p);

    if (!rec.mat_ptr->scatter(r, rec, srec))
        return emitted;
    float p_RR = 0.9;     // 概率反射系数
    if (random_double() > p_RR)
        return color(0, 0, 0);

    if (srec.is_specular) {
        return srec.attenuation
               * Mixture_sample(srec.scatter_ray) / p_RR;
    }

    auto light_ptr = make_shared<hittable_pdf>(scene.lights, rec.p);
    mixture_pdf p(light_ptr, srec.pdf_ptr);
    ray scatter_ray = ray(rec.p, p.generate(), r.time());
    auto pdf_val = p.value(scatter_ray.direction());

    return emitted
           + srec.attenuation * rec.mat_ptr->scattering_pdf(r, rec, scatter_ray)
             * Mixture_sample(scatter_ray)
             / pdf_val / p_RR;
}

color RenderEngine::NEE_sample(const ray &r, int depth, bool is_shadow) const {

    struct hit_record rec;
    float p_RR = 0.95;                        // 概率反射系数

    if (!scene.world.hit(r, 0.0001, infinity, rec)) // 在0-infinity范围内找最近邻的表面
        return scene.background->value(r);
    //srec 用于记录该材质的散射信息，包括衰减系数，散射光线方向分布，是否为镜面反射
    struct scatter_record srec;

    //除了光源以外，自发光emitted都是黑色的(0,0,0)
    color emitted = rec.mat_ptr->emitted(r, rec, rec.u, rec.v, rec.p); // 发射光线的颜色
    if (!rec.mat_ptr->scatter(r, rec, srec)) {
        if (is_shadow || depth == 0)
            return emitted;
        else
            return color(0, 0, 0);
    } else if (srec.is_medium && is_shadow) {
        struct hit_record rec_lgt;
        ray shadow_ray = ray(rec.p + r.direction() * 0.001, r.direction(), r.time());
        if (!rec.boundary_ptr->hit(shadow_ray, 0.001, infinity, rec_lgt)) { // 在0-infinity范围内找到内表面位置
            return NEE_sample(shadow_ray, depth + 1, true);
        } else {
            shadow_ray = ray(rec_lgt.p + r.direction() * 0.001, r.direction(), r.time());
            double light_trans_distance = (rec_lgt.p - rec.p).length();
            double light_attenuation = exp(-rec.density * light_trans_distance);
            return srec.attenuation * light_attenuation *
                   NEE_sample(shadow_ray, depth + 1, true);
        }
    }

    if (random_double() > p_RR)
        return color(0, 0, 0);
    if (srec.is_specular||srec.is_refract) {
        return srec.attenuation * NEE_sample(srec.scatter_ray, depth, is_shadow) / p_RR;
    }
    if (is_shadow) {
        return color(0, 0, 0);
    }

    //若光线没有追踪到光源，不是镜面反射，且是最后的shadow ray 则返回0
    //直接光照
    auto light_pdf_ptr = make_shared<hittable_pdf>(scene.lights, rec.p);
    vecf3 light_direction = unit_vector(light_pdf_ptr->generate());
    ray shadow_ray = ray(rec.p, light_direction, r.time());
    double p_dir = light_pdf_ptr->value(shadow_ray.direction());
    color direct_light, indirect_light;

    material_type m;
    rec.mat_ptr->getType(m);
    if (m != material_type::Isotropic) {
        if (p_dir) {
            direct_light = srec.attenuation * rec.mat_ptr->scattering_pdf(r, rec, shadow_ray)
                           * NEE_sample(shadow_ray, depth + 1, true) / p_dir;
        }
    } else {//处理体渲染
        struct hit_record rec_lgt;
        if (!rec.boundary_ptr->hit(shadow_ray, 0.001, infinity, rec_lgt)) { // 在0-infinity范围内找到内表面位置
            if (p_dir)
                direct_light = srec.attenuation * rec.mat_ptr->scattering_pdf(r, rec, shadow_ray) *
                               NEE_sample(shadow_ray, depth + 1, true)
                               / p_dir;
        } else {
//            shadow_ray = ray(rec_lgt.p+light_direction*0.001,light_direction,r.time());
//            double light_trans_distance = (rec_lgt.p-rec.p).length();
//            double light_attenuation = exp(-rec.density*light_trans_distance);
//            if (p_dir)
//                direct_light =srec.attenuation *light_attenuation* rec.mat_ptr->scattering_pdf(r, rec, shadow_ray)*
//                              NEE_ray_color(shadow_ray, scene, depth + 1,  true)
//                              / p_dir;
            direct_light = color(0, 0, 0);
        }
    }
    //间接光照
    vecf3 scattered_direction = unit_vector(srec.pdf_ptr->generate());
    ray scatter_ray = ray(rec.p, scattered_direction, r.time());
    double p_indir = srec.pdf_ptr->value(scattered_direction);
    indirect_light = srec.attenuation * rec.mat_ptr->scattering_pdf(r, rec, scatter_ray)
                     * NEE_sample(scatter_ray, depth + 1, false) / p_indir;

    return emitted + (direct_light + indirect_light) / p_RR;
}

color RenderEngine::Muliti_Importance_sample(const ray &r, int depth, double emitted_weight, bool is_shadow) const {


//    if(depth>=max_depth)
//        return color(0,0,0);
    struct hit_record rec;
    color result;
    if (!scene.world.hit(r, 0.001, infinity, rec)) { // 在0-infinity范围内找最近邻的表面
        return scene.background->value(r) * emitted_weight;
    }
    //srec 用于记录该材质的散射信息，包括衰减系数，散射光线方向分布，是否为镜面反射
    struct scatter_record srec;
    //如果光线追踪到光源，则返回光源的颜色
    color emitted = rec.mat_ptr->emitted(r, rec, rec.u, rec.v, rec.p); // 发射光线的颜色
    if (!rec.mat_ptr->scatter(r, rec, srec)) {
        return emitted * emitted_weight;
    } else if (srec.is_medium && is_shadow) {
        struct hit_record rec_lgt;
        ray shadow_ray = ray(rec.p + r.direction() * 0.001, r.direction(), r.time());
        if (!rec.boundary_ptr->hit(shadow_ray, 0.001, infinity, rec_lgt)) { // 在0-infinity范围内找到内表面位置
            return Muliti_Importance_sample(shadow_ray, depth + 1, emitted_weight, true);
        } else {
            shadow_ray = ray(rec_lgt.p + r.direction() * 0.001, r.direction(), r.time());
            double light_trans_distance = (rec_lgt.p - rec.p).length();
            double light_attenuation = exp(-0.01 * light_trans_distance);
            return srec.attenuation * light_attenuation *
                   Muliti_Importance_sample(shadow_ray, depth + 1, emitted_weight, true);
        }
    }

    //Russia Rotate
    float p_RR = 0.9;     // 概率反射系数
    if (random_double() > p_RR)
        return color(0, 0, 0);

    //如果是镜面反射，直接返回镜面反射的颜色，不依赖于光源
    if (srec.is_specular) {
        return srec.attenuation * Muliti_Importance_sample(srec.scatter_ray, depth, emitted_weight, is_shadow) / p_RR;
    }
    if (srec.is_refract) {
        return srec.attenuation * Muliti_Importance_sample(srec.scatter_ray, depth + 1, is_shadow) / p_RR;
    }
    //若光线没有追踪到光源，不是镜面反射，且是最后的shadow ray 则返回0
    if (is_shadow) {
        return color(0, 0, 0);
    }
    shared_ptr<hittable_pdf> light_pdf_ptr;
    vecf3 light_direction;
    ray shadow_ray, scatter_ray;
    double mis_brdf_sample, mis_light_sample, mis_tmp;
    double p_dir, p_indir, p_dir_tmp, p_indir_tmp;
    color direct_light(0, 0, 0), indirect_light(0, 0, 0);

    //直接光照
    light_pdf_ptr = make_shared<hittable_pdf>(scene.lights, rec.p);
    light_direction = unit_vector(light_pdf_ptr->generate());
    shadow_ray = ray(rec.p, light_direction, r.time());
    p_dir = light_pdf_ptr->value(shadow_ray.direction());
    //间接光照
    scatter_ray = ray(rec.p, unit_vector(srec.pdf_ptr->generate()), r.time());
    p_indir = srec.pdf_ptr->value(scatter_ray.direction());
    //启发式平衡函数，这正是MIS的权重函数
    p_dir_tmp = light_pdf_ptr->value(scatter_ray.direction());
    p_indir_tmp = srec.pdf_ptr->value(shadow_ray.direction());
    balance_heuristic(p_indir, p_dir_tmp, mis_brdf_sample, mis_tmp, 2);
    balance_heuristic(p_indir_tmp, p_dir, mis_tmp, mis_light_sample, 2);


    //处理一般渲染
    material_type m;
    rec.mat_ptr->getType(m);
    if (m != material_type::Isotropic) {
        if (p_dir) {
            direct_light = srec.attenuation * rec.mat_ptr->scattering_pdf(r, rec, shadow_ray)
                           * Muliti_Importance_sample(shadow_ray, depth + 1, mis_light_sample, true) / p_dir;
        }

        if (p_indir) {
            indirect_light = srec.attenuation * rec.mat_ptr->scattering_pdf(r, rec, scatter_ray)
                             * Muliti_Importance_sample(scatter_ray, depth + 1, mis_brdf_sample) / p_indir;
        }

    } else {
        //处理体渲染
        struct hit_record rec_lgt;
        if (!rec.boundary_ptr->hit(shadow_ray, 0.001, infinity, rec_lgt)) { // 在0-infinity范围内找到内表面位置
            if (p_dir)
                direct_light = srec.attenuation * rec.mat_ptr->scattering_pdf(r, rec, shadow_ray) *
                               Muliti_Importance_sample(shadow_ray, depth + 1, mis_light_sample, true)
                               / p_dir;
        } else {
//            shadow_ray = ray(rec_lgt.p+light_direction*0.001,light_direction,r.time());
//            double light_trans_distance = (rec_lgt.p-rec.p).length();
//            double light_attenuation = exp(-0.01*light_trans_distance);
//            if (p_dir)
//                direct_light =srec.attenuation *light_attenuation* rec.mat_ptr->scattering_pdf(r, rec, shadow_ray)*
//                              MIS_ray_color(shadow_ray, scene, depth + 1, mis_light_sample, true)
//                              / p_dir;
            direct_light = color(0, 0, 0);
        }

        if (p_indir) {
            indirect_light = srec.attenuation * rec.mat_ptr->scattering_pdf(r, rec, scatter_ray)
                             * Muliti_Importance_sample(scatter_ray, depth + 1, 1) / p_indir;
        }
    }


    result = emitted + (direct_light + indirect_light) / p_RR;
    return result;
}