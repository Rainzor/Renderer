#include "rtweekend.h"

#include "color.h"
#include "hittable_list.h"
#include "sphere.h"

#include <iostream>

rgbf ray_color(const ray& r, const hittable& world);

int main() {

    // Image
    const auto aspect_ratio = 16.0f / 9.0f;//长宽比
    const int image_width = 400;
    const int image_height = static_cast<int>(image_width / aspect_ratio);

    // World
    hittable_list world;
    world.add(make_shared<sphere>(pointf3(0, 0, -1), 0.5)); //通过make_shared创建共享指针
    world.add(make_shared<sphere>(pointf3(0, -100.5, -1), 100));


    // Camera
    auto viewport_height = 2.0;
    auto viewport_width = aspect_ratio * viewport_height;
    auto focal_length = 1.0;//焦距
    
    auto origin = pointf3(0, 0, 0);
    auto horizontal = vecf3(viewport_width, 0, 0);//水平方向
    auto vertical = vecf3(0, viewport_height, 0);//垂直方向
    auto lower_left_corner = origin - horizontal/2 - vertical/2 - vecf3(0, 0, focal_length);//平面左下角
    // Render

    std::cout << "P3\n" << image_width << ' ' << image_height << "\n255\n";

    for (int j = image_height-1; j >= 0; --j) {
        std::cerr << "\rScanlines remaining: " << j << ' ' << std::flush;
        for (int i = 0; i < image_width; ++i) {
            auto u = float(i) / (image_width-1);
            auto v = float(j) / (image_height-1);
            //direction = 左下角 + u*宽 + v*高 - 起始点
            ray r(origin, lower_left_corner + u*horizontal + v*vertical - origin);
            rgbf pixel_color = ray_color(r,world);
            write_color(std::cout, pixel_color);
        }
    }

    std::cerr << "\nDone.\n";
}

rgbf ray_color(const ray&r, const hittable& world) {
    struct hit_record rec;
    if(world.hit(r, 0, infinity, rec)) {
        return 0.5 * (rec.normal + rgbf(1, 1, 1));
    }
    // 背景颜色
    vecf3 unit_direction = unit_vector(r.direction());  // 单位化方向向量
    auto t = 0.5 * (unit_direction.y() + 1.0);          // y值在-1到1之间，重定义t在0到1之间
    return (1.0 - t) * rgbf(1.0, 1.0, 1.0) + t * rgbf(0.5, 0.7, 1.0);
}
