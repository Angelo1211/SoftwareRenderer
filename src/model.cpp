#include "model.h"
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include "vector3.h"
#include "matrix.h"
#include <limits>

Model::Model(std::string path){
    buildMesh(path);
    buildBoundaryBox();
}

Mesh * Model::getMesh(){
    return &mMesh;
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
    loadVertices(file);

    //Get faces
    loadFaces(file);

    //Close file after reading
    file.close();
}

void Model::loadFaces(std::ifstream &file){
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

void Model::loadVertices(std::ifstream &file){
    

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

void Model::initPosition(TransformParameters initVals){
    Matrix4 modelMatrix = Matrix4::transformMatrix(initVals);
    int size = mMesh.numVertices;
    std::vector<Vector3> * vertices = &mMesh.vertices;

    //Applying the multiplication
    for (int i = 0;i < size; ++i){
        (*vertices)[i] = modelMatrix.matMultVec((*vertices)[i]);
    }
}

//I know this is incredibly inefficient, I only need to do it once for now
//If if I ever need something faster I will move this into the vertex loading
//function and make use of the read there.
void Model::buildBoundaryBox(){
    float minX = std::numeric_limits<float>::max();
    float maxX = std::numeric_limits<float>::min();

    float minY = minX;
    float maxY = maxX;

    float minZ = minX;
    float maxZ = maxX;

    for(int i = 0; i < mMesh.numVertices; ++i){
        float x = mMesh.vertices[i].x;
        float y = mMesh.vertices[i].y;
        float z = mMesh.vertices[i].z;

        if(x > maxX){
            maxX = x;
        }
        else{
            if(x < minX){
                minX = x;
            }
        }

        if(y > maxY){
            maxY = y;
        }
        else{
            if(y < minY){
                minY = y;
            }
        }

        if(z > maxZ){
            maxZ = z;
        }
        else{
            if(z < minZ){
                minZ = z;
            }
        }
    }

    mBounds.mMaxX = maxX;
    mBounds.mMaxY = maxY;
    mBounds.mMaxZ = maxZ;

    mBounds.mMinX = minX;
    mBounds.mMinY = minY;
    mBounds.mMinZ = minZ;

}