#ifndef MODEL_H
#define MODEL_H

#include "mesh.h"
#include "string"

class Model{

    public:
        Model(std::string path); 
        void describeMesh();
        Mesh *getMesh();

    private:
        void buildMesh(std::string path);
        void getVertices(std::ifstream &fileHandle);
        void getFaces(std::ifstream &fileHandle);
        Mesh mMesh;
};

#endif