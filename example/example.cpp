#include "RenderEngine.h"
#include <iostream>
int main(){
   std::string img_name = "../output/img.png";
    Scene scene;
    cornell_zoom(scene);
    RenderEngine rayTracer(scene);
    int spp = 8;
    SampleMethod sm = SampleMethod::Mixture;
    rayTracer.render(spp, sm, "../output/img.png", true);
    return 0;
}