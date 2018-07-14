#include "softwareRenderer.h"
#include "shader.h"

SoftwareRenderer::SoftwareRenderer(){

}

SoftwareRenderer::~SoftwareRenderer(){

}

bool SoftwareRenderer::startUp(int w, int h){
    if( !createBuffers(w, h) ){
        return false;
    }
    return true;
}

void SoftwareRenderer::shutDown(){
    mCamera = nullptr;
    delete zBuffer;
    delete pixelBuffer;
}

void SoftwareRenderer::clearBuffers(){
    zBuffer->clear();
    pixelBuffer->clear();
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

void SoftwareRenderer::drawTriangularMesh(Mesh* triMesh){

    //Getting the vertices, faces 
    std::vector<Vector3> * faces = &triMesh->faces;
    std::vector<Vector3> * vertices = &triMesh->vertices;
    std::vector<Vector3> * normals = &triMesh->normals;

    //Array grouping vertices together into triangle
    Vector3 trianglePrimitive[3];

    //Initializing shader
    GourardShader shader;

    //Building ModelViewProjection matrix
    Matrix4 MVP = (mCamera->projectionMatrix)*(mCamera->viewMatrix);

    float intensity = 0;
    for (Vector3 &f : *faces ){

        //Pack vertices together into an array
        buildTri(f,trianglePrimitive, *vertices);

        //Skip faces that are pointing away from us
        if (backFaceCulling(trianglePrimitive, intensity)) continue;

        //Apply vertex shader
        for(int i = 0; i < 3; ++i){
            trianglePrimitive[i] = shader.vertex(trianglePrimitive[i], MVP, intensity);
        }

        //Clipping should occur here

        //Send to rasterizer which will also call the fragment shader and write to the 
        //zbuffer and pixel buffer
        Rasterizer::drawTriangles(trianglePrimitive, shader, pixelBuffer, zBuffer);
    }
}

Buffer<Uint32>* SoftwareRenderer::getRenderTarget(){
    return pixelBuffer;
}

void SoftwareRenderer::buildTri(Vector3 &f, Vector3 *trianglePrim, std::vector<Vector3> &verts){
    for(int i = 0; i < 3; ++i){
        trianglePrim[i] = verts[f.data[i]];
    }
}

void SoftwareRenderer::setCameraToRenderFrom(Camera * camera){
    mCamera = camera;
}

bool SoftwareRenderer::backFaceCulling(Vector3 *trianglePrim, float &intensity){
        //Triangle surface normal 
        //Should probably be calculated on load next time
        Vector3 N1 = trianglePrim[1] - trianglePrim[0];
        Vector3 N2 = trianglePrim[2] - trianglePrim[0];
        Vector3 N  = (N2.crossProduct(N1)).normalized();

        //View direction
        Vector3 view_dir =  trianglePrim[0] - mCamera->position;
        view_dir = view_dir.normalized();

        intensity =  N.dotProduct(view_dir);
        return intensity <= 0.0;
}