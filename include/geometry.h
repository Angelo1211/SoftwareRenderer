#ifndef GEOMETRY_H
#define GEOMETRY_H

// ===============================
// AUTHOR       : Angel Ortiz (angelo12 AT vt DOT edu)
// CREATE DATE  : 2018-07-19
// PURPOSE      : This file contains all of the geometry classes. Mostly important for
//                view frustrum culling since it contains the geometry for axis aligned
//                bounding boxes, planes and frustrums.
// ===============================

//Headers
#include "vector3D.h"
#include "mesh.h"
#include "matrix.h"

//Struct containing vertex data for a AABB
//around the model. Primarily for use in frustum culling
//Regiong R = {(x, y, z) | min.x <=x <= max.x | same Y | same Z}
struct AABox{
    Vector3f minPoints;
    Vector3f maxPoints;

    //Builds axis aligned bounding box of the given mesh
    void buildAABB(const Mesh &mesh);
    void update(const Matrix4 &modelMatrix);
};

//Only used in frustrum culling, a frustrum has 6 planes
//Equation is Ax + By + Cz + D = 0 (or somthing)
struct Plane{
    Vector3f normal;
    float D;

    float distance(const Vector3f &points);
    void setNormalAndPoint(const Vector3f &normal, const Vector3f &point);
};

//The shape of the camera view area
class Frustrum{
    private:
    	enum {
		TOP = 0, BOTTOM, LEFT,
		RIGHT, NEARP, FARP
        };
    public:
        float  fov, AR, near, far , nearH, nearW;
        Frustrum(float ratio): fov(50), near(0.1), far(100), AR(ratio){};
        Plane pl[6];

        void setCamInternals();
        void updatePlanes(Matrix4 &viewMat, const Vector3f &cameraPos) ;
        bool checkIfInside(AABox *bounds);
};
#endif
