#ifndef VEC2_H
#define VEC2_H
#include "common.h"
#include <cmath>
#include <iostream>
using std::sqrt;

template <typename T>
class vec2 {
public:
    vec2() : e{T(0), T(0)} {}
    vec2(T e0, T e1) : e{e0, e1} {}

    T x() const { return e[0]; }
    T y() const { return e[1]; }

    vec2 operator-() const { return vec2(-e[0], -e[1]); }
    T operator[](int i) const { return e[i]; }
    T& operator[](int i) { return e[i]; }

    vec2& operator+=(const vec2 &v) {
        e[0] += v.e[0];
        e[1] += v.e[1];
        return *this;
    }

    vec2& operator-=(const vec2 &v) {
        e[0] -= v.e[0];
        e[1] -= v.e[1];
        return *this;
    }

    vec2& operator*=(const T t) {
        e[0] *= t;
        e[1] *= t;
        return *this;
    }

    vec2& operator/=(const T t) {
        return *this *= T(1)/t;
    }

    bool operator==(const vec2 &v) const {
        return e[0] == v.e[0] && e[1] == v.e[1];
    }

    T length() const {
        return sqrt(length_squared());
    }

    T length_squared() const {
        return e[0]*e[0] + e[1]*e[1];
    }

    inline static vec2 random() {
        T x = static_cast<T>(random_double());
        T y = static_cast<T>(random_double());
        return vec2(x, y);
    }

    inline static vec2 random(T min, T max) {
        T x = static_cast<T>(random_double(min, max));
        T y = static_cast<T>(random_double(min, max));
        return vec2(x, y);
    }

    bool near_zero() const {
        // Return true if the vector is close to zero in all dimensions.
        const auto s = 1e-8;
        return (abs(e[0]) < s) && (abs(e[1]) < s);
    }

public:
    T e[2];
};

// vec2 Utility Functions

template <typename T>
inline std::ostream& operator<<(std::ostream &out, const vec2<T> &v) {
    return out << v.e[0] << ' ' << v.e[1];
}

template <typename T>
inline vec2<T> operator+(const vec2<T> &u, const vec2<T> &v) {
    return vec2<T>(u.e[0] + v.e[0], u.e[1] + v.e[1]);
}

template <typename T>
inline vec2<T> operator-(const vec2<T> &u, const vec2<T> &v) {
    return vec2<T>(u.e[0] - v.e[0], u.e[1] - v.e[1]);
}

template <typename T>
inline vec2<T> operator*(const vec2<T> &u, const vec2<T> &v) {
    return vec2<T>(u.e[0] * v.e[0], u.e[1] * v.e[1]);
}

template <typename T>
inline vec2<T> operator*(T t, const vec2<T> &v) {
    return vec2<T>(t*v.e[0], t*v.e[1]);
}

template <typename T>
inline vec2<T> operator*(const vec2<T> &v, T t) {
    return vec2<T>(t*v.e[0], t*v.e[1]);
}

template <typename T, typename U>
inline vec2<T> operator*(U t, const vec2<T>& v) {
    return v * static_cast<T>(t);
}

template <typename T, typename U>
inline vec2<T> operator*(const vec2<T>& v, U t) {
    return vec2<T>(static_cast<T>(t) * v.e[0], static_cast<T>(t) * v.e[1]);
}

template <typename T>
inline vec2<T> operator/(vec2<T> v, T t) {
    return (T(1)/t) * v;
}

template <typename T, typename U>
inline vec2<T> operator/(vec2<T> v, U t) {
    return static_cast<T>(1.0 / t) * v;
}

template <typename T>
inline T dot(const vec2<T> &u, const vec2<T> &v) {
    return u.e[0] * v.e[0]
           + u.e[1] * v.e[1];
}

template <typename T>
inline vec2<T> unit_vector(vec2<T> v) {
    return v / v.length();
}

// Type aliases for vec3
template <typename T>
using point2 = vec2<T>;   // 3D point

using vecf2 = vec2<float>; // float type
using pointf2 = point2<float>;
using texf2 = point2<float>; // texture coordinate
using vecd2 = vec2<double>; // double type
using pointd2 = point2<double>;


#endif // VEC2_H