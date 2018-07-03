#include "model.h"
#include <stdio.h>
#include <iostream>
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

    //Open file containing vertex data
    std::ifstream file;
    file.open(path.c_str());

    //Get vertices into mesh
    getVertices(file);

    //Get faces
    getFaces(file);

    //Close file after reading
    file.close();
}

void Model::getFaces(std::ifstream &file){
    std::string line, f, x ,y ,z;
    while(!file.eof()){
        
        std::getline(file,line);
        std::istringstream iss(line);
        iss >> f;
        if(f == "f"){
            iss >> x >> y >> z;
            Vector3 face(x,y,z);
            mMesh.faces.push_back(face);
        }
    }
    mMesh.numFaces = mMesh.faces.size();
    file.clear();
    file.seekg(0, file.beg);
}

void Model::getVertices(std::ifstream &file){
    std::string line, v, x ,y ,z;
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
    mMesh.numVertices = mMesh.vertices.size();
    file.clear();
    file.seekg(0, file.beg);
}