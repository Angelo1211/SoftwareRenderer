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
    std::vector<Vector3f> fNormals; //Normals for the whole face
    std::vector<Vector3f> texels;

    int numFaces = 0;
    std::vector<Vector3i> vertexIndices;
    std::vector<Vector3i> textureIndices;
    std::vector<Vector3i> normalsIndices;

    //Simple mesh description for debugging.
    void describeMesh();

    //Builds facet normals for early back face culling
    void buildFacetNormals();
};

#endif