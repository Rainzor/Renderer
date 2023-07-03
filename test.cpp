#include "RenderEngine.h"
#include <iostream>
int main(){
    std::string img_name = "../output/img.png";
    Scene scene;
    cornell_zoom(scene);
    RenderEngine rayTracer(scene);
    int spp = 256;
    SampleMethod sm = SampleMethod::MIS;
    rayTracer.render(spp, sm, img_name, true);
    return 0;
}