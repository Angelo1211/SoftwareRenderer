#ifndef OBJPARSER_H
#define OBJPARSER_H

#include <fstream>
#include <sstream>
#include "mesh.h"

//Just a namespace for functions related to loading data from wavefront OBJ files
namespace OBJ{
    //Assumes that the path given is valid
    Mesh& buildMeshFromFile(Mesh &mesh, std::string &path);

    //Checks if the path actually contains a valid file
    bool fileExists(std::string &path);

    //Get vertices, normals and all index data into the mesh object
    void loadFileData(Mesh &mesh, std::ifstream &file);

    //Simple string splitting method using a single char delimeter
    std::vector<std::string> splitStr(std::string &str, char delim);
}

#endif
