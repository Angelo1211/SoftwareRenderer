#ifndef MODEL_H
#define MODEL_H

#include <string>
#include "mesh.h"
#include "geometry.h"
#include "matrix.h"
#include "texture.h"

class Model{
    public:
        Model(std::string path, TransformParameters &initParameters); 

        Mesh *getMesh();
        Matrix4 *getModelMatrix();
        AABox *getBounds();
        Texture *getAlbedo();
        Texture *getNormal();

        void update();

        //Prints the mesh vertices for debugging
        void describeMesh();
    private:
        Texture mAlbedo{"../models/fire_hydrant_Base_Color.png"};
        Texture mNormal{"../models/fire_hydrant_Normal_OpenGL.png"};
        Mesh mMesh;
        AABox mBounds;
        Matrix4 mModelMatrix;
};

#endif