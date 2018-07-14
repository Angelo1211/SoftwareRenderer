#ifndef MESH_H
#define MESH_H

#include "vector3.h"
#include <vector>

struct Mesh{
    int numVertices = 0;
    std::vector<Vector3> vertices;

    std::vector<Vector3> normals;

    int numFaces = 0;
    std::vector<Vector3> faces;
};

#endif