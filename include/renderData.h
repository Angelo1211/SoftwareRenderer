#ifndef RENDERDATA_H
#define RENDERDATA_H

#include "matrix.h"
#include "mesh.h"
#include "shader.h"

struct RenderData{
    Matrix4 *modelMatrix; 
    Mesh *modelMesh;
    IShader *shader;
};




#endif