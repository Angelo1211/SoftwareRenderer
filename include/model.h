#ifndef MODEL_H
#define MODEL_H

#include <string>
#include "mesh.h"
#include "geometry.h"
#include "matrix.h"


class Model{
    public:
        Model(std::string path, TransformParameters &initParameters); 

        Mesh *getMesh();
        Matrix4 &getModelMatrix();
        AABox *getBounds();

        void update();

        //Prints the mesh vertices for debugging
        void describeMesh();
    private:
        Mesh mMesh;
        AABox mBounds;
        Matrix4 mModelMatrix;
};

#endif