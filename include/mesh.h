#ifndef MESH_H
#define MESH_H

#include "vector3.h"
#include <vector>

//Struct containing information relevant to the renderer about the vertices, normals and
//texture coordinates of a model. Also keeps track of useful stuff for iterations.
struct Mesh{
    int numVertices = 0;
    std::vector<Vector3> vertices;
    std::vector<Vector3> normals;

    int numFaces = 0;
    std::vector<Vector3> vertexIndices;
    std::vector<Vector3> textureIndices;
    std::vector<Vector3> normalsIndices;

    //Simple mesh description for debugging.
    void describeMesh();
};

//Struct containing vertex data for a tight bounding box around a model.
//Primarily for use in frustum culling
struct Bound3{
    float mMinX = 0.0;
    float mMinY = 0.0;
    float mMinZ = 0.0;

    float mMaxX = 0.0;
    float mMaxY = 0.0;
    float mMaxZ = 0.0;
};

#endif