//

#include "RenderEngine.h"
#include <iostream>

int main(){
    std::string img_name = "../output/img.png";
    Scene scene;


//    std::cout<<"Scene Options:"<<std::endl;
//    std::cout<<"1. Cornell Box"<<std::endl;
//    std::cout<<"2. Cornell Box with Specular"<<std::endl;
//    std::cout<<"3. Cornell Box with Glass"<<std::endl;
//    std::cout<<"4. Cornell Box with Smoke"<<std::endl;
//    std::cout<<"5. Final Scene"<<std::endl;
//    std::cout<<"6. Test Scene"<<std::endl;
//    std::cout<<"Please choose a scene: ";
//    int choice;
//    std::cin>>choice;
//    std::cout<<std::endl;
    cornell_box(scene);
    RenderEngine rayTracer(scene);

//    std::cout<<std::endl<<"Render Methods:"<<std::endl;
//    std::cout<<"1. BRDF Sample"<<std::endl;
//    std::cout<<"2. Mixture Sample"<<std::endl;
//    std::cout<<"3. NEE Sample"<<std::endl;
//    std::cout<<"4. Multiple Importance Sample"<<std::endl;
//    std::cout<<"Please choose a Method: ";
//    int choice_method,samples_per_pixel;
//    std::cin>>choice_method;
//    std::cout<<"Input spp: ";
//    std::cin>>samples_per_pixel;
//    std::cout<<std::endl;

    rayTracer.render(8, SampleMethod::BRDF, img_name, false);
    return 0;
}