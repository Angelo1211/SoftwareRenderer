#include "model.h"
#include "vector3D.h"

Model::Model(std::string path, TransformParameters &initParameters){
    OBJ::buildMeshFromFile(mMesh, path);
    initPosition(initParameters);
    mBounds.buildAABB(mMesh);
}

Mesh * Model::getMesh(){
    return &mMesh;
}

void Model::initPosition(TransformParameters initVals){
    Matrix4 modelMatrix = Matrix4::transformMatrix(initVals);
    int size = mMesh.numVertices;
    std::vector<Vector3f> * vertices = &mMesh.vertices;

    //Applying the multiplication
    for (int i = 0;i < size; ++i){
        (*vertices)[i] = modelMatrix.matMultVec((*vertices)[i]);
    }
}
//TO DO 
void Model::update(){
    //You'd get physics updates or user input updates or whatever here
    //Move
    //Update AABB
}

AABox *Model::getBounds(){
    return &mBounds;
}