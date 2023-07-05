#ifndef RENDER_RENDERENGINE_H
#define RENDER_RENDERENGINE_H
#include <chrono>
#include <iostream>
#include <memory>
#include <omp.h> // OpenMP
#include <vector>
#include <functional>
#include <thread>
#include <atomic>
#include "scene.h"
#define NUM_THREADS  16// 线程数

enum class SampleMethod {
    BRDF = 0,
    Light = 1,
    Mixture = 2,
    NEE = 3,
    MIS = 4
};
//TODO: 0.DEBUG MIS,
//TODO: 1.重构 BRDF 和 glass材质
//TODO: 2.体渲染
//TODO: 3.递归光线追踪--->迭代光线追踪
class RenderEngine{
public:
    RenderEngine(Scene scene, int width, int height):
            scene(scene), width(width), height(height) {}
    explicit RenderEngine(Scene scene):
            scene(scene), width(scene.width), height(scene.height) {}
    RenderEngine() = default;

    ~RenderEngine() = default;
    void ChangeScene(const Scene& s) {
        this->scene = s;
        this->width = s.width;
        this->height = s.height;
    }
    void setProgressCallback(const std::function<void(int)> &callback) {
        progressCallback = callback;
    }
    void render(int spp=16, SampleMethod method = SampleMethod::BRDF,const std::string& img_name="./output/img.png",bool isOpenMP=true);

private:
    color computePixelColor(int i, int j, int spp, SampleMethod method)const;
    color ray_color(const ray &r,SampleMethod method)const;
    color BRDF_sample(const ray &r)const;
    color light_sample(const ray &r)const;
    color Mixture_sample(const ray &r)const;
    color NEE_sample(const ray &r,int depth,bool is_shadow= false)const;
    color Muliti_Importance_sample(const ray &r,int depth, double emitted_weight,bool is_shadow= false)const;

    void balance_heuristic(double f_pdf, double g_pdf, double &weight_f, double &weight_g,int beta=1)const{
        if(beta!=1) {
            f_pdf = pow(f_pdf, beta);
            g_pdf = pow(g_pdf, beta);
        }
        weight_f = f_pdf / (f_pdf + g_pdf);
        weight_g = g_pdf / (f_pdf + g_pdf);
    };
public:
    Scene scene;
    int width{};
    int height{};
    std::function<void(int)> progressCallback;
};

#endif //RENDER_RENDERENGINE_H
