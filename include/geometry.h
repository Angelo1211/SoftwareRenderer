#ifndef GEOMETRY_H
#define GEOMETRY_H

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

struct Plane{
    Vector3f normal;
    float D;

    float distance(const Vector3f &points);
    void setNormalAndPoint(const Vector3f &normal, const Vector3f &point);
};

class Frustrum{
    private:
    	enum {
		TOP = 0, BOTTOM, LEFT,
		RIGHT, NEARP, FARP
        };
    public:
        float  fov, AR, near, far , nearH, nearW, farH, farW;
        Frustrum(float ratio): fov(50), near(0.1), far(100), AR(ratio){};
        Plane pl[6];

        void setCamInternals();
        void updatePlanes(Matrix4 &viewMat, const Vector3f &cameraPos) ;
        bool checkIfInside(AABox *bounds);
};


#endif
