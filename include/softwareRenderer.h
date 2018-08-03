#ifndef SRENDERER_H
#define SRENDERER_H

#include "rasterizer.h" 
#include "buffer.h"
#include "model.h"
#include "camera.h"
#include "light.h"

class SoftwareRenderer {

    public:
        //Dummy Constructor / Destructor
        SoftwareRenderer();
        ~SoftwareRenderer();

        //Creates all buffers and preps everything for for rendering
        bool startUp(int w, int h);  
        void shutDown();      

        //Overview:
        //01.-Gets pointers to render data form mesh
        //02.-Builds MVP 
        //03.-Iterates through all triangle faces
        //04.-Runs backface culling algo
        //05.-Applies vertex shader per vertex
        //06.-Performs clipping of triangles outside frustrum
        //07.-Applies perspective divide
        //08.-Sends to triangle rasterizer
        //09.-NDC -> viewport transform
        //10.-Iterates through triangle bounding box 
        //11.-Calculates barycentric coordinates
        //12.-Culls pixels outside of triangle or screen
        //13.-Runs fragment shader
        //14.-zBuffer update
        //15.-Writes to frame buffer
        //16.-Swap buffer
        void drawTriangularMesh(Model * currentModel);

        void clearBuffers();

        //Returns pixel buffer
        Buffer<Uint32>* getRenderTarget();

        void setCameraToRenderFrom(Camera * camera);

        void setSceneLights(BaseLight * lights, int numLights);
    private:
        //Buffer methods
        bool createBuffers(int w, int h);

        //Primitive level methods
        void buildTri(Vector3i &f, Vector3f *trianglePrim, std::vector<Vector3f> &vals);
        void perspectiveDivide(Vector3f *clippedVertices);

        //Culling and clipping methods
        bool backFaceCulling(Vector3f &facetNormal, Vector3f &vertex, Matrix4 &worldToObject);
        bool clipTriangles(Vector3f *clipSpaceVertices);

        //Pointer to the scene's target camera
        Camera * mCamera;
        bool startUpComplete = false;

        //Pointer to structs containing all the light info
        int mNumLights;
        BaseLight *mLights;

        Buffer<float> * zBuffer;
        Buffer<Uint32> * pixelBuffer;
};

#endif