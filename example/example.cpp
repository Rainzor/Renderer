#include "RenderEngine.h"
#include <iostream>
int main(){
   std::string img_name = "../output/img.png";
    Scene scene;
    test_scene(scene);
    RenderEngine rayTracer(scene);
    int spp = 16;
    SampleMethod sm = SampleMethod::Mixture;
    rayTracer.render(spp, sm, "../output/img.png", true);
    return 0;
}