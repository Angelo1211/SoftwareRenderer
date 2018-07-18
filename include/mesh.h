#ifndef MESH_H
#define MESH_H

#include "vector3D.h"
#include <vector>

//Struct containing information relevant to the renderer about the vertices, normals and
//texture coordinates of a model. Also keeps track of useful stuff for iterations.
struct Mesh{
    int numVertices = 0;
    std::vector<Vector3f> vertices;
    std::vector<Vector3f> normals;

    int numFaces = 0;
    std::vector<Vector3i> vertexIndices;
    std::vector<Vector3i> textureIndices;
    std::vector<Vector3i> normalsIndices;

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