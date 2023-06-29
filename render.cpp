//

#include "RayTracer.h"
#include <iostream>

int main(){
    std::string img_name = "./output/img.png";
    Scene scene;
    final_scene(scene);
    RayTracer rayTracer(scene);
    rayTracer.render(32, SampleMethod::MIS, img_name);
    return 0;
}