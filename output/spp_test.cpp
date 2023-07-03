#include "RenderEngine.h"
#include <chrono>
#include <fstream>
#include <iostream>
#include <sstream>
#include <map>
int main(){
    Scene scene;
    std::string output_path = "../output/compare/";
    cornell_mesh_objects(scene);
    RenderEngine rayTracer(scene);
    std::vector<int> spps{2,4,8, 16};
//    std::vector<int> spps{256};

//    std::vector<SampleMethod> methods{SampleMethod::NEE};
    std::vector<SampleMethod> methods{SampleMethod::MIS};
    std::map<SampleMethod, std::map<int, double>> render_times;

    for (SampleMethod sm : methods) {
        for (int spp : spps) {
            std::stringstream ss;
//            ss <<output_path << static_cast<int>(sm) << "_" << spp << ".png";
            ss <<output_path << "img.png";
            std::string img_name = ss.str();

            auto start_time = std::chrono::high_resolution_clock::now();
            rayTracer.render(spp, sm, img_name, false);
            auto end_time = std::chrono::high_resolution_clock::now();
            std::chrono::duration<double> elapsed_seconds = end_time - start_time;

            render_times[sm][spp] = elapsed_seconds.count();
        }
    }

    std::ofstream outfile(output_path+"timing_results.csv");
// Write header
    outfile << "SampleMethod,SPP,Time" << std::endl;

    for (const auto& method_times : render_times) {
        SampleMethod sm = method_times.first;
        for (const auto& spp_time : method_times.second) {
            int spp = spp_time.first;
            double time = spp_time.second;
            outfile << static_cast<int>(sm) << "," << spp << "," << time << std::endl;
        }
    }
    outfile.close();
    return 0;
}