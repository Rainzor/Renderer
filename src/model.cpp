//
// Created by Runze on 01/07/2023.
//
#include <fstream>
#include <sstream>
#include <iostream> // cout 函数需要用
#include "model.h"

using namespace std;

// ------------ Imported Model class
model::model(const char *filePath) {
    ModelImporter modelImporter = ModelImporter();
    modelImporter.parseOBJ(filePath); // uses modelImporter to get vertex information
    numVertices = modelImporter.getNumVertices(); // 出错，这里的数值还是0
    numTriangles = modelImporter.getNumTriangles();
    std::vector<float> verts;
    modelImporter.getTriangleVertices(verts);
    std::vector<float> tcs;
    modelImporter.getTriangleTexCoords(tcs);
    std::vector<float> nmls;
    modelImporter.getTriangleNormals(nmls);

    bool is_textured = !tcs.empty();
    bool is_normal = !nmls.empty();

    for (int i = 0; i < numTriangles*3; i++) {
        triangle_vertices.push_back(vecf3(verts[i * 3], verts[i * 3 + 1], verts[i * 3 + 2]));
        if(is_textured)
            triangle_texCoords.push_back(vecf2(tcs[i * 2], tcs[i * 2 + 1]));
        if(is_normal)
            triangle_normals.push_back(vecf3(nmls[i * 3], nmls[i * 3 + 1], nmls[i * 3 + 2]));
    }

    verts.clear();
    tcs.clear();
    nmls.clear();

    modelImporter.getVertexVals(verts);
    modelImporter.getTextureVals(tcs);
    modelImporter.getNormalVals(nmls);

    for(int i = 0; i < numVertices; i++){
        vertices.push_back(vecf3(verts[i * 3], verts[i * 3 + 1], verts[i * 3 + 2]));
        if(is_textured)
            texCoords.push_back(vecf2(tcs[i * 2], tcs[i * 2 + 1]));
        if(is_normal)
            normals.push_back(vecf3(nmls[i * 3], nmls[i * 3 + 1], nmls[i * 3 + 2]));
    }

    modelImporter.getVertexIndices(vertInds);
    modelImporter.getTextureIndices(stInds);
    modelImporter.getNormalIndices(normInds);

}

int model::getNumVertices() { return numVertices; } // accessors // 出错，这里的数值还是0

int model::getNumTriangles() { return numTriangles; }

std::vector<vecf3> model::getVertices() { return vertices; }

std::vector<vecf2> model::getTextureCoords() { return texCoords; }

std::vector<vecf3> model::getNormals() { return normals; }

std::vector<vecf3> model::getTriangleVertices() { return triangle_vertices; }

std::vector<vecf2> model::getTriangleTexCoords() { return triangle_texCoords; }

std::vector<vecf3> model::getTriangleNormals() { return triangle_normals; }


// -------------- Model Importer class
ModelImporter::ModelImporter() {}
void ModelImporter::parseOBJ(const char *filePath) {
    float x, y, z;
    string content;
    ifstream fileStream(filePath, ios::in);
    string line = "";

    int iNum = 0;//调试
    while (!fileStream.eof()) {

        getline(fileStream, line);
        if (line.compare(0, 2, "v ") == 0) { // vertex position ("v" case)

            stringstream ss(line.erase(0, 1));
            ss >> x; ss >> y; ss >> z; // extract the vertex position values

            vertVals.push_back(x);
            vertVals.push_back(y);
            vertVals.push_back(z);


        }
        if (line.compare(0, 2, "vt") == 0) { // texture coordinates ("vt" case)
            stringstream ss(line.erase(0, 2));
            ss >> x; ss >> y; // extract texture coordinate values
            stVals.push_back(x);
            stVals.push_back(y);

        }
        if (line.compare(0, 2, "vn") == 0) { // vertex normals ("vn" case)
            stringstream ss(line.erase(0, 2));
            ss >> x; ss >> y; ss >> z; // extract the normal vector values
            normVals.push_back(x);
            normVals.push_back(y);
            normVals.push_back(z);

        }
        if (line.compare(0, 2, "f ") == 0) {
            // 注意，这里的 f 后面必须加一个空格，因为line.compare(0, 2, "f ")中表示从索引0开始的2个字符，而f不加空格只有一个字符

            string oneCorner, v, t, n;
            stringstream ss(line.erase(0, 2));
            bool isNormal = !normVals.empty();
            bool isTexture = !stVals.empty();
            for (int i = 0; i < 3; i++) {
                getline(ss, oneCorner, ' '); // extract triangle face references
                stringstream oneCornerSS(oneCorner);
                getline(oneCornerSS, v, '/');
                int vertRef = (stoi(v) - 1) * 3; // "stoi" converts string to integer
                triangleVerts.push_back(vertVals[vertRef]); // build vector of vertices
                triangleVerts.push_back(vertVals[vertRef + 1]);
                triangleVerts.push_back(vertVals[vertRef + 2]);
                vertInds.push_back(vertRef/3);
                if(isTexture){
                    getline(oneCornerSS, t, '/');
                    int tcRef = (stoi(t) - 1) * 2;
                    textureCoords.push_back(stVals[tcRef]); // build vector of texture coords
                    textureCoords.push_back(stVals[tcRef + 1]);
                    stInds.push_back(tcRef/2);
                }

                if(isNormal){
                    getline(oneCornerSS, n, '/');
                    int normRef = (stoi(n) - 1) * 3;
                    normals.push_back(normVals[normRef]); //… and normals
                    normals.push_back(normVals[normRef + 1]);
                    normals.push_back(normVals[normRef + 2]);
                    normInds.push_back(normRef/3);
                }
            }

        }
    }
}

//void ModelImporter::parseOBJ(const char *filePath) {
//    float x, y, z;
//    string content;
//    ifstream fileStream(filePath, ios::in);
//    string line = "";
//
//    while (std::getline(fileStream, line)) {
//        std::istringstream ss(line);
//
//        std::string prefix;
//        ss >> prefix;
//
//        if (prefix == "v ") { // vertex position ("v" case)
//            ss >> x >> y >> z; // extract the vertex position values
//
//            vertVals.push_back(x);
//            vertVals.push_back(y);
//            vertVals.push_back(z);
//        } else if (prefix == "vt") { // texture coordinates ("vt" case)
//            ss >> x >> y; // extract texture coordinate values
//
//            stVals.push_back(x);
//            stVals.push_back(y);
//        } else if (prefix == "vn") { // vertex normals ("vn" case)
//            ss >> x >> y >> z; // extract the normal vector values
//
//            normVals.push_back(x);
//            normVals.push_back(y);
//            normVals.push_back(z);
//        } else if (prefix == "f ") {
//            std::string oneCorner, v, t, n;
//
//            for (int i = 0; i < 3; i++) {
//                ss >> oneCorner;
//                std::istringstream oneCornerSS(oneCorner);
//
//                std::getline(oneCornerSS, v, '/');
//                int vertRef = (std::stoi(v) - 1) * 3; // "stoi" converts string to integer
//                triangleVerts.push_back(vertVals[vertRef]); // build vector of vertices
//                triangleVerts.push_back(vertVals[vertRef + 1]);
//                triangleVerts.push_back(vertVals[vertRef + 2]);
//
//                if (std::getline(oneCornerSS, t, '/') && !stVals.empty()) {
//                    if (!t.empty()) {
//                        int tcRef = (std::stoi(t) - 1) * 2;
//                        textureCoords.push_back(stVals[tcRef]); // build vector of texture coords
//                        textureCoords.push_back(stVals[tcRef + 1]);
//                    }
//                }
//
//                if (std::getline(oneCornerSS, n, '/') && !normVals.empty()) {
//                    if (!n.empty()) {
//                        int normRef = (std::stoi(n) - 1) * 3;
//                        normals.push_back(normVals[normRef]); // build vector of normals
//                        normals.push_back(normVals[normRef + 1]);
//                        normals.push_back(normVals[normRef + 2]);
//                    }
//                }
//            }
//        }
//    }
//}

int ModelImporter::getNumVertices() { return (vertVals.size() / 3); } // accessors  // 先注释，为了调试，等调试结束再解除注释

int ModelImporter::getNumTriangles() { return (triangleVerts.size() / 9); }

void ModelImporter::getVertexVals(std::vector<float> & vvals) { vvals = vertVals; }

void ModelImporter::getTextureVals(std::vector<float>& svals) { svals = stVals; }

void ModelImporter::getNormalVals(std::vector<float>& nvals) { nvals = normVals; }

void ModelImporter::getVertexIndices(std::vector<int> & vidxs) { vidxs = vertInds; }

void ModelImporter::getTextureIndices(std::vector<int>& sidxs) { sidxs = stInds; }

void ModelImporter::getNormalIndices(std::vector<int>& nidxs) { nidxs = normInds; }

void ModelImporter::getTriangleVertices(std::vector<float>& tri_v) { tri_v = triangleVerts; }

void ModelImporter::getTriangleTexCoords(std::vector<float>& tri_t) { tri_t = textureCoords; }

void ModelImporter::getTriangleNormals(std::vector<float>& tri_n) { tri_n = normals; }
