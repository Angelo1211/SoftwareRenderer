#include "softwareRenderer.h"
#include "shader.h"

SoftwareRenderer::SoftwareRenderer(){}
SoftwareRenderer::~SoftwareRenderer(){}

bool SoftwareRenderer::startUp(int w, int h){
    if( !createBuffers(w, h) ){
        return false;
    }
    startUpComplete = true;
    return true;
}

void SoftwareRenderer::shutDown(){
    mCamera = nullptr;
    if (startUpComplete){
        delete zBuffer;
        delete pixelBuffer;
    }
}

void SoftwareRenderer::drawTriangularMesh(Mesh* triMesh){

    //Getting the vertices, faces 
    std::vector<Vector3> * vIndices = &triMesh->vertexIndices;
    std::vector<Vector3> * nIndices = &triMesh->normalsIndices;
    std::vector<Vector3> * vertices = &triMesh->vertices;
    std::vector<Vector3> * normals = &triMesh->normals;
    int numFaces = triMesh->numFaces;

    //Array grouping vertices together into triangle
    Vector3 trianglePrimitive[3];
    Vector3 normalPrim[3];

    //Initializing shader
    GouraudShader shader;

    //Basic light direction
    Vector3 lightDir{1,0,0};

    //Building ModelViewProjection matrix
    Matrix4 MVP = (mCamera->projectionMatrix)*(mCamera->viewMatrix);

    //Iterate through every triangle
    for (int j = 0; j < numFaces; ++j){
        //Current vertex and normal indices
        Vector3 f = (*vIndices)[j];
        Vector3 n = (*nIndices)[j];

        //Pack vertex and normal data together into an array
        buildTri(f,trianglePrimitive, *vertices);
        buildTri(n,normalPrim, *normals);

        //Skip faces that are pointing away from us
        if (backFaceCulling(trianglePrimitive)) continue;

        //Apply vertex shader
        for(int i = 0; i < 3; ++i){
            //Vector3 normal = (mCamera->viewMatrix).matMultVec(normalPrim[i]);
            trianglePrimitive[i] = shader.vertex(trianglePrimitive[i], MVP, normalPrim[i], lightDir.normalized(), i);
        }

        //Clipping should occur here

        //Send to rasterizer which will also call the fragment shader and write to the 
        //zbuffer and pixel buffer.
        Rasterizer::drawTriangles(trianglePrimitive, shader, pixelBuffer, zBuffer);
    }
}

void SoftwareRenderer::clearBuffers(){
    zBuffer->clear();
    pixelBuffer->clear();
}

Buffer<Uint32>* SoftwareRenderer::getRenderTarget(){
    return pixelBuffer;
}

void SoftwareRenderer::setCameraToRenderFrom(Camera * camera){
    mCamera = camera;
}

bool SoftwareRenderer::createBuffers(int w, int h){
    int pixelCount = w*h;
    bool success = true;

    zBuffer = new Buffer<float>(w, h, new float[pixelCount]);
    if( zBuffer == nullptr){
        printf("Could not build z-Buffer.\n");
        success = false;
    }
    else{
        pixelBuffer = new Buffer<Uint32>(w, h, new Uint32[pixelCount]);
        if( pixelBuffer == nullptr){
            printf("Could not build pixel Buffer.\n");
            success = false;
        }
    }
    return success;
}

void SoftwareRenderer::buildTri(Vector3 &index, Vector3 *primitive, std::vector<Vector3> &vals){
    for(int i = 0; i < 3; ++i){
        primitive[i] = vals[(int)index.data[i]];
    }
}

bool SoftwareRenderer::backFaceCulling(Vector3 *trianglePrim){
        //Triangle surface normal 
        //Should probably be calculated on load next time
        Vector3 N1 = trianglePrim[1] - trianglePrim[0];
        Vector3 N2 = trianglePrim[2] - trianglePrim[0];
        Vector3 N  = (N2.crossProduct(N1)).normalized();

        Vector3 view_dir =  trianglePrim[0] - mCamera->position;
        view_dir = view_dir.normalized();

        //Returns false if the triangle cannot see the camera
        float intensity =  N.dotProduct(view_dir);
        return intensity <= 0.0;
}