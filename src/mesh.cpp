#include "mesh.h"

void Mesh::describeMesh(){
    for(int i = 0; i < numVertices; ++i){
        printf("Vertex  %2.1d: %f, %f, %f \n",i,vertices[i].x, vertices[i].y, vertices[i].z);
    }
    printf("Meshsize is: %d \n", numVertices);
}