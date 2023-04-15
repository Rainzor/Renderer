#ifndef VEC3_H
#define VEC3_H

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
    return vec3<T>(static_cast<T>(t) * v.e[0], static_cast<T>(t) * v.e[1], static_cast<T>(t) * v.e[2]);
}

template <typename T,typename U>
inline vec3<T> operator*(const vec3<T> &v, U t) {
    return vec3<T>(static_cast<T>(t) * v.e[0], static_cast<T>(t) * v.e[1], static_cast<T>(t) * v.e[2]);
}

template <typename T>
inline vec3<T> operator*(const vec3<T> &v, T t) {
    return t * v;
}

template <typename T, typename U>
inline vec3<T> operator*(U t, const vec3<T>& v) {
    return static_cast<T>(t) * v;
}

template <typename T>
inline vec3<T> operator/(vec3<T> v, T t) {
    return (T(1)/t) * v;
}

template <typename T, typename U>
inline vec3<T> operator/(vec3<T> v, U t) {
    return static_cast<T>(1/t) * v;
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


// Type aliases for vec3
template <typename T>
using point3 = vec3<T>;   // 3D point
template <typename T>
using color = vec3<T>;    // RGB color

using vecf3 = vec3<float>; // float type
using pointf3 = point3<float>;
using rgbf = color<float>;



#endif // VEC3_H