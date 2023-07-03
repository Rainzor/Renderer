#pragma once
#include <vector>
#include "common.h"
class model
{
private:
    int numVertices;
    int numTriangles;
    std::vector<vecf3> triangle_vertices;
    std::vector<vecf2> triangle_texCoords;
    std::vector<vecf3> triangle_normals;
    std::vector<vecf3> vertices;
    std::vector<vecf2> texCoords;
    std::vector<vecf3> normals;
    std::vector<int> vertInds;
    std::vector<int> stInds;
    std::vector<int> normInds;

public:
    model(const char *filePath);
    int getNumVertices();
    int getNumTriangles();
    std::vector<vecf3> getVertices();
    std::vector<vecf2> getTextureCoords();
    std::vector<vecf3> getNormals();
    std::vector<vecf3> getTriangleVertices();
    std::vector<vecf2> getTriangleTexCoords();
    std::vector<vecf3> getTriangleNormals();


};
class ModelImporter
{
private:
    // values as read in from OBJ file
    std::vector<float> vertVals;
    std::vector<float> stVals;
    std::vector<float> normVals;
    std::vector<int> vertInds;
    std::vector<int> stInds;
    std::vector<int> normInds;

    // values stored for later use as vertex attributes
    std::vector<float> triangleVerts;
    std::vector<float> textureCoords;
    std::vector<float> normals;
public:
    ModelImporter();
    void parseOBJ(const char *filePath);
    int getNumVertices();
    int getNumTriangles();
    void getVertexVals(std::vector<float> & vvals);
    void getTextureVals(std::vector<float> & tvals);
    void getNormalVals(std::vector<float> & nvals);

    void getVertexIndices(std::vector<int> & vidxs);
    void getTextureIndices(std::vector<int> & tidxs);
    void getNormalIndices(std::vector<int> & nidxs);
    void getTriangleVertices(std::vector<float>& tri_v);
    void getTriangleTexCoords(std::vector<float>& tri_st);
    void getTriangleNormals(std::vector<float>& tri_norm);
public:
    bool is_texture;
    bool is_normal;
};
