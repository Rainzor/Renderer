#include "scene.h"
void cornell_box(Scene& scene){
    //BACKGROUND
    scene.background = make_shared<solid_color>(color(0, 0, 0)) ;

    //WORLD
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
    box1 = make_shared<rotate>(box1, 15,Axis::Y);
    box1 = make_shared<translate>(box1, vecf3(265, 0, 295));
    objects.add(box1);

    shared_ptr<hittable> box2 = make_shared<box>(pointf3(0, 0, 0), pointf3(165, 165, 165), white);
    box2 = make_shared<rotate>(box2, -18,Axis::Y);
    box2 = make_shared<translate>(box2, vecf3(130, 0, 65));
    objects.add(box2);
    scene.world = objects;

    //LIGHTS
    scene.lights = make_shared<hittable_list>();
    scene.lights->add(make_shared<xz_rect>(213, 343, 227, 332, 554, shared_ptr<material>()));

    //CAMERA
    double aspect_ratio = 1.0;
    int image_width = 600;
    pointf3 lookfrom(278, 278, -800);
    pointf3 lookat(278, 278, 0);
    double vfov = 40.0;
    double aperture = 0.0;
    vecf3 vup(0, 1, 0);
    auto dist_to_focus = 10.0;
    int image_height = static_cast<int>(image_width / aspect_ratio);
    scene.cam = make_shared<camera>(lookfrom, lookat, vup, vfov, aspect_ratio, aperture, dist_to_focus, 0.0, 1.0);
    scene.width = image_width;
    scene.height = image_height;
}

void cornell_specular(Scene& scene){
    scene.background = make_shared<solid_color>(color(0, 0, 0)) ;

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
    box1 = make_shared<rotate>(box1, 15,Axis::Y);
    box1 = make_shared<translate>(box1, vecf3(265, 0, 295));
    objects.add(box1);

    //粗糙材质
    shared_ptr<hittable> box2 = make_shared<box>(pointf3(0, 0, 0), pointf3(165, 165, 165), white);
    box2 = make_shared<rotate>(box2, -18,Axis::Y);
    box2 = make_shared<translate>(box2, vecf3(130, 0, 65));
    objects.add(box2);
    scene.world = objects;

    //LIGHTS
    scene.lights = make_shared<hittable_list>();
    scene.lights->add(make_shared<xz_rect>(213, 343, 227, 332, 554, shared_ptr<material>()));

    //CAMERA
    double aspect_ratio = 1.0;
    int image_width = 600;
    pointf3 lookfrom(278, 278, -800);
    pointf3 lookat(278, 278, 0);
    double vfov = 40.0;
    double aperture = 0.0;
    vecf3 vup(0, 1, 0);
    auto dist_to_focus = 10.0;
    int image_height = static_cast<int>(image_width / aspect_ratio);
    scene.cam = make_shared<camera>(lookfrom, lookat, vup, vfov, aspect_ratio, aperture, dist_to_focus, 0.0, 1.0);
    scene.width = image_width;
    scene.height = image_height;

}

void cornell_glass(Scene &scene){
    scene.background = make_shared<solid_color>(color(0, 0, 0)) ;

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
    box1 = make_shared<rotate>(box1, 15,Axis::Y);
    box1 = make_shared<translate>(box1, vecf3(265, 0, 295));
    objects.add(box1);
    //添加一个玻璃球
    auto glass = make_shared<dielectric>(1.5);
    objects.add(make_shared<sphere>(pointf3(190, 90, 190), 90, glass));
    scene.world = objects;

    //LIGHTS
    scene.lights = make_shared<hittable_list>();
    scene.lights->add(make_shared<xz_rect>(213, 343, 227, 332, 554, shared_ptr<material>()));
    //加入一个球面光源
    scene.lights->add(make_shared<sphere>(pointf3(190, 90, 190), 90, shared_ptr<material>()));

    //CAMERA
    double aspect_ratio = 1.0;
    int image_width = 600;
    pointf3 lookfrom(278, 278, -800);
    pointf3 lookat(278, 278, 0);
    double vfov = 40.0;
    double aperture = 0.0;
    vecf3 vup(0, 1, 0);
    auto dist_to_focus = 10.0;
    int image_height = static_cast<int>(image_width / aspect_ratio);
    scene.cam = make_shared<camera>(lookfrom, lookat, vup, vfov, aspect_ratio, aperture, dist_to_focus, 0.0, 1.0);
    scene.width = image_width;
    scene.height = image_height;
}
void cornell_triangle(Scene &scene){
    //BACKGROUND
    scene.background = make_shared<solid_color>(0,0,0);

    //WORLD
    hittable_list objects;
    auto red = make_shared<lambertian>(color(.65, .05, .05));
    auto white = make_shared<lambertian>(color(.73, .73, .73));
    auto blue = make_shared<lambertian>(color(0.2, 0.4, 0.9));
    auto green = make_shared<lambertian>(color(.12, .45, .15));
    auto light = make_shared<diffuse_light>(color(15, 15, 15));

    objects.add(make_shared<yz_rect>(0, 555, 0, 555, 555, green));
    objects.add(make_shared<yz_rect>(0, 555, 0, 555, 0, red));
    objects.add(make_shared<flip_face>(make_shared<xz_rect>(213, 343, 227, 332, 554, light)));
    objects.add(make_shared<xz_rect>(0, 555, 0, 555, 0, white));
    objects.add(make_shared<xz_rect>(0, 555, 0, 555, 555, white));
    objects.add(make_shared<xy_rect>(0, 555, 0, 555, 555, white));
    //粗糙材质

    shared_ptr<hittable> box1 = make_shared<box>(pointf3(0, 0, 0), pointf3(165, 165, 165), white);
    box1 = make_shared<rotate>(box1, -18,Axis::Y);
    box1 = make_shared<translate>(box1, vecf3(130, 0, 65));
    objects.add(box1);

    pointf3 A(0, 0, 0);
    pointf3 B(200, 0, 0);
    pointf3 C(200, 0, 200*sqrt(3) / 2);
    pointf3 D(200, 400*sqrt(2.0 / 3.0), 200*sqrt(3) / 6);
    shared_ptr<material> aluminum = make_shared<metal>(color(0.8, 0.85, 0.88), 0);

    shared_ptr<hittable> tetrahedron_face1 = make_shared<triangle>(A, C, B, white);
    shared_ptr<hittable> tetrahedron_face2 = make_shared<triangle>(A, D, B, aluminum);
    shared_ptr<hittable> tetrahedron_face3 = make_shared<triangle>(A, C, D, white);
    shared_ptr<hittable> tetrahedron_face4 = make_shared<triangle>(B, D, C, white);
    float rotate_angle = 30;
    vecf3 translation_vector(265, 0, 255);

    shared_ptr<hittable> tetrahedron_face1_transformed = make_shared<rotate>(tetrahedron_face1, rotate_angle,Axis::Y);
//    tetrahedron_face1_transformed = make_shared<rotate>(tetrahedron_face1_transformed, 45,Axis::X);
//    shared_ptr<hittable> tetrahedron_face1_transformed = make_shared<rotate_y>(tetrahedron_face1,rotate_angle);
    tetrahedron_face1_transformed = make_shared<translate>(tetrahedron_face1_transformed, translation_vector);

    shared_ptr<hittable> tetrahedron_face2_transformed = make_shared<rotate>(tetrahedron_face2, rotate_angle,Axis::Y);
//    tetrahedron_face2_transformed = make_shared<rotate>(tetrahedron_face2_transformed, 45,Axis::X);
//    shared_ptr<hittable> tetrahedron_face2_transformed = make_shared<rotate_y>(tetrahedron_face2,rotate_angle);

    tetrahedron_face2_transformed = make_shared<translate>(tetrahedron_face2_transformed, translation_vector);

    shared_ptr<hittable> tetrahedron_face3_transformed = make_shared<rotate>(tetrahedron_face3, rotate_angle,Axis::Y);
//    tetrahedron_face3_transformed = make_shared<rotate>(tetrahedron_face3_transformed, 45,Axis::X);
//    shared_ptr<hittable> tetrahedron_face3_transformed = make_shared<rotate_y>(tetrahedron_face3,rotate_angle);
    tetrahedron_face3_transformed = make_shared<translate>(tetrahedron_face3_transformed, translation_vector);

    shared_ptr<hittable> tetrahedron_face4_transformed = make_shared<rotate>(tetrahedron_face4, rotate_angle,Axis::Y);
//    tetrahedron_face4_transformed = make_shared<rotate>(tetrahedron_face4_transformed, 45,Axis::X);
//    shared_ptr<hittable> tetrahedron_face4_transformed = make_shared<rotate_y>(tetrahedron_face4,rotate_angle);
    tetrahedron_face4_transformed = make_shared<translate>(tetrahedron_face4_transformed, translation_vector);
    objects.add(tetrahedron_face1_transformed);
    objects.add(tetrahedron_face2_transformed);
    objects.add(tetrahedron_face3_transformed);
    objects.add(tetrahedron_face4_transformed);

//    shared_ptr<hittable> trian = make_shared<triangle>(pointf3(0,0,0),pointf3(100,0,50),pointf3(40,50,0),blue);
//    trian = make_shared<rotate_y>(trian, 15);
//    trian = make_shared<translate>(trian, vecf3(265, 300, 395));
//    objects.add(trian);

    scene.world = objects;

    //LIGHTS
    scene.lights = make_shared<hittable_list>();
    scene.lights->add(make_shared<xz_rect>(213, 343, 227, 332, 554, shared_ptr<material>()));

    //CAMERA
    double aspect_ratio = 1.0;
    int image_width = 600;
    pointf3 lookfrom(278, 278, -800);
    pointf3 lookat(278, 278, 0);
    double vfov = 40.0;
    double aperture = 0.0;
    vecf3 vup(0, 1, 0);
    auto dist_to_focus = 10.0;
    int image_height = static_cast<int>(image_width / aspect_ratio);
    scene.cam = make_shared<camera>(lookfrom, lookat, vup, vfov, aspect_ratio, aperture, dist_to_focus, 0.0, 1.0);
    scene.width = image_width;
    scene.height = image_height;
}
void cornell_smoke(Scene &scene){
    scene.background = make_shared<solid_color>(color(0, 0, 0)) ;

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
    box1 = make_shared<rotate>(box1, 15,Axis::Y);
    box1 = make_shared<translate>(box1, vecf3(265, 0, 295));

    shared_ptr<hittable> box2 = make_shared<box>(pointf3(0, 0, 0), pointf3(165, 165, 165), white);
    box2 = make_shared<rotate>(box2, -18,Axis::Y);
    box2 = make_shared<translate>(box2, vecf3(130, 0, 65));

    objects.add(make_shared<constant_medium>(box1, 0.01, color(0.1,0.1,0.1)));
    objects.add(make_shared<constant_medium>(box2, 0.01, color(1,1,1)));

    scene.world = objects;

    //LIGHTS
    scene.lights = make_shared<hittable_list>();
    scene.lights->add(make_shared<xz_rect>(213, 343, 227, 332, 554, shared_ptr<material>()));

    //CAMERA
    double aspect_ratio = 1.0;
    int image_width = 600;
    pointf3 lookfrom(278, 278, -800);
    pointf3 lookat(278, 278, 0);
    double vfov = 40.0;
    double aperture = 0.0;
    vecf3 vup(0, 1, 0);
    auto dist_to_focus = 10.0;
    int image_height = static_cast<int>(image_width / aspect_ratio);
    scene.cam = make_shared<camera>(lookfrom, lookat, vup, vfov, aspect_ratio, aperture, dist_to_focus, 0.0, 1.0);
    scene.width = image_width;
    scene.height = image_height;
}

void final_scene(Scene &scene){
    scene.background = make_shared<solid_color>(color(0, 0, 0)) ;

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

    auto emat = make_shared<lambertian>(make_shared<image_texture>("../img/earthmap.jpg"));
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
            make_shared<rotate>(
                    make_shared<bvh_node>(boxes2, 0.0, 1.0), 15,Axis::Y),
            vecf3(-100, 270, 395)));

    scene.world = objects;

    //LIGHTS
    scene.lights = make_shared<hittable_list>();
    scene.lights->add(make_shared<xz_rect>(213, 343, 227, 332, 554, shared_ptr<material>()));

    //CAMERA
    double aspect_ratio = 1.0;
    int image_width = 800;
    pointf3 lookfrom(478, 278, -600);
    pointf3 lookat(278, 278, 0);
    double vfov = 40.0;
    double aperture = 0.0;
    vecf3 vup(0, 1, 0);
    auto dist_to_focus = 10.0;
    int image_height = static_cast<int>(image_width / aspect_ratio);
    scene.cam = make_shared<camera>(lookfrom, lookat, vup, vfov, aspect_ratio, aperture, dist_to_focus, 0.0, 1.0);
    scene.width = image_width;
    scene.height = image_height;
}