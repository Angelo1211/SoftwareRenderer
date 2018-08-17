#ifndef MESH_H
#define MESH_H

// ===============================
// AUTHOR       : Angel Ortiz (angelo12 AT vt DOT edu)
// CREATE DATE  : 2018-07-03
// PURPOSE      : Container for all geometry information related to vertices and 
//                faces. This information is what the renderer uses to build triangles.
// ===============================
// SPECIAL NOTES: Once again, this class uses a bunch of vectors instead of arrays and 
// it's something I would change if I had more time. 
// ===============================

//Headers
#include "vector3D.h"
#include <vector>

//Struct containing information relevant to the renderer about the vertices, normals and
//texture coordinates of a model. Also keeps track of useful stuff for iterating.
struct Mesh{
    //Per vertex values
    int numVertices = 0;
    std::vector<Vector3f> vertices;
    std::vector<Vector3f> normals;
    std::vector<Vector3f> texels;
    std::vector<Vector3f> tangents;
    std::vector<Vector3f> biTangents;

    //Per face values
    int numFaces = 0;
    std::vector<Vector3f> fNormals; //Normals for the whole face
    std::vector<Vector3i> vertexIndices;
    std::vector<Vector3i> textureIndices;
    std::vector<Vector3i> normalsIndices;

    //Simple mesh description for debugging.
    void describeMesh();

    //Builds facet normals used in early back face culling
    void buildFacetNormals();

    //Builds tangent and bitangent vectors for normal mapping
    void buildTangentSpace();
};

#endif