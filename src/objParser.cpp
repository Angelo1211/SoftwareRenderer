#include "objParser.h"

Mesh& OBJ::buildMeshFromFile(Mesh &mesh, std::string &path){

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
    Vector3 indices[3];
    char delimeter = '/';
    while(!file.eof()){
        std::getline(file,line);
        std::istringstream iss(line);
        iss >> key;
        if(key == "v"){ //Vertex data
            iss >> x >> y >> z;
            Vector3 vertex(std::stof(x),std::stof(y),std::stof(z));
            mesh.vertices.push_back(vertex);
        }
        else if(key == "vn"){ //Normal data
            iss >> x >> y >> z;
            Vector3 normal(std::stof(x),std::stof(y),std::stof(z));
            mesh.normals.push_back(normal);
        }
        else if(key == "f"){ //index data
            iss >> x >> y >> z;
            std::vector<std::string> splitX = split(x,delimeter); 
            std::vector<std::string> splitY = split(y,delimeter);
            std::vector<std::string> splitZ = split(z,delimeter);
            for(int i = 0; i < splitX.size(); ++i){
                //printf("%s\n",splitX[i].c_str());
                indices[i] = Vector3(std::stof(splitX[i])-1,std::stof(splitY[i])-1,std::stof(splitZ[i])-1);
            }
            printf("\n");
            mesh.vertexIndices.push_back(indices[0]);
            mesh.textureIndices.push_back(indices[1]);
            mesh.normalsIndices.push_back(indices[2]);
        }
    }
    mesh.numVertices = mesh.vertices.size();
    mesh.numFaces = mesh.vertexIndices.size();
    file.clear();
    file.seekg(0, file.beg);
}

std::vector<std::string> OBJ::split(std::string &str, char delim){
    std::stringstream ss(str);
    std::string token;
    std::vector<std::string> splitString;
    while(std::getline(ss,token,delim)){
        if(token == ""){
            //If token is empty just write -1
            //Since you cannot have an idnex of that size anyway
            splitString.push_back("-1"); 
        }
        else{
            splitString.push_back(token);
        }
    }
    return splitString;
}