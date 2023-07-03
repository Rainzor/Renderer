#include "triangle.h"
triangle::triangle(const pointf3& a, const pointf3& b, const pointf3& c, shared_ptr<material> m)
        : v0(a), v1(b), v2(c), mat_ptr(m) {
    // 计算三角形的法向量
    vn1 = unit_vector(cross(v1 - v0, v2 - v0));
    vn2 = vn1;
    vn3 = vn1;

    vt0 = texf2(0, 0);
    vt1 = texf2(0, 1);
    vt2 = texf2(1, 0);

//    // 计算三角形的纹理坐标
//    vecf3 edge1 = v1 - v0;
//    vecf3 edge2 = v2 - v1;
//    vecf3 edge3 = v0 - v2;
//
//    float len1 = edge1.length();
//    float len2 = edge2.length();
//    float len3 = edge3.length();
//
//    float max_len = std::max(len1,std::max(len2, len3));
//    float angle_cos;
//    float angle_sin;
//    float remaining_len;
//    if (max_len == len1) {
//        vt0 = texf2 (0, 0);
//        vt1 = texf2 (0, 1);
//        angle_cos = dot(edge1, -edge3) / (len1 * len3);
//        angle_sin = sqrt(1 - angle_cos * angle_cos);
//        remaining_len = len3/len1;
//        vt2 = texf2 (remaining_len * angle_cos, remaining_len * angle_sin);
//    } else if (max_len == len2) {
//        vt1 = texf2(0, 0);
//        vt2 = texf2(0, 1);
//        angle_cos = dot(edge2, -edge1) / (len2 * len1);
//        angle_sin = sqrt(1 - angle_cos * angle_cos);
//        remaining_len = len1/len2;
//        vt0 = texf2(remaining_len * angle_cos, remaining_len * angle_sin);
//    } else {
//        vt2 = texf2(0, 0);
//        vt0 = texf2(0, 1);
//        angle_cos = dot(edge3, -edge2) / (len3 * len2);
//        angle_sin = sqrt(1 - angle_cos * angle_cos);
//        remaining_len = len2/len3;
//        vt1 = texf2(remaining_len * angle_cos, remaining_len * angle_sin);
//    }
};

triangle::triangle(const std::vector<pointf3> &vertexes, shared_ptr<material> m) {
    if(vertexes.size() != 3){
        std::cerr << "triangle: vertexes.size() != 3" << std::endl;
        exit(1);
    } else {
        v0 = vertexes[0];
        v1 = vertexes[1];
        v2 = vertexes[2];
        vn1 = unit_vector(cross(v1 - v0, v2 - v0));
        vn2 = vn1;
        vn3 = vn1;

        vt0 = texf2(0, 0);
        vt1 = texf2(0, 1);
        vt2 = texf2(1, 0);
    }
}

triangle::triangle(const std::vector<pointf3> &vertexes, const std::vector<texf2> &textures, shared_ptr<material> m) {
    if (textures.empty()){
        std::cerr<<"triangle: textures is empty"<<std::endl;
        exit(1);
    }

    else {
        v0 = vertexes[0];
        v1 = vertexes[1];
        v2 = vertexes[2];
        vt0 = textures[0];
        vt1 = textures[1];
        vt2 = textures[2];
        mat_ptr = m;

        // 计算三角形的法向量
        vn1 = unit_vector(cross(v1 - v0, v2 - v0));
        vn2 = vn1;
        vn3 = vn1;
    }
}
triangle::triangle(const std::vector<pointf3> &vertexes, const std::vector<texf2> &textures, const std::vector<vecf3> &normals, shared_ptr<material> m) {
    if (normals.empty()) {
        std::cerr << "triangle: normals is empty" << std::endl;
        exit(1);
    }
    else {
        v0 = vertexes[0];
        v1 = vertexes[1];
        v2 = vertexes[2];
        vt0 = textures[0];
        vt1 = textures[1];
        vt2 = textures[2];
        vn1 = normals[0];
        vn2 = normals[1];
        vn3 = normals[2];
        mat_ptr = m;
    }
}

bool triangle::hit(const ray& r, double t_min, double t_max, hit_record& rec) const {
    // 使用 Möller-Trumbore 算法计算三角形与光线的交点
    vecf3 edge1 = v1 - v0;
    vecf3 edge2 = v2 - v0;
    vecf3 h = cross(r.direction(), edge2);

    auto a = dot(edge1, h);

    // 判断光线是否与三角形平行
    if (a == 0)
        return false;

    auto f = 1.0 / a;
    vecf3 s = r.origin() - v0;
    //重心坐标u
    auto u = f * dot(s, h);

    // 判断交点是否在三角形内部
    if (u < 0.0 || u > 1.0)
        return false;

    vecf3 q = cross(s, edge1);
    //重心坐标v
    auto v = f * dot(r.direction(), q);

    if (v < 0.0 || u + v > 1.0)
        return false;

    //计算 t 值
    auto t = f * dot(edge2, q);

    if (t < t_min || t > t_max)
        return false;

    pointf3 weight = pointf3(1 - u - v, u, v);

    // 记录击中信息
    rec.t = t;
    rec.p = r.at(t);
    get_triangle_uv(weight,rec.u, rec.v);
    rec.mat_ptr = mat_ptr;
    vecf3 normal = unit_vector(weight.x() * vn1 + weight.y() * vn2 + weight.z() * vn3);
    rec.set_face_normal(r, normal);

    return true;
}

bool triangle::bounding_box(double time0, double time1, aabb& output_box) const {
    pointf3 min_point, max_point;
    min_point = pointf3(
            std::min(v0.x(), std::min(v1.x(), v2.x())),
            std::min(v0.y(), std::min(v1.y(), v2.y())),
            std::min(v0.z(), std::min(v1.z(), v2.z()))
    );
    max_point = pointf3(
            std::max(v0.x(), std::max(v1.x(), v2.x())),
            std::max(v0.y(), std::max(v1.y(), v2.y())),
            std::max(v0.z(), std::max(v1.z(), v2.z()))
    );
    if(min_point.x() == max_point.x()) {
        min_point.e[0] -= 0.0001;
        max_point.e[0] += 0.0001;
    }
    if(min_point.y() == max_point.y()) {
        min_point.e[1] -= 0.0001;
        max_point.e[1] += 0.0001;
    }
    if(min_point.z() == max_point.z()) {
        min_point.e[2] -= 0.0001;
        max_point.e[2] += 0.0001;
    }

    output_box = aabb(min_point, max_point);
    return true;
}

