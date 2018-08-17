// ===============================
// AUTHOR       : Angel Ortiz (angelo12 AT vt DOT edu)
// CREATE DATE  : 2018-07-03
// ===============================

//Headers
#include "mesh.h"

void Mesh::describeMesh(){
    for(int i = 0; i < numVertices; ++i){
        printf("Vertex  %2.1d: %f, %f, %f \n",i,vertices[i].x, vertices[i].y, vertices[i].z);
    }
    printf("Meshsize is: %d \n", numVertices);
}
    
void Mesh::buildFacetNormals(){
    for(int i = 0; i < numFaces; ++i){
        Vector3i indices = vertexIndices[i];
        Vector3f N1 = vertices[indices.data[1]] - vertices[indices.data[0]];
        Vector3f N2 = vertices[indices.data[2]] - vertices[indices.data[0]];
        fNormals.push_back((N1.crossProduct(N2)).normalized());
    }
}
//Builds per vertex tangent and bitangents by first finding tangeents and bitangents
//per face. Then taking the average value at each vertex and then renormalizing because
//average process might be altering direction. Then also corrects for any incorrect handedness
//issues. Finally sets the correct vectors in a per vertex fashion
void Mesh::buildTangentSpace(){
    std::vector<std::vector<Vector3f>> tempTangents(numVertices);
    std::vector<std::vector<Vector3f>> tempBiTangents(numVertices);
    Vector3f tangent, biTangent;

    //Extract the tangent and bitangentn of each surface triangle
    //Assign the value to a temporary vector of vectors of vector3's (yikes)
    //“When I wrote this, only God and I understood what I was doing. Now, God only knows.”
    for(int i = 0; i < numFaces; ++i){
        Vector3i vIndices = vertexIndices[i];
        Vector3i tIndices = textureIndices[i];

        Vector3f v0 = vertices[vIndices.data[0]];
        Vector3f v1 = vertices[vIndices.data[1]];
        Vector3f v2 = vertices[vIndices.data[2]];

        Vector3f edge1 = v1 - v0;
        Vector3f edge2 = v2 - v0;

        Vector3f deltaUV1 = texels[tIndices.data[1]] - texels[tIndices.data[0]];
        Vector3f deltaUV2 = texels[tIndices.data[2]] - texels[tIndices.data[0]];

        float f = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV2.x * deltaUV1.y);

        tangent.x = f * (deltaUV2.y * edge1.x - deltaUV1.y * edge2.x);
        tangent.y = f * (deltaUV2.y * edge1.y - deltaUV1.y * edge2.y);
        tangent.z = f * (deltaUV2.y * edge1.z - deltaUV1.y * edge2.z);
        tangent = tangent.normalized();

        biTangent.x = f * (-deltaUV2.x * edge1.x + deltaUV1.x * edge2.x);
        biTangent.y = f * (-deltaUV2.x * edge1.y + deltaUV1.x * edge2.y);
        biTangent.z = f * (-deltaUV2.x * edge1.z + deltaUV1.x * edge2.z);
        biTangent = biTangent.normalized();

        tempTangents[vIndices.data[0]].push_back(tangent);
        tempTangents[vIndices.data[1]].push_back(tangent);
        tempTangents[vIndices.data[2]].push_back(tangent);

        tempBiTangents[vIndices.data[0]].push_back(biTangent);
        tempBiTangents[vIndices.data[1]].push_back(biTangent);
        tempBiTangents[vIndices.data[2]].push_back(biTangent);

    }

    //Take average of tangents and bitangents at that vertex and populate the array in the 
    //correct order
    for(int j = 0; j < numVertices; ++j){
        int count1 = 0, count2 = 0;

        Vector3f temp1{0};
        while(!tempTangents[j].empty()){
            ++count1;
            temp1 += tempTangents[j].back();
            if (temp1.x == 0 && temp1.y == 0 && temp1.z == 0){
                temp1 = tempTangents[j].back();
            }
            tempTangents[j].pop_back();
        }

        temp1 = temp1 * (1.0f/count1);
        tangents.push_back(temp1);

        Vector3f temp2{0};
        while(!tempBiTangents[j].empty()){
            ++count2;
            temp2 += tempBiTangents[j].back();
             if (temp2.x == 0 && temp2.y == 0 && temp2.z == 0){
                temp2 = tempBiTangents[j].back();
            }
            tempBiTangents[j].pop_back();
        }
        temp2 = temp2 * (1.0f/count2);
        biTangents.push_back(temp2);
    }

    //Renormalize and correct handedness of tangent/bitangent vectors
    for(int k = 0; k < numFaces; ++k){
        Vector3i vIndices = vertexIndices[k];
        Vector3i nIndices = normalsIndices[k];

        //Per vertex
        Vector3f tangentV0 = tangents[vIndices.data[0]]; 
        Vector3f tangentV1 = tangents[vIndices.data[1]];
        Vector3f tangentV2 = tangents[vIndices.data[2]];

        Vector3f normalV0 = normals[nIndices.data[0]]; 
        Vector3f normalV1 = normals[nIndices.data[1]];
        Vector3f normalV2 = normals[nIndices.data[2]];

        Vector3f biTangentV0 = biTangents[vIndices.data[0]]; 
        Vector3f biTangentV1 = biTangents[vIndices.data[1]];
        Vector3f biTangentV2 = biTangents[vIndices.data[2]];

        //Renormalizing
        tangentV0  = (tangentV0 - (normalV0*tangentV0.dotProduct(normalV0))).normalized();
        tangentV1  = (tangentV1 - (normalV1*tangentV1.dotProduct(normalV1))).normalized();
        tangentV2  = (tangentV2 - (normalV2*tangentV2.dotProduct(normalV2))).normalized();

        //Correcting handedness
        if (biTangentV0.dotProduct(normalV0.crossProduct(tangentV0)) < 0.0f){
            tangentV0 = tangentV0  * -1.0f;
        }

        if (biTangentV1.dotProduct(normalV1.crossProduct(tangentV1)) < 0.0f){
            tangentV1 = tangentV1  * -1.0f;
        }

        if (biTangentV2.dotProduct(normalV2.crossProduct(tangentV2)) < 0.0f){
            tangentV2 = tangentV2  * -1.0f;
        }

        tangents[vIndices.data[0]]  = tangentV0;
        tangents[vIndices.data[1]]  = tangentV1; 
        tangents[vIndices.data[2]]  = tangentV2; 

    }


}