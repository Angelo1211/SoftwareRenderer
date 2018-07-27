#include "mesh.h"

void Mesh::describeMesh(){
    for(int i = 0; i < numVertices; ++i){
        printf("Vertex  %2.1d: %f, %f, %f \n",i,vertices[i].x, vertices[i].y, vertices[i].z);
    }
    printf("Meshsize is: %d \n", numVertices);
}
    
void Mesh::buildFacetNormals(){
    for(int i = 0; i < numFaces; ++i){
        Vector3i indices = vertexIndices[i];
        Vector3f N1 = vertices[indices.data[1]] - vertices[indices.data[0]];
        Vector3f N2 = vertices[indices.data[2]] - vertices[indices.data[0]];
        fNormals.push_back((N1.crossProduct(N2)).normalized());
    }
}