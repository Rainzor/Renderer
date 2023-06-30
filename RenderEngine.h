#ifndef RENDER_RENDERENGINE_H
#define RENDER_RENDERENGINE_H
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
#include "triangle.h"
#include "participate_medium.h"
#include "scene.h"
//#include <QObject>
#define NUM_THREADS 8 // 线程数

enum class SampleMethod {
    BRDF = 1,
    Mixture = 2,
    Light = 3,
    NEE = 4,
    MIS = 5
};

class RenderEngine{
public:
    RenderEngine(Scene scene, int width, int height):
            scene(scene), width(width), height(height) {}
    RenderEngine(Scene scene):
            scene(scene), width(scene.width), height(scene.height) {}
    RenderEngine() {}
    void ChangeScene(const Scene& s) {
        this->scene = s;
        this->width = s.width;
        this->height = s.height;
    }
    ~RenderEngine() {}
    void render(int spp=16, SampleMethod method = SampleMethod::BRDF,const std::string img_name="./output/img.png",bool isOpenMP=true) const;

private:
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
    int width;
    int height;
};

#endif //RENDER_RENDERENGINE_H
