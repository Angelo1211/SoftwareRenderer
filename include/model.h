#ifndef MODEL_H
#define MODEL_H

// ===============================
// AUTHOR       : Angel Ortiz (angelo12 AT vt DOT edu)
// CREATE DATE  : 2018-07-03
// PURPOSE      : Container for all of the data realted to a model such as texture data,
//                mesh data and even a model matrix for transformations.
//                It has an update method that could be called based on physics updates
//                but is not much more than a stub.
// ===============================
// SPECIAL NOTES: Should probably be rewritten to be a struct instead of having to use
// all of these useless getters. This class is little more than a glorified container.
// ===============================

//Headers
#include <string>
#include "mesh.h"
#include "geometry.h"
#include "matrix.h"
#include "texture.h"
#include "objParser.h"

class Model{
    public:
        //On model creation all textures are loaded, the mesh is built and even an
        //AABB is built.
        Model( const TransformParameters &initParameters, const std::string meshPath, const std::string materialPath) : 
            mAlbedo(materialPath + "_albedo.png", "RGB"),
            mNormal(materialPath + "_normal.png", "XYZ"),
            mAmbient(materialPath + "_ao.png", "BW"),
            mRoughness(materialPath + "_rough.png", "BW"),
            mMetallic(materialPath + "_metal.png", "BW"),
            mModelMatrix(Matrix4::transformMatrix(initParameters))
        {   
            OBJ::buildMeshFromFile(mMesh, meshPath);
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
        Texture *getRoughness();
        Texture *getMetallic();


        void update();

        //Prints the mesh vertices for debugging
        void describeMesh();
    private:
        Texture mAlbedo;
        Texture mNormal;
        Texture mAmbient;
        Texture mRoughness;
        Texture mMetallic;

        Mesh mMesh;
        AABox mBounds;
        Matrix4 mModelMatrix;
};

#endif