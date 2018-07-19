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
            //Setting max values
            if(mesh.vertices[i].data[ax] > maxVals.data[ax]){
                maxVals.data[ax] = mesh.vertices[i].data[ax];
            }
            //Setting min values
            if(mesh.vertices[i].data[ax] < minVals.data[ax]){
                minVals.data[ax] = mesh.vertices[i].data[ax];
            }
        }
    }
    minPoints = minVals;
    maxPoints = maxVals;
}

//---------------------------------PLANE------------------------------------//

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
    nearH = near * tanHalfFOV;
    nearW = nearH * AR;

    farH  = far * tanHalfFOV;
    farW  = farH * AR;
}

void Frustrum::updatePlanes(Matrix4 &viewMat, const Vector3f &cameraPos){
    Vector3f X(viewMat(0,0), viewMat(0,1), viewMat(0,2));
    Vector3f Y(viewMat(1,0), viewMat(1,1), viewMat(1,2));
    Vector3f Z(viewMat(2,0), viewMat(2,1), viewMat(2,2));
    
    Vector3f nearCenter = cameraPos - Z * near;
    Vector3f farCenter  = cameraPos - Z * far;

    Vector3f point;
    Vector3f normal;

    //Near plane 
    pl[NEARP].setNormalAndPoint(-Z, nearCenter);
    //Far plane 
    pl[FARP].setNormalAndPoint(Z, farCenter);

    //Top plane
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




