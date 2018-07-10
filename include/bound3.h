#ifndef BOUND3_H
#define BOUND3_H

#include "vector3.h"

//Struct containing vertex data for a tight bounding box around a model.
//Primarily for use in frustum culling
struct Bound3{
    float mMinX = 0.0;
    float mMinY = 0.0;
    float mMinZ = 0.0;

    float mMaxX = 0.0;
    float mMaxY = 0.0;
    float mMaxZ = 0.0;
};

#endif