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

        //This could be expanded with more methods
        // to draw different types of meshes or models
        void drawTriangularMesh(Mesh * triMesh);

        void clearBuffers();

        Buffer<Uint32>* getRenderTarget();

        void setCameraToRenderFrom(Camera * camera);
        
    private:
        //Buffer methods
        bool createBuffers(int w, int h);

        //Primitive building methods
        void buildTri(Vector3 &f, Vector3 *trianglePrim, std::vector<Vector3> &verts);

        //Culling methods
        bool backFaceCulling(Vector3 *trianglePrim, float &intensity);

        
        //Pointer to the scene's target camera
        Camera * mCamera;

        Buffer<float> * zBuffer;
        Buffer<Uint32> * pixelBuffer;
};

#endif