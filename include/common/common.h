#ifndef RTWEEKEND_H
#define RTWEEKEND_H

#include <cmath>
#include <limits>
#include <memory>
#include <random>
#include <cmath>

//添加一些常用的变量、常数、函数和头文件

// Usings

using std::make_shared;
using std::shared_ptr;
using std::sqrt;
using std::abs;
// Constants

const double infinity = std::numeric_limits<double>::infinity();
const double pi = 3.1415926535897932385;

// Utility Functions

inline double degrees_to_radians(double degrees) {
    return degrees * pi / 180.0;
}

// Returns a random real in [0,1).
inline double random_double() {
    static std::uniform_real_distribution<double> distribution(0.0, 1.0);
    static std::mt19937 generator;
    return distribution(generator);
}
// Returns a random real in [min,max).
inline double random_double(double min, double max) {
    return min + (max - min) * random_double();
}

// Returns a random integer in [min,max].
inline int random_int(int min, int max) {
    return static_cast<int>(random_double(min, max + 1));
}

inline double clamp(double x, double min, double max) {
    if (x < min) return min;
    if (x > max) return max;
    return x;
}

// Common Headers

#include "vec3.h"
#include "ray.h"
#include "vec2.h"
#endif