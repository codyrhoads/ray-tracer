#include <stdio.h>
#include <fstream>
#include <string>
#include <vector>
#include <memory>

#include "FileParser.hpp"
#include "SceneObject.hpp"
#include "Camera.hpp"
#include "LightSource.hpp"

using namespace std;

enum Command {
    RENDER, SCENEINFO, PIXELRAY, FIRSTHIT, PIXELCOLOR, PIXELTRACE
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
    string BRDF = "Blinn-Phong";
    
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
        if (argc > 5 && !strcmp(argv[5], "-altbrdf")) {
            BRDF = "Alternate";
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
    else if (!strcmp(argv[1], "pixelcolor")) {
        command = PIXELCOLOR;
        if (argc < 7) {
            printf("Not enough arguments for pixelcolor.\n");
            return 0;
        }
        if (argc > 7 && !strcmp(argv[7], "-altbrdf")) {
            BRDF = "Alternate";
        }
    }
    else if (!strcmp(argv[1], "pixeltrace")) {
        command = PIXELTRACE;
        if (argc < 7) {
            printf("Not enough arguments for pixeltrace.\n");
            return 0;
        }
        if (argc > 7 && !strcmp(argv[7], "-altbrdf")) {
            BRDF = "Alternate";
        }
    }
    else {
        printf("Unrecognized command.\n");
        return 0;
    }
    
    // Parse the input file, which should always be located at index 2.
    parser.parse(string(argv[2]));
    camera = parser.getCamera();
    if (command != SCENEINFO) {
        camera->setImageSize(atoi(argv[3]), atoi(argv[4]));
    }
    lights = parser.getLights();
    objects = parser.getObjects();
    
    if (command == RENDER) {
        camera->render(objects, lights, BRDF);
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
        camera->pixelColor(objects, lights, atof(argv[5]), atof(argv[6]), BRDF);
    }
    else if (command == PIXELTRACE) {
        // argv[5] and argv[6] are the x and y coordinates of the pixel to test,
        // respectively.
        camera->pixelTrace(objects, lights, atof(argv[5]), atof(argv[6]), BRDF);
    }
    
    return 0;
}
