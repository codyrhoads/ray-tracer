//
//  Camera.cpp
//  ray-tracer
//
//  Created by Cody Rhoads on 4/4/17.
//
//

#include <stdio.h>
#include <memory>
#include <vector>

#include "Camera.hpp"
#include "SceneObject.hpp"
#include "Ray.hpp"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

using namespace std;
using namespace glm;

Camera::Camera() :
location(vec3(0, 0, 0)),
up(vec3(0, 0, 0)),
right(vec3(0, 0, 0)),
lookAt(vec3(0, 0, 0))
{
    
}

Camera::Camera(const vec3 &location, const vec3 &up, const vec3 &right,
               const vec3 &lookAt) :
location(location),
up(up),
right(right),
lookAt(lookAt)
{
    
}

void Camera::setImageSize(int width, int height)
{
    imageWidth = width;
    imageHeight = height;
}

void Camera::render(const vector<shared_ptr<SceneObject>> objects)
{
    float t;
    int index, rgbIndex = 0;
    unsigned char *rgbData = new unsigned char[imageWidth*imageHeight*3];
    
    for (int j = imageHeight; j > 0; j--) {
        for (int i = 0; i < imageWidth; i++) {
            setCurrRay(i, j);
            index = getClosestObjectIndex(objects, t);
            
            if (index != -1) {
                // set pixel color to object color
                rgbData[rgbIndex++] = round(objects.at(index)->getColor().r * 255);
                rgbData[rgbIndex++] = round(objects.at(index)->getColor().g * 255);
                rgbData[rgbIndex++] = round(objects.at(index)->getColor().b * 255);
            }
            else {
                // set pixel to default color
                rgbData[rgbIndex++] = 0;
                rgbData[rgbIndex++] = 0;
                rgbData[rgbIndex++] = 0;
            }
        }
    }
    
    stbi_write_png("output.png", imageWidth, imageHeight, 3, rgbData,
                   sizeof(unsigned char)*3*imageWidth);
    
    delete[] rgbData;
}

void Camera::pixelRay(float pixelX, float pixelY)
{
    setCurrRay(pixelX, pixelY);
    
    printf("Pixel: [%.4g, %.4g] ", pixelX, pixelY);
    currRay->printRayInfo();
}

void Camera::firstHit(const vector<shared_ptr<SceneObject>> objects,
                      float pixelX, float pixelY)
{
    float t;
    
    pixelRay(pixelX, pixelY);
    
    int index = getClosestObjectIndex(objects, t);
    
    if (index != -1) {
        shared_ptr<SceneObject> obj = objects.at(index);
        printf("T = %.4g\n", t);
        printf("Object Type: %s\n", obj->getObjectType().c_str());
        printf("Color: %.4g %.4g %.4g\n", obj->getColor().r, obj->getColor().g,
               obj->getColor().b);
    }
    else {
        printf("No Hit\n");
    }
}

void Camera::printCameraInfo()
{
    printf("- Location: {%.4g %.4g %.4g}\n", location.x, location.y, location.z);
    printf("- Up: {%.4g %.4g %.4g}\n", up.x, up.y, up.z);
    printf("- Right: {%.4g %.4g %.4g}\n", right.x, right.y, right.z);
    printf("- Look at: {%.4g %.4g %.4g}\n", lookAt.x, lookAt.y, lookAt.z);
}

int Camera::getClosestObjectIndex(const vector<shared_ptr<SceneObject>> objects,
                                  float &t)
{
    int index = -1;
    
    for (int i = 0; i < objects.size(); i++) {
        float currT;
        if (objects.at(i)->testIntersection(currRay, currT)) {
            if (index == -1 || t > currT) {
                t = currT;
                index = i;
            }
        }
    }
    
    return index;
}

void Camera::setCurrRay(float pixelX, float pixelY)
{
    float screenU, screenV;
    vec3 screenIntersect, dir;
    
    screenU = (pixelX + 0.5)/imageWidth - 0.5;
    screenV = (pixelY + 0.5)/imageHeight - 0.5;
    
    screenIntersect = location + screenU * right + screenV * up
                      + normalize(lookAt - location);
    dir = normalize(screenIntersect - location);
    
    currRay = make_shared<Ray>(location, dir);
}
