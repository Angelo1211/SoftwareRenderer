#include "mesh.h"

void Mesh::describeMesh(){
    for(int i = 0; i < numVertices; ++i){
        Vector3 vertex = vertices[i];
        printf("Vertex  %2.1d: %f, %f, %f \n",i,vertex.x, vertex.y, vertex.z);
    }
    printf("Meshsize is: %d \n", numVertices);
}