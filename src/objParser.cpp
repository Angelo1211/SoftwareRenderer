// ===============================
// AUTHOR       : Angel Ortiz (angelo12 AT vt DOT edu)
// CREATE DATE  : 2018-07-14
// ===============================

//Headers
#include "objParser.h"

Mesh& OBJ::buildMeshFromFile(Mesh &mesh, std::string path){

    //Open file containing vertex data
    std::ifstream file;
    file.open(path.c_str());
    
    //Get vertices and normal data &
    //Get face, normal and texture indices
    loadFileData(mesh, file);

    file.close(); 
}

bool OBJ::fileExists(std::string &path){
    std::ifstream file(path.c_str());
    return file.good();
}

//Main OBJ parsing function
void OBJ::loadFileData(Mesh &mesh, std::ifstream &file){
    std::string line, key, x ,y ,z;
    float tempU, tempV, intpart;
    Vector3i indices[3];
    char delimeter = '/';
    while(!file.eof()){
        std::getline(file,line);
        std::istringstream iss(line);
        iss >> key;
        if(key == "v"){ //Vertex data
            iss >> x >> y >> z;
            Vector3f vertex(std::stof(x),std::stof(y),std::stof(z));
            mesh.vertices.push_back(vertex);
        }
        else if(key == "vn"){ //Normal data
            iss >> x >> y >> z;
            Vector3f normal(std::stof(x),std::stof(y),std::stof(z));
            mesh.normals.push_back(normal);
        }
        else if(key == "vt"){ //Texture data
            iss >> x >> y;
            Vector3f tex(std::stof(x), std::stof(y), 0);
            mesh.texels.push_back(tex);
        }
        else if(key == "f"){ //index data
            iss >> x >> y >> z;
            std::vector<std::string> splitX = splitStr(x,delimeter); 
            std::vector<std::string> splitY = splitStr(y,delimeter);
            std::vector<std::string> splitZ = splitStr(z,delimeter);
            for(int i = 0; i < splitX.size(); ++i){
                //Subtracted by 1 because OBJ files count indices starting by 1
                indices[i] = Vector3i(std::stoi(splitX[i])-1,std::stoi(splitY[i])-1,std::stoi(splitZ[i])-1);
            }
            mesh.vertexIndices.push_back(indices[0]);
            mesh.textureIndices.push_back(indices[1]);
            mesh.normalsIndices.push_back(indices[2]);
        }
    }
    mesh.numVertices = mesh.vertices.size();
    mesh.numFaces = mesh.vertexIndices.size();
    //Reset file in case you want to re-read it
    file.clear();
    file.seekg(0, file.beg);
}

//Creates stringstream of string and subdivides it by the delimeter and returns
//a vector of the individual components of the string
std::vector<std::string> OBJ::splitStr(std::string &str, char delim){
    std::stringstream ss(str);
    std::string token;
    std::vector<std::string> splitString;
    while(std::getline(ss,token,delim)){
        if(token == ""){
            //If token is empty just write 0 it will result in a -1 index
            //Since that index number is nonsensical you can catch it pretty easily later
            splitString.push_back("0"); 
        }
        else{
            splitString.push_back(token);
        }
    }
    return splitString;
}