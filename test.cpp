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
    std::vector<int> spps{2,4,8, 16,32,64};
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

////
//#include <iostream>
//#include <fstream>
//#include <string>
//#include <sstream>
//
//int main() {
//    std::string inputFileName = "../data/mitsuba.obj";
//    std::string outputFileName1 = "../data/mitsuba1.obj";
//    std::string outputFileName2 = "../data/mitsuba2.obj";
//
//    std::ifstream inputFile(inputFileName);
//    std::ofstream outputFile1(outputFileName1);
//    std::ofstream outputFile2(outputFileName2);
//
//    if (!inputFile.is_open()) {
//        std::cerr << "Failed to open input file: " << inputFileName << std::endl;
//        return 1;
//    }
//
//    if (!outputFile1.is_open()) {
//        std::cerr << "Failed to open output file: " << outputFileName1 << std::endl;
//        return 1;
//    }
//
//    if (!outputFile2.is_open()) {
//        std::cerr << "Failed to open output file: " << outputFileName2 << std::endl;
//        return 1;
//    }
//
//    std::string line;
//    bool firstObject = true;
//
//    while (std::getline(inputFile, line)) {
//        if (line.compare(0, 8, "# object") == 0) {
//            firstObject = !firstObject;
//            continue;
//        }
//
//        if (firstObject) {
//            outputFile1 << line << std::endl;
//        } else {
//            outputFile2 << line << std::endl;
//        }
//    }
//
//    inputFile.close();
//    outputFile1.close();
//    outputFile2.close();
//
//    std::cout << "Successfully split the input file into " << outputFileName1 << " and " << outputFileName2 << std::endl;
//
//    return 0;
//}