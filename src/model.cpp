#include "model.h"
#include "objParser.h"

Model::Model(std::string path, TransformParameters &initParameters){
    OBJ::buildMeshFromFile(mMesh, path);
    mBounds.buildAABB(mMesh);
    mMesh.buildFacetNormals();
    mMesh.buildTangentSpace();
    mModelMatrix = Matrix4::transformMatrix(initParameters);
}

Mesh * Model::getMesh(){
    return &mMesh;
}

void Model::update(){
    //You'd get physics updates or user input updates or whatever here
    //Recalculate model matrix for movement or scaling
    mBounds.update(mModelMatrix);
}

AABox *Model::getBounds(){
    return &mBounds;
}

Matrix4 *Model::getModelMatrix(){
    return &mModelMatrix;
}

Texture *Model::getAlbedo(){
    return &mAlbedo;
}

Texture *Model::getNormal(){
    return &mNormal;
}