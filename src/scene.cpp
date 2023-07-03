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

void cornell_triangle_glass(Scene &scene){
    scene.background = make_shared<solid_color>(0,0,0);

    //WORLD
    hittable_list objects;
    auto red = make_shared<lambertian>(color(.65, .05, .05));
    auto white = make_shared<lambertian>(color(.73, .73, .73));
    auto blue = make_shared<lambertian>(color(0.75, 0.82, 0.94));
    auto green = make_shared<lambertian>(color(.12, .45, .15));
    auto light = make_shared<diffuse_light>(color(9, 9, 9));

    objects.add(make_shared<yz_rect>(0, 555, 0, 555, 555, green));
    objects.add(make_shared<yz_rect>(0, 555, 0, 555, 0, red));
    objects.add(make_shared<flip_face>(make_shared<xz_rect>(163, 393, 177, 382, 554, light)));
    objects.add(make_shared<xz_rect>(0, 555, 0, 555, 0, white));
    objects.add(make_shared<xz_rect>(0, 555, 0, 555, 555, white));
    objects.add(make_shared<xy_rect>(0, 555, 0, 555, 555, white));
    //镜面材质
    pointf3 A(0, 0, 0);
    pointf3 B(225, 0, 0);
    pointf3 C(225, 0, 225*sqrt(3) / 2);
    pointf3 D(225, 460*sqrt(2.0 / 3.0), 0);

    std::vector<pointf3> vertices{A,B,C,D};
    std::vector<int> faces{0,1,2,0,2,3,0,3,1,1,3,2};
    shared_ptr<material> aluminum = make_shared<metal>(color(0.8, 0.85, 0.88), 0);
    shared_ptr<hittable> mesh1 = make_shared<mesh_triangle>(vertices,faces,aluminum);
    float rotate_angle = 28;
    vecf3 translation_vector(395, 188, 440);
    mesh1 = make_shared<rotate>(mesh1, rotate_angle,Axis::Y);
    mesh1 = make_shared<translate>(mesh1, translation_vector);
    objects.add(mesh1);
    scene.world = objects;

    //玻璃球
    auto glass = make_shared<dielectric>(1.5);
    objects.add(make_shared<sphere>(pointf3(190, 240, 125), 75, glass));
    scene.world = objects;
    //粗糙材质
    shared_ptr<hittable> box2 = make_shared<box>(pointf3(0, 0, 0), pointf3(165, 165, 165), white);
    box2 = make_shared<rotate>(box2, -18,Axis::Y);
    box2 = make_shared<translate>(box2, vecf3(130, 0, 65));
    objects.add(box2);
    scene.world = objects;
    //LIGHTS
    scene.lights = make_shared<hittable_list>();
    scene.lights->add(make_shared<xz_rect>(163, 393, 177, 382, 554,  shared_ptr<material>()));

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

void cornell_mesh_objects(Scene &scene){
    //BACKGROUND
    scene.background = make_shared<solid_color>(0,0,0);

    //WORLD
    hittable_list objects;
    auto red = make_shared<lambertian>(color(.65, .05, .05));
    auto white = make_shared<lambertian>(color(.73, .73, .73));
    auto blue = make_shared<lambertian>(color(0.2, 0.4, 0.9));
    auto green = make_shared<lambertian>(color(.12, .45, .15));
    auto light = make_shared<diffuse_light>(color(7, 7, 7));

    objects.add(make_shared<yz_rect>(0, 555, 0, 555, 555, green));
    objects.add(make_shared<yz_rect>(0, 555, 0, 555, 0, red));
    objects.add(make_shared<flip_face>(make_shared<xz_rect>(113, 443, 127, 432, 554, light)));
    objects.add(make_shared<xz_rect>(0, 555, 0, 555, 0, white));
    objects.add(make_shared<xz_rect>(0, 555, 0, 555, 555, white));
    objects.add(make_shared<xy_rect>(0, 555, 0, 555, 555, white));
    //粗糙材质

    std::string obj_filename1 = "../data/David328.obj";
    shared_ptr<hittable> mesh1 = make_shared<mesh_triangle>(obj_filename1,white,30);
    mesh1 = make_shared<rotate>(mesh1, -90,Axis::X);
    mesh1 = make_shared<rotate>(mesh1, 15,Axis::Y);
    vecf3 translation_vector1(375, 130, 325);
    mesh1 = make_shared<translate>(mesh1, translation_vector1);
    objects.add(mesh1);

    std::string obj_filename2 = "../data/Bunny_head.obj";
    shared_ptr<hittable> mesh2 = make_shared<mesh_triangle>(obj_filename2,white,2500);
    mesh2 = make_shared<rotate>(mesh2, -180,Axis::Y);
    vecf3 translation_vector2(120, 120, 235);
    mesh2 = make_shared<translate>(mesh2, translation_vector2);
    objects.add(mesh2);
    scene.world = objects;


    //LIGHTS
    scene.lights = make_shared<hittable_list>();
    scene.lights->add(make_shared<xz_rect>(113, 443, 127, 432, 554, shared_ptr<material>()));

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
    scene.lights->add(make_shared<xz_rect>(113, 443, 127, 432, 554, shared_ptr<material>()));

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

void cornell_mitsuba(Scene &scene){
//BACKGROUND
    scene.background = make_shared<solid_color>(0,0,0);

    //WORLD
    hittable_list objects;
    auto red = make_shared<lambertian>(color(.65, .05, .05));
    auto white = make_shared<lambertian>(color(.73, .73, .73));
    auto blue = make_shared<lambertian>(color(0.2, 0.4, 0.9));
    auto green = make_shared<lambertian>(color(.12, .45, .15));
    auto light = make_shared<diffuse_light>(color(7, 7, 7));

    objects.add(make_shared<yz_rect>(0, 555, 0, 555, 555, green));
    objects.add(make_shared<yz_rect>(0, 555, 0, 555, 0, red));
    objects.add(make_shared<flip_face>(make_shared<xz_rect>(113, 443, 127, 432, 554, light)));
    objects.add(make_shared<xz_rect>(0, 555, 0, 555, 0, white));
    objects.add(make_shared<xz_rect>(0, 555, 0, 555, 555, white));
    objects.add(make_shared<xy_rect>(0, 555, 0, 555, 555, white));



    std::string obj_filename1 = "../data/mitsuba1.obj";
    shared_ptr<hittable> mesh1 = make_shared<mesh_triangle>(obj_filename1,white,110);
    std::string obj_filename2 = "../data/mitsuba2.obj";
    shared_ptr<hittable> mesh2 = make_shared<mesh_triangle>(obj_filename2,white,110);
    std::vector<shared_ptr<hittable>> mitusba_meshes;
    mitusba_meshes.push_back(mesh1);
    mitusba_meshes.push_back(mesh2);
    shared_ptr<hittable> mitsuba = make_shared<hittable_list>(mitusba_meshes);


    mitsuba = make_shared<rotate>(mitsuba, -180,Axis::Y);
    vecf3 translation_vector2(277, 121, 310);
    mitsuba = make_shared<translate>(mitsuba, translation_vector2);
    objects.add(mitsuba);

    scene.world = objects;


    //LIGHTS
    scene.lights = make_shared<hittable_list>();
    scene.lights->add(make_shared<xz_rect>(113, 443, 127, 432, 554, shared_ptr<material>()));

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

void cornell_zoom(Scene &scene){
    //BACKGROUND
    scene.background = make_shared<solid_color>(0.1,0.1,0.1);

    //WORLD
    hittable_list objects;
    shared_ptr<material> aluminum = make_shared<metal>(color(0.5, 0.5, 0.5), 0.0);

    auto red = make_shared<lambertian>(color(.65, .05, .05));
    auto white = make_shared<lambertian>(color(.73, .73, .73));
    auto blue = make_shared<lambertian>(color(0.2, 0.4, 0.9));
    auto green = make_shared<lambertian>(color(.12, .45, .15));
    auto black = make_shared<lambertian>(color(0.1,0.1,0.1));
    auto light = make_shared<diffuse_light>(color(12, 12, 12));

    objects.add(make_shared<yz_rect>(0, 555, 0, 555, 555, green));
    objects.add(make_shared<yz_rect>(0, 555, 0, 555, 0, red));
    objects.add(make_shared<flip_face>(make_shared<xz_rect>(153, 403, 197, 392, 554, light)));
    objects.add(make_shared<xz_rect>(0, 555, 0, 555, 0, white));
    objects.add(make_shared<xz_rect>(0, 555, 0, 555, 555, white));
    objects.add(make_shared<xy_rect>(0, 555, 0, 555, 555, white));

    std::string obj_filename1 = "../data/spot/spot.obj";
    auto texture1 = make_shared<lambertian>(make_shared<image_texture>("../data/spot/spot_texture.png"));

    shared_ptr<hittable> spot = make_shared<mesh_triangle>(obj_filename1,texture1,150);
    spot = make_shared<rotate>(spot, 30,Axis::Y);
    vecf3 translation_vector1(405, 155, 270);
    spot = make_shared<translate>(spot, translation_vector1);
    objects.add(spot);
    std::string obj_filename2 = "../data/stanford_bunny/bunny.obj";
    auto texture2 = make_shared<lambertian>(make_shared<image_texture>("../data/stanford_bunny/bunny.jpg"));
    shared_ptr<hittable> bunny = make_shared<mesh_triangle>(obj_filename2,texture2,1200);
    bunny = make_shared<rotate>(bunny, -180,Axis::Y);
    vecf3 translation_vector2(170, 171, 255);
    bunny = make_shared<translate>(bunny, translation_vector2);
    objects.add(bunny);

    //底座
    shared_ptr<hittable> box1 = make_shared<box>(pointf3(-100, 0, -128), pointf3(90, 30, 128), white);
//    shared_ptr<hittable> box1_cover = make_shared<box>(pointf3(-100, 0, -128), pointf3(90, 28, 128), black);
//    std::vector<shared_ptr<hittable>> box1s;
//    shared_ptr<hittable> box1 = make_shared<hittable_list>(box1s);
    box1 = make_shared<translate>(box1, vecf3(415, 0, 285));
    objects.add(box1);
    shared_ptr<hittable> box2 = make_shared<box>(pointf3(-125, 0, -95), pointf3(125, 58, 95), white);
    box2 = make_shared<translate>(box2, vecf3(170, 0, 250));
    objects.add(box2);
    scene.world = objects;

    //LIGHTS
    scene.lights = make_shared<hittable_list>();
    scene.lights->add(make_shared<xz_rect>(153, 403, 197, 392, 554, shared_ptr<material>()));

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

void test_scene(Scene & scene){
    //BACKGROUND
    scene.background = make_shared<solid_color>(0,0,0);

    //WORLD
    hittable_list objects;
    shared_ptr<material> aluminum = make_shared<metal>(color(0.8, 0.85, 0.88), 0.0);

    auto red = make_shared<lambertian>(color(.65, .05, .05));
    auto white = make_shared<lambertian>(color(.73, .73, .73));
    auto blue = make_shared<lambertian>(color(0.2, 0.4, 0.9));
    auto green = make_shared<lambertian>(color(.12, .45, .15));
    auto light = make_shared<diffuse_light>(color(12, 12, 12));

    objects.add(make_shared<yz_rect>(0, 555, 0, 555, 555, green));
    objects.add(make_shared<yz_rect>(0, 555, 0, 555, 0, red));
    objects.add(make_shared<flip_face>(make_shared<xz_rect>(153, 403, 197, 392, 554, light)));
    objects.add(make_shared<xz_rect>(0, 555, 0, 555, 0, white));
    objects.add(make_shared<xz_rect>(0, 555, 0, 555, 555, white));
    objects.add(make_shared<xy_rect>(0, 555, 0, 555, 555, white));

    std::string obj_filename1 = "../data/spot/spot.obj";
    auto texture1 = make_shared<lambertian>(make_shared<image_texture>("../data/spot/spot_texture.png"));

    shared_ptr<hittable> spot = make_shared<mesh_triangle>(obj_filename1,texture1,150);
    spot = make_shared<rotate>(spot, 30,Axis::Y);
    vecf3 translation_vector1(405, 155, 270);
    spot = make_shared<translate>(spot, translation_vector1);
    objects.add(spot);
    std::string obj_filename2 = "../data/stanford_bunny/bunny.obj";
    auto texture2 = make_shared<lambertian>(make_shared<image_texture>("../data/stanford_bunny/bunny.jpg"));
    shared_ptr<hittable> bunny = make_shared<mesh_triangle>(obj_filename2,texture2,1200);
    bunny = make_shared<rotate>(bunny, -180,Axis::Y);
    vecf3 translation_vector2(170, 171, 255);
    bunny = make_shared<translate>(bunny, translation_vector2);
    objects.add(bunny);

    //底座
    shared_ptr<hittable> box1 = make_shared<box>(pointf3(-100, 0, -128), pointf3(90, 30, 128), white);
    box1 = make_shared<translate>(box1, vecf3(415, 0, 285));
    objects.add(box1);
    shared_ptr<hittable> box2 = make_shared<box>(pointf3(-125, 0, -95), pointf3(125, 58, 95), aluminum);
    box2 = make_shared<translate>(box2, vecf3(170, 0, 250));
    objects.add(box2);
    scene.world = objects;


    //LIGHTS
    scene.lights = make_shared<hittable_list>();
    scene.lights->add(make_shared<xz_rect>(153, 403, 197, 392, 554, shared_ptr<material>()));

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