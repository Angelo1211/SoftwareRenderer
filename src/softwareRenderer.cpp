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
    std::vector<Vector3i> * vIndices = &triMesh->vertexIndices;
    std::vector<Vector3i> * nIndices = &triMesh->normalsIndices;
    std::vector<Vector3f> * vertices = &triMesh->vertices;
    std::vector<Vector3f> * normals = &triMesh->normals;
    int numFaces = triMesh->numFaces;

    //Array grouping vertices together into triangle
    Vector3f trianglePrimitive[3];
    Vector3f normalPrim[3];

    //Initializing shader
    GouraudShader shader;

    //Basic light direction
    Vector3f lightDir{1,0,0};

    //Building ModelViewProjection matrix
    Matrix4 MVP = (mCamera->projectionMatrix)*(mCamera->viewMatrix);

    //Iterate through every triangle
    for (int j = 0; j < numFaces; ++j){
        //Current vertex and normal indices
        Vector3i f = (*vIndices)[j];
        Vector3i n = (*nIndices)[j];

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

        //Skip triangles that are outside viewing frustrum
        //Does not rebuild triangles that are partially out TO DO
        if (clipTriangles(trianglePrimitive)) continue;

        perspectiveDivide(trianglePrimitive);

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

void SoftwareRenderer::buildTri(Vector3i &index, Vector3f *primitive, std::vector<Vector3f> &vals){
    for(int i = 0; i < 3; ++i){
        primitive[i] = vals[index.data[i]];
    }
}

bool SoftwareRenderer::backFaceCulling(Vector3f *trianglePrim){
        //Triangle surface normal 
        //Should probably be calculated on load next time
        Vector3f N1 = trianglePrim[1] - trianglePrim[0];
        Vector3f N2 = trianglePrim[2] - trianglePrim[0];
        Vector3f N  = (N2.crossProduct(N1)).normalized();

        Vector3f view_dir =  trianglePrim[0] - mCamera->position;
        view_dir = view_dir.normalized();

        //Returns false if the triangle cannot see the camera
        float intensity =  N.dotProduct(view_dir);
        return intensity <= 0.0;
}

bool SoftwareRenderer::clipTriangles(Vector3f *clipSpaceVertices){
    int count = 0;
    for(int i = 0; i < 3; ++i){
        Vector3f vertex = clipSpaceVertices[i];
        bool inside = (-vertex.w <= vertex.x <= vertex.w ) 
                    && (-vertex.w <= vertex.y <= vertex.w)
                    && (0 <= vertex.z <= vertex.w);
        if (!inside) ++count;
    }
    //If count equals three it means every vertex was out so we skip it
    return count == 3 ;
}

void SoftwareRenderer::perspectiveDivide(Vector3f *clippedVertices){
    for(int i = 0; i < 3; ++i){
        clippedVertices[i] = clippedVertices[i].perspectiveDivide();
    }
}