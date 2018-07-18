#ifndef VECTOR3_H
#define VECTOR3_H

#include <string>

//Basic 3D vector class for general calculations
//Should probably be made a template at some point 
struct Vector3{
    //I know this is undefined behaviour and really not recommended but
    //Using an operator overload for looping was slowing down my program 20%
    union {
            //Anonymous struct and array union hack
            float data[3];
            struct {
                float x;
                float y;
                float z;
            };
        };
    
    Vector3(float x1, float y1, float z1) : x(x1), y(y1), z(z1) {};
    Vector3(): x(0.0f), y(0.0f), z(0.0f) {};

    //Scalar operations
    Vector3 &operator-(); //Negate components of vector
    Vector3 operator*(float rhs); //Scalar vector multiplication

    //Vector operations
    Vector3 operator-(Vector3 &rhs);
    Vector3 operator+(Vector3 &rhs);
    Vector3 crossProduct(Vector3 &rhs);
    Vector3 &normalized();
    float   dotProduct(Vector3 &rhs);
    float length();
    
    //Print for debugging purposes
    void print();
};

#endif