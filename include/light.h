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
    float radius;
    char type;
    float time = 2 * M_PI ;
    unsigned int totalTime;

    void update(unsigned int deltaT){
        totalTime += deltaT;
        float ang    = static_cast<float>(totalTime) * time;
        switch(type){
            case 'o':
            {
                                float orbX   = std::sin(ang) * radius; 
                float orbZ   = std::cos(ang) * radius;
                position.x   = orbZ;
                position.y   = orbX;
                position.z   = orbX;
            }

            break;

            case 'f':
            break;

            case 'c':
            {
                float colX    = (std::sin(ang/12e3) + 1)/2.0f ; 
                float colY    = (std::cos(ang/6e3) + 1)/2.0f ; 
                color.x       = colX;
                color.y       = 1.0f-colY;
                color.z       = colY;
            }
            break;

            case 'l':
            {
                float linX    = std::sin(ang) * radius ; 
                position.x    = linX;
            }
            break;

            default:
            break;
        }
    }

};

#endif