#ifndef VECTOR3D_H
#define VECTOR3D_H

// ===============================
// AUTHOR       : Angel Ortiz (angelo12 AT vt DOT edu)
// CREATE DATE  : 2018-07-18
// PURPOSE      : Template vector class used for all vector operations, 3d, 2d and 4d
//                vectors included. It is capable of performing most normal vector 
//                operations and allows for access using both x,y,z and array indexing
// ===============================
// SPECIAL NOTES: Homogeneous coordinates are used with 3D vectors by hiding in the 
//                implementation a 4th variable w taht contains the 4th coordinate.
//                In the words of Douglas Adams:
//   "This has made a lot of people very angry and been widely regarded as a bad move"
// ===============================

//Headers
#include <string>
#include <type_traits>
#include "math.h"

//Basic 3D vector class for general calculations
template<typename T>
struct Vector3{
    //I know this is undefined behaviour and really not recommended but
    //Using an operator overload for looping was slowing down my program 20%
    union {
        //Anonymous struct and array union hack
        T data[3];
        struct {
            T x;
            T y;
            T z;
        };
    };
    //W component only ever used in perspective projection and clipping,
    //Just imagine it isn't here
    T w;

    //Constructors
    Vector3(): x(0), y(0), z(0), w(1) {};
    Vector3(T val): x(val), y(val), z(val), w(1) {};
    Vector3(T x1, T y1, T z1) : x(x1), y(y1), z(z1), w(1) {};
    
    //Homogenous coordinates to cartesian transformation
    Vector3 &perspectiveDivide(){
        x /= w;
        y /= w;
        z /= w;
        return *this;
    }

    //Scalar-vector operations
    Vector3 operator-() const
    {return Vector3(-x, -y, -z);} 
    Vector3 operator*(const T &rhs) const //Scalar-vector multiplication
    {return Vector3(x*rhs, y*rhs, z*rhs);}
    Vector3 operator+(const T &rhs) const //Scalar-vector addition
    {return Vector3(x+rhs, y+rhs, z+rhs);}

    //Vector-vector operations
    Vector3 operator-(const Vector3 &rhs) const
    {return Vector3(x - rhs.x, y - rhs.y, z - rhs.z);}

    Vector3 operator+(const Vector3 &rhs) const
    {return Vector3(x + rhs.x, y + rhs.y, z + rhs.z);}

    void operator+=(const Vector3 &rhs) {   
        x += rhs.x;
        y += rhs.y;
        z += rhs.z;
    }

    void operator-=(const Vector3 &rhs) 
    {(*this) = (*this) - rhs ;}

    Vector3 operator*(const Vector3 &rhs) const
    {return Vector3(x * rhs.x, y * rhs.y, z * rhs.z);}

    Vector3 crossProduct(const Vector3 &r) const
    {return Vector3( (y*r.z - z*r.y), (z*r.x - x*r.z), (x*r.y - y*r.x) );}

    T   dotProduct(const Vector3 &rhs) const
    {return x*rhs.x + y*rhs.y + z*rhs.z;}

    T   dot2D(const Vector3 &rhs) const
    {return x*rhs.x + y*rhs.y;}

    T length() const
    {return std::sqrt(x*x + y*y + z*z);}

    //Used to account for edge case of len = 0 but branching impedes SIMD 
    //auto vectorization
    Vector3 &normalized(){
        T len = length();
        T factor = 1.0f / len;
        x *= factor;
        y *= factor;
        z *= factor;

        return *this;
    }

    //Only used for blinn shading within specular reflection calculations
    static Vector3 reflect(const Vector3 &I, const Vector3 &N){
        return I - ((N * I.dotProduct(N)) * 2.0f);
    }
    
    //Print for debugging purposes
    void print() const{
        std::string str;
        if(std::is_same<T,float>::value){
            str = "Vecf:(%f, %f, %f)\n";
            
        }
        else if(std::is_same<T,int>::value) {
            str = "Veci:(%d, %d, %d)\n";    
        }
        printf(str.c_str(),x,y,z);
    }

    void zero() {
        x = 0;
        y = 0; 
        z = 0;
    }
};

//Shorthands for the common vector types
typedef Vector3<float> Vector3f; 
typedef Vector3<int> Vector3i; 

#endif