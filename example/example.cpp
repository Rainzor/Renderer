#include "RenderEngine.h"
#include <iostream>
int main(){
   std::string img_name = "../output/img.png";
    Scene scene;
    cornell_box(scene);
    RenderEngine rayTracer(scene);
    int spp = 8;
    SampleMethod sm = SampleMethod::MIS;
    rayTracer.render(spp, sm, img_name, true);
    return 0;
}