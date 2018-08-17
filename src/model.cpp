// ===============================
// AUTHOR       : Angel Ortiz (angelo12 AT vt DOT edu)
// CREATE DATE  : 2018-07-03
// ===============================

//Headers
#include "model.h"

Mesh * Model::getMesh(){
    return &mMesh;
}

void Model::update(){
    //Recalculate model matrix for movement or scaling
    mBounds.update(mModelMatrix);
}
AABox *Model::getBounds(){
    return &mBounds;
}
Matrix4 *Model::getModelMatrix(){
    return &mModelMatrix;
}
//Texture getters
Texture *Model::getAlbedo(){
    return &mAlbedo;
}
Texture *Model::getNormal(){
    return &mNormal;
}
Texture *Model::getAO(){
    return &mAmbient;
}
Texture *Model::getRoughness(){
    return &mRoughness;
}
Texture *Model::getMetallic(){
    return &mMetallic;
}