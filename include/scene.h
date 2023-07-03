//
// Created by Runze on 29/06/2023.
//

#ifndef RAYTRACER_SCENE_H
#define RAYTRACER_SCENE_H
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
#include "mesh_triangle.h"

typedef struct Scene{
    shared_ptr<texture> background;
    hittable_list world;
    shared_ptr<hittable_list> lights;
    shared_ptr<camera> cam;
    int width;
    int height;
} Scene;


void cornell_box(Scene& scene);

void cornell_specular(Scene& scene);

void cornell_triangle_glass(Scene &scene);

void cornell_smoke(Scene &scene);

void cornell_mitsuba(Scene &scene);
void cornell_mesh_objects(Scene &scene);

void cornell_zoom(Scene &scene);
void final_scene(Scene &scene);

void test_scene(Scene & scene);
#endif //RAYTRACER_SCENE_H
