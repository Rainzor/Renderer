#ifndef VEC3_H
#define VEC3_H
#include "common.h"
#include <cmath>
#include <iostream>
using std::sqrt;

template <typename T>
class vec3 {
    public:
        vec3() : e{T(0), T(0), T(0)} {}
        vec3(T e0, T e1, T e2) : e{e0, e1, e2} {}

        T x() const { return e[0]; }
        T y() const { return e[1]; }
        T z() const { return e[2]; }

        vec3 operator-() const { return vec3(-e[0], -e[1], -e[2]); }
        T operator[](int i) const { return e[i]; }
        T& operator[](int i) { return e[i]; }

        vec3& operator+=(const vec3 &v) {
            e[0] += v.e[0];
            e[1] += v.e[1];
            e[2] += v.e[2];
            return *this;
        }

        vec3& operator-=(const vec3 &v) {
            e[0] -= v.e[0];
            e[1] -= v.e[1];
            e[2] -= v.e[2];
            return *this;
        }

        vec3& operator*=(const T t) {
            e[0] *= t;
            e[1] *= t;
            e[2] *= t;
            return *this;
        }

        vec3& operator/=(const T t) {
            return *this *= T(1)/t;
        }

        T length() const {
            return sqrt(length_squared());
        }

        T length_squared() const {
            return e[0]*e[0] + e[1]*e[1] + e[2]*e[2];
        }

        inline static vec3 random() {
            T x = static_cast<T>(random_double());
            T y = static_cast<T>(random_double());
            T z = static_cast<T>(random_double());
            return vec3(x, y, z);
        }
        inline static vec3 random(T min, T max) {
            T x = static_cast<T>(random_double(min, max));
            T y = static_cast<T>(random_double(min, max));
            T z = static_cast<T>(random_double(min, max));
            return vec3(x, y, z);
        }
        bool near_zero() const {
            // Return true if the vector is close to zero in all dimensions.
            const auto s = 1e-8;
            return (abs(e[0]) < s) && (abs(e[1]) < s) && (abs(e[2]) < s);
        }

    public:
        T e[3];
};
// vec3 Utility Functions

template <typename T>
inline std::ostream& operator<<(std::ostream &out, const vec3<T> &v) {
    return out << v.e[0] << ' ' << v.e[1] << ' ' << v.e[2];
}

template <typename T>
inline vec3<T> operator+(const vec3<T> &u, const vec3<T> &v) {
    return vec3<T>(u.e[0] + v.e[0], u.e[1] + v.e[1], u.e[2] + v.e[2]);
}

template <typename T>
inline vec3<T> operator-(const vec3<T> &u, const vec3<T> &v) {
    return vec3<T>(u.e[0] - v.e[0], u.e[1] - v.e[1], u.e[2] - v.e[2]);
}

template <typename T>
inline vec3<T> operator*(const vec3<T> &u, const vec3<T> &v) {
    return vec3<T>(u.e[0] * v.e[0], u.e[1] * v.e[1], u.e[2] * v.e[2]);
}


template <typename T>
inline vec3<T> operator*(T t, const vec3<T> &v) {
    return vec3<T>(t*v.e[0], t*v.e[1], t*v.e[2]);
}

template <typename T>
inline vec3<T> operator*(const vec3<T> &v, T t) {
    return vec3<T>(t*v.e[0], t*v.e[1], t*v.e[2]);
}

template <typename T, typename U>
inline vec3<T> operator*(U t, const vec3<T>& v) {
    return v * static_cast<T>(t);
}

template <typename T, typename U>
inline vec3<T> operator*(const vec3<T>& v, U t) {
    return vec3<T>(static_cast<T>(t) * v.e[0], static_cast<T>(t) * v.e[1], static_cast<T>(t) * v.e[2]);
}

template <typename T>
inline vec3<T> operator/(vec3<T> v, T t) {
    return (T(1)/t) * v;
}

template <typename T, typename U>
inline vec3<T> operator/(vec3<T> v, U t) {
    return static_cast<T>(1.0 / t) * v;
}

template <typename T>
inline T dot(const vec3<T> &u, const vec3<T> &v) {
return u.e[0] * v.e[0]
        + u.e[1] * v.e[1]
        + u.e[2] * v.e[2];
}

template <typename T>
inline vec3<T> cross(const vec3<T> &u, const vec3<T> &v) {
    return vec3<T>(u.e[1] * v.e[2] - u.e[2] * v.e[1],
                    u.e[2] * v.e[0] - u.e[0] * v.e[2],
                    u.e[0] * v.e[1] - u.e[1] * v.e[0]);
}

template <typename T>
inline vec3<T> unit_vector(vec3<T> v) {
    return v / v.length();
}

// 特化版本
//  针对 int 类型的特化版本
// template <>
// inline vec3<int> vec3<int>::random() {
//     int x = random_double()>0.5?1:0;
//     int y = random_double()>0.5?1:0;
//     int z = random_double()>0.5?1:0;
//     return vec3<int>(x, y, z);
// }

// Type aliases for vec3
template <typename T>
using point3 = vec3<T>;   // 3D point

using vecf3 = vec3<float>; // float type
using pointf3 = point3<float>;
using color = vecf3;  // RGB color
using rgbf = color;
using vecd3 = vec3<double>; // double type
using pointd3 = point3<double>;


inline vecf3 reflect(const vecf3& v, const vecf3& n) {//反射
    return v - 2 * dot(v, n) * n;
}
inline vecf3 refract(const vecf3& uv, const vecf3& n, float etai_over_etat) {//折射
    //折射
    auto cos_theta = fmin(dot(-uv, n), 1.0);
    vecf3 r_out_perp = etai_over_etat * (uv + cos_theta * n);
    vecf3 r_out_parallel = -sqrt(fabs(1.0 - r_out_perp.length_squared())) * n;
    return r_out_perp + r_out_parallel;
}
#endif // VEC3_H