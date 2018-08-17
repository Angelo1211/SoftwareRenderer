#ifndef SRENDERER_H
#define SRENDERER_H

// ===============================
// AUTHOR       : Angel Ortiz (angelo12 AT vt DOT edu)
// CREATE DATE  : 2018-07-10
// PURPOSE      : Core graphics class that performs rendering for different mesh types. 
//                The core rendering function is drawTriangularMesh that contains the logic
//                for drawing triangular mesh objects as described in the mesh class.
//                It attempts to recreate a similar pipeline to the old OpenGL 2  one
//                due to its simplicity.
// ===============================
// SPECIAL NOTES: As mentioned in the manager level class, many tasks done here should be
// offloaded to the manager class to reduce coupling. Specifically, the buffers should be
// moved to the manager class and treated simply as rendertargets for the renderer to draw
// to. There is also no real need for the render class to have knowledge of my definition
// of a model and camera. On a second pass I would rebuild this from the ground up with a 
// similar structure to opengl's VBO, VAO and EBO's system. 
// ===============================

//Headers
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

        //Creates all buffers and preps everything for model rendering
        bool startUp(int w, int h);  
        void shutDown();      

        //Overview:
        //01.-Gets pointers to mesh data from model
        //02.-Builds Model view and projection matrix
        //03.-Iterates through all triangle faces (in parallel!)
        //04.-Runs backface culling algo uisng facet normmals
        //05.-Applies vertex shader per vertex in face
        //06.-Performs clipping of triangles fully outside view frustrum 
        //07.-Applies perspective divide using "hidden" w coordinate
        //08.-Sends shader data and triangle to rasterizer
        //09.-NDC to viewport transform
        //10.-Builds triangle bounding box and Iterates through
        //11.-Calculates perspective correct barycentric coordinates
        //12.-Culls pixels outside of triangle or screen
        //13.-Runs fragment shader per pixel that passes depth buffer check
        //14.-updates zBuffer with new value and gamma corrects color
        //15.-Writes to pixel frame buffer 
        void drawTriangularMesh(Model * currentModel);

        //Returns pixel buffer
        Buffer<Uint32>* getRenderTarget();
        void clearBuffers();

        //Set up methods called by rendermanager
        void setCameraToRenderFrom(Camera * camera);
        void setSceneLights(BaseLight * lights, int numLights);

    private:
        //Buffer methods
        bool createBuffers(int w, int h);

        //Primitive level methods
        void packDataIntoTris(Vector3i &f, Vector3f *trianglePrim, std::vector<Vector3f> &vals);
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

        //Buffers
        Buffer<float> * zBuffer;
        Buffer<Uint32> * pixelBuffer;
};

#endif