#include <stdio.h>
#include <fstream>
#include <string>
#include <vector>

#include "FileParser.hpp"
#include "SceneObject.hpp"
#include "Camera.hpp"
#include "LightSource.hpp"

using namespace std;

enum Command {
    RENDER, SCENEINFO, PIXELRAY, FIRSTHIT
};

int main(int argc, char **argv) {
    FileParser parser = FileParser();
    ifstream file;
    string line;
    shared_ptr<Camera> camera;
    vector<shared_ptr<LightSource>> lights;
    vector<shared_ptr<SceneObject>> objects;
    Command command;
    
    if (argc < 2) {
        printf("Need a command.\n");
        return 0;
    }
    
    if (!strcmp(argv[1], "render")) {
        command = RENDER;
        if (argc < 5) {
            printf("Not enough arguments for render.\n");
            return 0;
        }
    }
    else if (!strcmp(argv[1], "sceneinfo")) {
        command = SCENEINFO;
        if (argc < 3) {
            printf("Not enough arguments for sceneinfo.\n");
            return 0;
        }
    }
    else if (!strcmp(argv[1], "pixelray")) {
        command = PIXELRAY;
        if (argc < 7) {
            printf("Not enough arguments for pixelray.\n");
            return 0;
        }
    }
    else if (!strcmp(argv[1], "firsthit")) {
        command = FIRSTHIT;
        if (argc < 7) {
            printf("Not enough arguments for firsthit.\n");
            return 0;
        }
    }
    else {
        printf("Unrecognized command.\n");
        return 0;
    }
    
    // Parse the input file, which should always be located at index 2.
    parser.parse(string(argv[2]));
    camera = parser.getCamera();
    lights = parser.getLights();
    objects = parser.getObjects();
    
    if (command == RENDER) {
        
    }
    else if (command == SCENEINFO) {
        printf("Camera:\n");
        camera->print();
        printf("\n");
        
        printf("%d light(s)\n", (int)lights.size());
        for (int i = 0; i < lights.size(); i++) {
            printf("Light[%d]:\n", i);
            lights.at(i)->print();
            printf("\n");
        }
        
        printf("%d object(s)\n", (int)objects.size());
        for (int i = 0; i < objects.size(); i++) {
            printf("Object[%d]:\n", i);
            objects.at(i)->print();
            printf("\n");
        }
    }
    else if (command == PIXELRAY) {
        
    }
    else {
        
    }
    
    return 0;
}
