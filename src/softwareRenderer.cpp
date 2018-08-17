// ===============================
// AUTHOR       : Angel Ortiz (angelo12 AT vt DOT edu)
// CREATE DATE  : 2018-07-10
// ===============================

//Headers
#include "softwareRenderer.h"
#include "shader.h"
#include "mesh.h"
#include "omp.h"

//Dummy Constructor / Destructor
SoftwareRenderer::SoftwareRenderer(){}
SoftwareRenderer::~SoftwareRenderer(){}

bool SoftwareRenderer::startUp(int w, int h){
    if( !createBuffers(w, h) ){
        return false;
    }
    //Want to make sure that we don't call for a delete of zbuffer and pixelbuffer
    //Unless the startup has been complete
    startUpComplete = true;
    return startUpComplete;
}

void SoftwareRenderer::shutDown(){
    mLights = nullptr;
    mCamera = nullptr;
    //Only delete buffers if startup completed successfully
    if (startUpComplete){
        delete zBuffer;
        delete pixelBuffer;
    }
}

void SoftwareRenderer::drawTriangularMesh(Model * currentModel){
    //Getting the vertices, faces, normals and texture data for the whole model
    Mesh *triMesh = currentModel->getMesh();
    std::vector<Vector3i> * vIndices = &triMesh->vertexIndices;
    std::vector<Vector3i> * tIndices = &triMesh->textureIndices;
    std::vector<Vector3i> * nIndices = &triMesh->normalsIndices;
    std::vector<Vector3f> * fNormals = &triMesh->fNormals;

    std::vector<Vector3f> * vertices = &triMesh->vertices;
    std::vector<Vector3f> * texels   = &triMesh->texels;
    std::vector<Vector3f> * normals  = &triMesh->normals;
    std::vector<Vector3f> * tangents = &triMesh->tangents;
    int numFaces = triMesh->numFaces;

    //Initializing shader textures
    PBRShader shader;
    shader.albedoT   = currentModel->getAlbedo();
    shader.normalT   = currentModel->getNormal();
    shader.ambientOT = currentModel->getAO();
    shader.roughT    = currentModel->getRoughness();
    shader.metalT    = currentModel->getMetallic();

    //Setting up lighting
    Vector3f lightPositions[mNumLights];
    Vector3f lColor[mNumLights];
    for(int x = 0; x < mNumLights; ++x){
        lColor[x] = mLights[x].color;
        lightPositions[x] = mLights[x].position;
    }

    //Initializing shader matrices & variables
    shader.MV  = (mCamera->viewMatrix)*(*(currentModel->getModelMatrix()));
    shader.MVP = (mCamera->projectionMatrix)*shader.MV;
    shader.V   = (mCamera->viewMatrix);
    shader.M   = *(currentModel->getModelMatrix());
    shader.N   = (shader.M.inverse()).transpose(); 

    shader.cameraPos  = mCamera->position;
    shader.numLights  = mNumLights;
    shader.lightCol   = lColor;
    shader.lightPos   = lightPositions;

    //Building worldToObject matrix
    Matrix4 worldToObject = (*(currentModel->getModelMatrix())).inverse();

    //Iterate through every triangle on mesh with early quiting by backface culling
    //It also uses dynamic scheduling since on average 50% of the threads would finish early
    //because of backface culling. This allows for redistributing of parallel tasks between
    //threads to increase parallelization effectiveness.
    #pragma omp parallel for firstprivate(shader) schedule(dynamic)
    for (int j= 0; j < numFaces; ++j){
        //Arrays used to group vertices together into triangles
        Vector3f trianglePrimitive[3], normalPrim[3], uvPrim[3],
             tangentPrim[3];

        //Current vertex, normals and texture data indices
        Vector3i f = (*vIndices)[j];
        Vector3i n = (*nIndices)[j];
        Vector3i u = (*tIndices)[j];

        //Last setup of shader light variables
        Vector3f lightDir[mNumLights * 3 ];
        shader.lightDirVal = lightDir;    

        //Pack vertex, normal and UV data into triangles
        packDataIntoTris(f, trianglePrimitive, *vertices);
        packDataIntoTris(n, normalPrim, *normals);
        packDataIntoTris(u, uvPrim, *texels);
        packDataIntoTris(f, tangentPrim, *tangents);

        //Early quit if face is pointing away from camera
        if (backFaceCulling((*fNormals)[j], trianglePrimitive[0], worldToObject)) continue;

        //Apply vertex shader
        for(int i = 0; i < 3; ++i){
            trianglePrimitive[i] = shader.vertex(trianglePrimitive[i], normalPrim[i],
                                                uvPrim[i], tangentPrim[i], i);
        }

        //Skip triangles that are outside viewing frustrum
        //Does not rebuild triangles that are only partially out
        if (clipTriangles(trianglePrimitive)) continue;

        perspectiveDivide(trianglePrimitive);

        //Send to rasterizer which will also call the fragment shader and write to the 
        //zbuffer and pixel buffer.
        Rasterizer::drawTriangles(trianglePrimitive, shader, pixelBuffer, zBuffer);
        
    }
}

//Candidate to be refactored out into render manager
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

void SoftwareRenderer::setSceneLights(BaseLight * lights, int numLights){
    mNumLights = numLights;
    mLights = lights;
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

void SoftwareRenderer::packDataIntoTris(Vector3i &index, Vector3f *primitive, std::vector<Vector3f> &vals){
    for(int i = 0; i < 3; ++i){
        primitive[i] = vals[index.data[i]];
    }
}

//Gets view direction in object space and uses it to check if the facet normal
//Is aligned with the viewdirection
bool SoftwareRenderer::backFaceCulling(Vector3f &facetNormal, Vector3f &vert,  Matrix4 &worldToObject){
        Vector3f viewDir =  worldToObject.matMultVec(mCamera->position) -  vert;
        viewDir.normalized();

        //Returns false if the triangle cannot see the camera
        float intensity =  facetNormal.dotProduct(viewDir);
        return intensity <= 0.0;
}

//Inside bounds described in perspective matrix calculation
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
        clippedVertices[i].perspectiveDivide();
    }
}