#ifndef MODEL_H
#define MODEL_H

#include "mesh.h"
#include "string"
#include "bound3.h"
#include "matrix.h"

class Model{

    public:
        Model(std::string path); 
        void describeMesh();

        Mesh *getMesh();
        
        void initPosition(TransformParameters initPos);
        void update();

    private:
        
        void buildBoundaryBox();
        void buildMesh(std::string path);
        void loadVertices(std::ifstream &fileHandle);
        void loadFaces(std::ifstream &fileHandle);
        Mesh mMesh;
        Bound3 mBounds;
};

#endif