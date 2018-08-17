// ===============================
// AUTHOR       : Angel Ortiz (angelo12 AT vt DOT edu)
// CREATE DATE  : 2018-07-10
// ===============================

//Headers
#include "scene.h"
#include "objParser.h"
#include <fstream>
#include <sstream>
#include <sys/stat.h>
#include <sys/types.h>

Scene::Scene(const std::string &sceneName){
    //Building all the useful path strings
    std::string folderPath = "../scenes/" + sceneName;
    std::string baseFilePath = folderPath + "/" + sceneName;
    
    if( !findSceneFolder(folderPath)){
        //If you do not find the scene folder quit
        emptyScene = true; 
    }
    else{
        //Load all cameras, models and lights and return false if it fails
        emptyScene = !loadContent(baseFilePath, sceneName);
    }
}

Scene::~Scene(){
    //Making sure you don't attempt to delete models that don't exist
    if (!emptyScene){
        for(Model *models : modelsInScene){
            delete models;
        }
        delete [] lights;
    }
}

//Update Order is critical for correct culling
void Scene::update(unsigned int deltaT){
    mainCamera.update(deltaT);
    for(Model *models : modelsInScene){
        models->update();
    }
    frustrumCulling();
}
//-----------------------------GETTERS----------------------------------------------
std::queue<Model*>* Scene::getVisiblemodels(){
    return &visibleModels;
}
Camera* Scene::getCurrentCamera(){
    return &mainCamera;
}
BaseLight * Scene::getCurrentLights(){
    return lights;
}
int Scene::getLightCount(){
    return lightCount;
}
//----------------------------------------------------------------

bool Scene::checkIfEmpty(){
    return emptyScene;
}

//-----------------------------SCENE LOADING-----------------------------------

//TODO: separate into new class or function
//Config file parsing, gets all the important 
bool Scene::loadContent(const std::string &baseFilePath, const std::string &sceneName ){
    std::string configFilePath = baseFilePath + "_config.txt";
    std::ifstream file(configFilePath.c_str());
    TransformParameters initParameters;

    //Begin config file parsing
    if(!file.good()){
        //Check config file exists
        printf("Error! Config: %s does not exist.\n",configFilePath.c_str());
        return false;
    }
    else{
        //Checking that config file belongs to current scene and is properly formatted
        std::string line, key, x, y, z;
        std::getline(file,line);
        std::istringstream iss(line);
        iss >> key;
        if(key != "s"){
            printf("Error! Config file: %s is not properly formatted.\n",configFilePath.c_str());
            return false;
        }
        else{
            iss >> key;
            if(key != sceneName){
                //Checks the config file belongs to the correct scene
                printf("Error! Config file: %s does not belong to current scene.\n",configFilePath.c_str());
                return false;
            }
            else{
                //Now we can parse the rest of the file "safely"
                while(!file.eof()){
                    std::getline(file,line);
                    std::istringstream iss(line);
                    iss >> key;
                    //MODEL SETUP
                    if(key == "m"){ 
                        printf("Loading models...\n");
                        iss >> key;
                        int max = stoi(key);
                        for(int i = 0; i < max; ++i){

                            //Burn one line with model type for now
                            std::getline(file,line);

                            //Position
                            std::getline(file,line);
                            std::istringstream pos(line);
                            pos >> key >> x >> y >> z;
                            initParameters.translation = Vector3f(stof(x), stof(y), stof(z));

                            //Rotation
                            std::getline(file,line);
                            std::istringstream rot(line);
                            rot >> key >> x >> y >> z;
                            initParameters.rotation = Vector3f(stof(x)*M_PI/180.0f, stof(y)*M_PI/180.0f, stof(z)*M_PI/180.0f);

                            //Scaling
                            std::getline(file,line);
                            std::istringstream sca(line);
                            sca >> key >> x >> y >> z;
                            initParameters.scaling = Vector3f(stof(x), stof(y), stof(z));

                            //Burning empty line that makes the config easier to read
                            std::getline(file,line);
                            
                            //Attempts to load model with the initparameters it has read
                            loadSceneModel(baseFilePath, initParameters);
                        }
                    }
                    //LIGHT SETUP
                    else if(key == "l"){ 
                        printf("Loading lights...\n");
                        iss >> key;
                        lightCount = stoi(key);
                        //Initializes light array
                        lights = new BaseLight[lightCount];
                        for(int i = 0; i < lightCount; ++i){

                            //Burn one line with light type for now
                            std::getline(file,line);

                            //Position
                            std::getline(file,line);
                            std::istringstream pos(line);
                            pos >> key >> x >> y >> z;
                            lights[i].position = Vector3f(stof(x), stof(y), stof(z));

                            //Color
                            std::getline(file,line);
                            std::istringstream col(line);
                            col >> key >> x >> y >> z;
                            lights[i].color = Vector3f(stof(x), stof(y), stof(z));

                            //Burning empty line that makes the config easier to read
                            std::getline(file,line);
                        }
                    }
                }
                //Lastly we check if the scene is empty and return 
                return !modelsInScene.empty();       
            }
        }
    }
}

bool Scene::findSceneFolder(const std::string &scenePath){
    struct stat info;
    //folder is blocking access
    if( stat( scenePath.c_str(), &info ) != 0 ){
        printf( "cannot access %s\n", scenePath.c_str() );
         return false;
    }
    else if( info.st_mode & S_IFDIR ){
        //Folder is accessible
        printf( "%s is a valid scene\n", scenePath.c_str() );
        return true;
    }
    else{
        //Folder does not exist
        printf("Error! Scene: %s does not exist.\n",scenePath.c_str());
        return false;
    }
}

void Scene::loadSceneModel(const std::string &baseFilePath, const TransformParameters &init){
    std::string meshFilePath = baseFilePath + "_mesh.obj";
    if(!OBJ::fileExists(meshFilePath)){
        //If the mesh deos not exist it's very likely nothing else does, quit early
        printf("Error! Mesh: %s does not exist.\n",meshFilePath.c_str());
    }
    else{
        printf( "%s is a valid mesh\n", meshFilePath.c_str() );
        modelsInScene.push_back(new Model(baseFilePath, init));
    }
}

//-------------------------------------------------------------

void Scene::frustrumCulling(){
    for(Model *model : modelsInScene){
        bool visible = mainCamera.checkVisibility(model->getBounds());
        if (visible) {
            visibleModels.push(model);
        }
    }
}












