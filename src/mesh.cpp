#include "mesh.h"

void Mesh::describeMesh(){
    int meshSize = numVertices;
    for(int i = 0; i < meshSize; ++i){
        Vector3 vertex = vertices[i];
        printf("Vertex  %2.1d: %f, %f, %f \n",i,vertex.x, vertex.y, vertex.z);
    }
    printf("Meshsize is: %d \n", meshSize);
}