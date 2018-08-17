#ifndef LIGHT_H
#define LIGHT_H

// ===============================
// AUTHOR       : Angel Ortiz (angelo12 AT vt DOT edu)
// CREATE DATE  : 2018-08-03
// PURPOSE      : POD struct containing the two vectors that describe a lightsource
// ===============================

#include "vector3D.h"

struct BaseLight{
    Vector3f position;
    Vector3f color;
};

#endif