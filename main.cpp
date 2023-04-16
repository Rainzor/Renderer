#include <iostream>
#include "color.h"
#include "vec3.h"
#include "ray.h"


double hit_sphere(const pointf3& center, float radius, const ray& r);

rgbf ray_color(const ray& r);


int main() {

    // Image
    const auto aspect_ratio = 16.0f / 9.0f;//长宽比
    const int image_width = 400;
    const int image_height = static_cast<int>(image_width / aspect_ratio);

   
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
            rgbf pixel_color = ray_color(r);
            write_color(std::cout, pixel_color);
        }
    }

    std::cerr << "\nDone.\n";
}

double hit_sphere(const pointf3& center, float radius, const ray& r) {
    vecf3 oc = r.origin() - center;//A-C
    auto a = dot(r.direction(), r.direction());//b*b
    auto b = 2.0 * dot(oc, r.direction());//2*b*(A-C)
    auto c = dot(oc, oc) - radius * radius;//(A-C)*(A-C)-r*r
    auto discriminant = b * b - 4 * a * c;//delta
    if (discriminant < 0) {
        return -1.0;
    } else {
        return (-b - sqrt(discriminant)) / (2.0 * a);//取方程中靠近相机的交点
    }
}

rgbf ray_color(const ray&r){
    auto t = hit_sphere(pointf3(0, 0, -1), 0.5, r);  // 球心，半径，光线
    if (t > 0.0) {
        vecf3 N = unit_vector(r.at(t) - vecf3(0, 0, -1));//球心，交点
        return 0.5 * rgbf(N.x() + 1, N.y() + 1, N.z() + 1);//归一化
    }
    // 背景颜色
    vecf3 unit_direction = unit_vector(r.direction());  // 单位化方向向量
    t = 0.5 * (unit_direction.y() + 1.0);          // y值在-1到1之间，重定义t在0到1之间
    return (1.0 - t) * rgbf(1.0, 1.0, 1.0) + t * rgbf(0.5, 0.7, 1.0);
}
