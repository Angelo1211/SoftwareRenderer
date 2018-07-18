#ifndef SRENDERER_H
#define SRENDERER_H

#include "rasterizer.h" 
#include "buffer.h"
#include "mesh.h"
#include "camera.h"

class SoftwareRenderer {

    public:
        //Dummy Constructor / Destructor
        SoftwareRenderer();
        ~SoftwareRenderer();

        //Creates all buffers and preps everything for for rendering
        bool startUp(int w, int h);  
        void shutDown();      

        //Draws mesh assuming it is made of triangular primites
        //1.-Gets pointers to render data form mesh
        //2.-Builds MVP 
        void drawTriangularMesh(Mesh * triMesh);

        void clearBuffers();

        //Returns pixel buffer
        Buffer<Uint32>* getRenderTarget();

        void setCameraToRenderFrom(Camera * camera);
    private:
        //Buffer methods
        bool createBuffers(int w, int h);

        //Primitive building methods
        void buildTri(Vector3i &f, Vector3f *trianglePrim, std::vector<Vector3f> &vals);

        //Culling methods
        bool backFaceCulling(Vector3f *trianglePrim);

        //Pointer to the scene's target camera
        Camera * mCamera;
        bool startUpComplete = false;

        Buffer<float> * zBuffer;
        Buffer<Uint32> * pixelBuffer;
};

#endif