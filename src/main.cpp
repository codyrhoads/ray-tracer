#include <stdio.h>
#include <fstream>
#include <string>
#include <vector>
#include <memory>

#include "FileParser.hpp"
#include "SceneObject.hpp"
#include "Camera.hpp"
#include "LightSource.hpp"
#include "OptionalArgs.hpp"

using namespace std;

enum Command {
    RENDER, SCENEINFO, PIXELRAY, FIRSTHIT, PIXELCOLOR, PRINTRAYS
};

int main(int argc, char **argv)
{
    FileParser parser = FileParser();
    ifstream file;
    string line;
    shared_ptr<Camera> camera;
    vector<shared_ptr<LightSource>> lights;
    vector<shared_ptr<SceneObject>> objects;
    Command command;
    OptionalArgs optArgs;
    
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
        optArgs = OptionalArgs(argv, argc, 5);
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
    else if (!strcmp(argv[1], "pixelcolor")) {
        command = PIXELCOLOR;
        if (argc < 7) {
            printf("Not enough arguments for pixelcolor.\n");
            return 0;
        }
        optArgs = OptionalArgs(argv, argc, 7);
    }
    else if (!strcmp(argv[1], "printrays")) {
        command = PRINTRAYS;
        if (argc < 7) {
            printf("Not enough arguments for printrays.\n");
            return 0;
        }
        optArgs = OptionalArgs(argv, argc, 7);
    }
    else {
        printf("Unrecognized command.\n");
        return 0;
    }
    
    // Parse the input file, which should always be located at index 2.
    parser.parse(string(argv[2]), optArgs.useBVH);
    camera = parser.getCamera();
    if (command != SCENEINFO) {
        camera->setImageSize(atoi(argv[3]), atoi(argv[4]));
    }
    lights = parser.getLights();
    objects = parser.getObjects();
    
    if (command == RENDER) {
        camera->render(objects, lights, optArgs);
    }
    else if (command == SCENEINFO) {
        printf("Camera:\n");
        camera->printCameraInfo();
        
        printf("\n---\n\n");
        
        printf("%d light(s)\n\n", (int)lights.size());
        for (unsigned int i = 0; i < lights.size(); i++) {
            printf("Light[%d]:\n", i);
            lights.at(i)->printLightInfo();
            printf("\n");
        }
        
        printf("---\n\n");
        
        printf("%d object(s)\n\n", (int)objects.size());
        for (unsigned int i = 0; i < objects.size(); i++) {
            printf("Object[%d]:\n", i);
            objects.at(i)->printObjectInfo();
            printf("\n");
        }
    }
    else if (command == PIXELRAY) {
        // argv[5] and argv[6] are the x and y coordinates of the pixel to test,
        // respectively.
        camera->pixelRay(atof(argv[5]), atof(argv[6]));
    }
    else if (command == FIRSTHIT) {
        // argv[5] and argv[6] are the x and y coordinates of the pixel to test,
        // respectively.
        camera->firstHit(objects, atof(argv[5]), atof(argv[6]));
    }
    else if (command == PIXELCOLOR) {
        // argv[5] and argv[6] are the x and y coordinates of the pixel to test,
        // respectively.
        camera->pixelColor(objects, lights, atof(argv[5]), atof(argv[6]), optArgs);
    }
    else if (command == PRINTRAYS) {
        // argv[5] and argv[6] are the x and y coordinates of the pixel to test,
        // respectively.
        camera->printRays(objects, lights, atof(argv[5]), atof(argv[6]), optArgs);
    }
    
    return 0;
}
