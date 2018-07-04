#include "vector3.h"
#include <math.h>

void Vector3::rotX(float theta){
    float cosine  = std::cos(theta);
    float sine    = std::sin(theta);
    float yTemp = y;
    float zTemp = z;
    y = yTemp*cosine + -zTemp*sine;
    z = yTemp*sine   + zTemp*cosine;
}   


void Vector3::rotY(float theta){
    float cosine  = std::cos(theta);
    float sine    = std::sin(theta);
    float xTemp = x;
    float zTemp = z;
    x = xTemp*cosine + zTemp*sine;
    z = -xTemp*sine   + zTemp*cosine;
}   

void Vector3::rotZ(float theta){
    float cosine  = std::cos(theta);
    float sine    = std::sin(theta);
    float xTemp = x;
    float yTemp = y;
    x = xTemp*cosine + -yTemp*sine;
    y = xTemp*sine   + yTemp*cosine;
}   

void Vector3::scale(float scale){
    x *= scale;
    y *= scale;
    z *= scale;
}

void Vector3::translate(float dx, float dy, float dz){
    x +=dx;
    y +=dy;
    z +=dz;
}