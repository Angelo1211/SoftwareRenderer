#ifndef VECTOR3_H
#define VECTOR3_H

#include <string>

struct Vector3{
    float x;
    float y;
    float z;

    
    Vector3(float x1, float y1, float z1) : x(x1), y(y1), z(z1)
    {}

    Vector3(): x(0.0f), y(0.0f), z(0.0f)
    {};


    //Negate components of vector
    Vector3 &operator-();

    Vector3 operator-(Vector3 &rhs);

    //Accessing components using array notation for looping
    float &operator[](int i);

    Vector3 &normalized();

    float length();

    Vector3 crossProduct(Vector3 &rhs);

    float   dotProduct(Vector3 &rhs);

    void print();

};

#endif