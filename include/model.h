#ifndef MODEL_H
#define MODEL_H

#include <string>
#include "mesh.h"
#include "geometry.h"
#include "matrix.h"
#include "texture.h"
#include "objParser.h"

class Model{
    public:
        Model(std::string basePath, TransformParameters &initParameters) : 
            mAlbedo(basePath + "_albedo.png", "RGB"),
            mNormal(basePath + "_normal.png", "XYZ"),
            mAmbient(basePath + "_ao.png", "BW"),
            mModelMatrix(Matrix4::transformMatrix(initParameters))
        {   
            OBJ::buildMeshFromFile(mMesh, basePath + "_mesh.obj");
            mBounds.buildAABB(mMesh);
            mMesh.buildFacetNormals();
            mMesh.buildTangentSpace();
        }; 

        //TODO: too many getters, unify into one method?
        Mesh *getMesh();
        Matrix4 *getModelMatrix();
        AABox *getBounds();
        Texture *getAlbedo();
        Texture *getNormal();
        Texture *getAO();

        void update();

        //Prints the mesh vertices for debugging
        void describeMesh();
    private:
        
        Texture mAlbedo;
        Texture mNormal;
        Texture mAmbient;
        Mesh mMesh;
        AABox mBounds;
        Matrix4 mModelMatrix;
};

#endif