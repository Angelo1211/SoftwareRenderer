// ===============================
// AUTHOR       : Angel Ortiz (angelo12 AT vt DOT edu)
// CREATE DATE  : 2018-07-19
// ===============================

//Headers
#include "geometry.h"
#include <limits>
#include <math.h>

//-------------------------------AABOX------------------------------------//

void AABox::buildAABB(const Mesh &mesh){
    Vector3f minVals(std::numeric_limits<float>::max());
    Vector3f maxVals(std::numeric_limits<float>::min());

    //Iterating through every vertx in the mesh
    for(int i = 0; i < mesh.numVertices; ++i){
        //Checking the current vertex for all axes
        for(int ax = 0; ax < 3; ++ax){
            //Thanks @Erkaman!
            //Setting max values
            maxVals.data[ax] = std::max(mesh.vertices[i].data[ax], maxVals.data[ax]);
            //Setting min values
            minVals.data[ax] = std::min(mesh.vertices[i].data[ax], minVals.data[ax]);
        }
    }
    minPoints = minVals;
    maxPoints = maxVals;
}

//Any change in the posiiton, rotation, scale of an object will cause a change
//in the AABB This function rebuilds it based on those changes
//which are contained within a model matrix.
void AABox::update(const Matrix4 &modelMatrix){
    Vector3f minVals(std::numeric_limits<float>::max());
    Vector3f maxVals(std::numeric_limits<float>::min());
    Vector3f vertices[8];

    //Reconstructing the 8 vertices of an AABB from the min and max valuesin the struct
    vertices[0] = Vector3f(minPoints.x, minPoints.y, minPoints.z);
    vertices[1] = Vector3f(maxPoints.x, minPoints.y, minPoints.z);
    vertices[2] = Vector3f(minPoints.x, maxPoints.y, minPoints.z);
    vertices[3] = Vector3f(maxPoints.x, maxPoints.y, minPoints.z);
    vertices[4] = Vector3f(minPoints.x, minPoints.y, maxPoints.z);
    vertices[5] = Vector3f(maxPoints.x, minPoints.y, maxPoints.z);
    vertices[6] = Vector3f(minPoints.x, maxPoints.y, maxPoints.z);
    vertices[7] = Vector3f(maxPoints.x, maxPoints.y, maxPoints.z);

    //Iterating through every vertx in the mesh
    for(int i = 0; i < 8; ++i){
        //Checking the current vertex for all axes
        for(int ax = 0; ax < 3; ++ax){
            //Thanks @Erkaman!
            //Setting max values
            maxVals.data[ax] = std::max(vertices[i].data[ax], maxVals.data[ax]);
            //Setting min values
            minVals.data[ax] = std::min(vertices[i].data[ax], minVals.data[ax]);
        }
    }

    minPoints = minVals;
    maxPoints = maxVals;
}


//---------------------------------PLANE------------------------------------//
//Returns a negative value if not aligned in the same direction of plane normal
//which I established to be pointing towards the interior of the camera frustrum
float Plane::distance(const Vector3f &points){
    return normal.dotProduct(points) + D;
}

void Plane::setNormalAndPoint(const Vector3f &n, const Vector3f &p0){
    normal = n;
    D = -(n.dotProduct(p0));
}

//-------------------------------FRUSTRUM------------------------------------//

void Frustrum::setCamInternals(){
    float tanHalfFOV  =  tan( (fov/2) * (M_PI / 180) );
    nearH = near * tanHalfFOV; //Half of the frustrum near plane height
    nearW = nearH * AR;
}

//Calculates frustrum planes in world space
void Frustrum::updatePlanes(Matrix4 &viewMat, const Vector3f &cameraPos){
    setCamInternals();
    Vector3f X(viewMat(0,0), viewMat(0,1), viewMat(0,2)); //Side Unit Vector
    Vector3f Y(viewMat(1,0), viewMat(1,1), viewMat(1,2)); //Up Unit Vector
    Vector3f Z(viewMat(2,0), viewMat(2,1), viewMat(2,2)); //Forward vector
    
    //Gets worlds space position of the center points of the near and far planes
    //The forward vector Z points towards the viewer so you need to negate it and scale it
    //by the distance (near or far) to the plane to get the center positions
    Vector3f nearCenter = cameraPos - Z * near;
    Vector3f farCenter  = cameraPos - Z * far;

    Vector3f point;
    Vector3f normal;

    //We build the planes using a normal and a point (in this case the center)
    //Z is negative here because we want the normal vectors we choose to point towards
    //the inside of the view frustrum that way we can cehck in or out with a simple 
    //Dot product
    pl[NEARP].setNormalAndPoint(-Z, nearCenter);
    //Far plane 
    pl[FARP].setNormalAndPoint(Z, farCenter);

    //Again, want to get the plane from a normal and point
    //You scale the up vector by the near plane height and added to the nearcenter to 
    //optain a point on the edge of both near and top plane.
    //Subtracting the cameraposition from this point generates a vector that goes along the 
    //surface of the plane, if you take the cross product with the direction vector equal
    //to the shared edge of the planes you get the normal
    point  = nearCenter + Y*nearH;
    normal = (point - cameraPos).normalized();
    normal = normal.crossProduct(X);
    pl[TOP].setNormalAndPoint(normal, point);

    //Bottom plane
    point  = nearCenter - Y*nearH;
    normal = (point - cameraPos).normalized();
    normal = X.crossProduct(normal);
    pl[BOTTOM].setNormalAndPoint(normal, point);

    //Left plane
    point  = nearCenter - X*nearW;
    normal = (point - cameraPos).normalized();
    normal = normal.crossProduct(Y);
    pl[LEFT].setNormalAndPoint(normal, point);

    //Right plane
    point  = nearCenter + X*nearW;
    normal = (point - cameraPos).normalized();
    normal = Y.crossProduct(normal);
    pl[RIGHT].setNormalAndPoint(normal, point);
    
}

//False is fully outside, true if inside or intersects
//based on iquilez method
bool Frustrum::checkIfInside(AABox *box){

    //Check box outside or inside of frustrum
    for(int i =0; i < 6; ++i){
        int out = 0;
        out += ((pl[i].distance(Vector3f(box->minPoints.x, box->minPoints.y, box->minPoints.z))< 0.0 )?1:0);
        out += ((pl[i].distance(Vector3f(box->maxPoints.x, box->minPoints.y, box->minPoints.z))< 0.0 )?1:0);
        out += ((pl[i].distance(Vector3f(box->minPoints.x, box->maxPoints.y, box->minPoints.z))< 0.0 )?1:0);
        out += ((pl[i].distance(Vector3f(box->maxPoints.x, box->maxPoints.y, box->minPoints.z))< 0.0 )?1:0);
        out += ((pl[i].distance(Vector3f(box->minPoints.x, box->minPoints.y, box->maxPoints.z))< 0.0 )?1:0);
        out += ((pl[i].distance(Vector3f(box->maxPoints.x, box->minPoints.y, box->maxPoints.z))< 0.0 )?1:0);
        out += ((pl[i].distance(Vector3f(box->minPoints.x, box->maxPoints.y, box->maxPoints.z))< 0.0 )?1:0);
        out += ((pl[i].distance(Vector3f(box->maxPoints.x, box->maxPoints.y, box->maxPoints.z))< 0.0 )?1:0);
        
        if (out == 8) return false;
    }
    return true;
}