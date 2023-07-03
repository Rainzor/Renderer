//
// Created by Runze on 01/07/2023.
//
#include "mesh_triangle.h"
#include <chrono>
mesh_triangle::mesh_triangle(const std::vector<pointf3> &vertices, const std::vector<int>& faces,shared_ptr<material> m) {
    Init(vertices,faces,m);
}
mesh_triangle::mesh_triangle(const std::string& filename, shared_ptr<material> m,int scale){
    ModelImporter model;
    auto start_time = std::chrono::high_resolution_clock::now();
    model.parseOBJ(filename.c_str());
    auto end_time = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed_seconds = end_time - start_time;
    if(elapsed_seconds.count()>1.0)
        std::cout<<"Loading "<<filename<<" takes "<<elapsed_seconds.count()<<" seconds"<<std::endl;
    std::vector<pointf3> vertices;
    std::vector<float> vertices_f;
    std::vector<int> vertInds;

    model.getVertexIndices(vertInds);
    model.getVertexVals(vertices_f);
    for (int i = 0; i < vertices_f.size(); i += 3) {
            vertices.emplace_back(vertices_f[i]*scale, vertices_f[i + 1]*scale, vertices_f[i + 2]*scale);
    }
    Init(vertices, vertInds, m);

//    model mod = model(filename.c_str());
//    std::vector<pointf3> triangle_verts = mod.getTriangleVertices();
//    std::vector<texf2> triangle_texs = mod.getTriangleTexCoords();
//    std::vector<vecf3> triangle_norms = mod.getTriangleNormals();
//    std::vector<pointf3> vertices = mod.getVertices();
//    num = mod.getNumTriangles();
//    bool hasTex = !triangle_texs.empty();
//    bool hasNorm = !triangle_norms.empty();
//
//
//
//    this->aabb_min = vertices[0];
//    this->aabb_max = vertices[0];
//    for (auto vertice : vertices) {
//        if(vertice.x() < aabb_min.x()) aabb_min.e[0] = vertice.x();
//        if(vertice.y() < aabb_min.y()) aabb_min.e[1] = vertice.y();
//        if(vertice.z() < aabb_min.z()) aabb_min.e[2] = vertice.z();
//        if(vertice.x() > aabb_max.x()) aabb_max.e[0] = vertice.x();
//        if(vertice.y() > aabb_max.y()) aabb_max.e[1] = vertice.y();
//        if(vertice.z() > aabb_max.z()) aabb_max.e[2] = vertice.z();
//    }
//
//    std::vector<pointf3> a_triangle_vertices;
//    std::vector<texf2> a_triangle_texs;
//    std::vector<vecf3> a_triangle_norms;
//    for (int i = 0; i < num; i++) {
//        a_triangle_vertices.clear();
//        a_triangle_texs.clear();
//        a_triangle_norms.clear();
//        for (int j = 0; j < 3; ++j) {
//            a_triangle_vertices.push_back(triangle_verts[i*3+j]);
//            if(hasTex) a_triangle_texs.push_back(triangle_texs[i*3+j]);
//            if(hasNorm) a_triangle_norms.push_back(triangle_norms[i*3+j]);
//        }
//        meshes.add(make_shared<triangle>(a_triangle_vertices, a_triangle_texs, a_triangle_norms, m));
//    }
//


}

void mesh_triangle::Init(const std::vector<pointf3> &vertices, const std::vector<int>& faces,shared_ptr<material> m){
    this->aabb_min = vertices[0];
    this->aabb_max = vertices[0];
    for (auto vertice : vertices) {
        if(vertice.x() < aabb_min.x()) aabb_min.e[0] = vertice.x();
        if(vertice.y() < aabb_min.y()) aabb_min.e[1] = vertice.y();
        if(vertice.z() < aabb_min.z()) aabb_min.e[2] = vertice.z();
        if(vertice.x() > aabb_max.x()) aabb_max.e[0] = vertice.x();
        if(vertice.y() > aabb_max.y()) aabb_max.e[1] = vertice.y();
        if(vertice.z() > aabb_max.z()) aabb_max.e[2] = vertice.z();
    }

    pointf3 mean_point = (aabb_min + aabb_max) / 2;

    aabb_min = aabb_min - mean_point;
    aabb_max = aabb_max - mean_point;
    num = faces.size() / 3;
    for (int i = 0; i < faces.size(); i += 3) {
        pointf3 a = vertices[faces[i]]-mean_point;
        pointf3 b = vertices[faces[i + 1]]-mean_point;
        pointf3 c = vertices[faces[i + 2]]-mean_point;
        triangles.add(make_shared<triangle>(a, b, c, m));
    }
    auto start_time = std::chrono::high_resolution_clock::now();
    bvh = make_shared<bvh_node>(triangles,0,1);
    auto end_time = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed_seconds = end_time - start_time;
    if(elapsed_seconds.count()>1.0)
        std::cout<<"Building BVH takes "<<elapsed_seconds.count()<<" seconds"<<std::endl;
    mat_ptr = m;
}


bool mesh_triangle::hit(const ray &r, double t_min, double t_max, hit_record &rec) const {
    return bvh->hit(r, t_min, t_max, rec);
}

bool mesh_triangle::bounding_box(double time0, double time1, aabb &output_box) const {
    output_box = aabb(aabb_min, aabb_max);
    return true;
}