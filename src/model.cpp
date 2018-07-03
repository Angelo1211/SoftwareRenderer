#include "model.h"
#include <stdio.h>
#include <fstream>
#include <sstream>
#include "vector3.h"

Model::Model(std::string path){
    buildMesh(path);
}

Mesh Model::getMesh(){
    return mMesh;
}

void Model::describeMesh(){
    int meshSize = mMesh.numVertices;
    for(int i = 0; i < meshSize; ++i){
        Vector3 vertex = mMesh.vertices[i];
        printf("Vertex  %2.1d: %f, %f, %f \n",i,vertex.x, vertex.y, vertex.z);
    }
    printf("Meshsize is: %d \n", meshSize);
}

void Model::buildMesh(std::string path){
    printf("Loading models...\n");
    std::fstream file;
    std::string line, v, x ,y ,z;

    //Open file containing vertex data
    file.open(path.c_str());

    //Get vertices into mesh
    while(!file.eof()){
        std::getline(file,line);
        std::istringstream iss(line);
        iss >> v;
        if(v == "v"){
            iss >> x >> y >> z;
            Vector3 vertex(x,y,z);
            mMesh.vertices.push_back(vertex);
        }
    }
    file.close();
    mMesh.numVertices = mMesh.vertices.size();
}