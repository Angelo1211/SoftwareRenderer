#include "model.h"

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

Texture *Model::getAO(){
    return &mAmbient;
}

Texture *Model::getRoughness(){
    return &mRoughness;
}

Texture *Model::getMetallic(){
    return &mMetallic;
}