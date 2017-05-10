//
//  Camera.cpp
//  ray-tracer
//
//  Created by Cody Rhoads on 4/4/17.
//
//

#include <stdio.h>

#include "Camera.hpp"
#include "SceneObject.hpp"
#include "Ray.hpp"
#include "LightSource.hpp"
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

void Camera::setImageSize(const int width, const int height)
{
    imageWidth = width;
    imageHeight = height;
}

void Camera::renderBlinnPhong(const vector<shared_ptr<SceneObject>> &objects,
                              const vector<shared_ptr<LightSource>> &lights)
{
    int index, rgbIndex = 0;
    unsigned char *rgbData = new unsigned char[imageWidth * imageHeight * 3];
    
    for (int j = imageHeight - 1; j >= 0; j--) {
        for (int i = 0; i < imageWidth; i++) {
            setCurrRay(i, j);
            index = currRay->getClosestObjectIndex(objects);
            
            if (index != -1) {
                vec3 color = objects.at(index)->getColorBlinnPhong(objects, lights, currRay);
                // set pixel color to object color
                rgbData[rgbIndex++] = round(std::min(color.r, 1.0f) * 255);
                rgbData[rgbIndex++] = round(std::min(color.g, 1.0f) * 255);
                rgbData[rgbIndex++] = round(std::min(color.b, 1.0f) * 255);
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

void Camera::renderCookTorrance(const vector<shared_ptr<SceneObject>> &objects,
                                const vector<shared_ptr<LightSource>> &lights)
{
    int index, rgbIndex = 0;
    unsigned char *rgbData = new unsigned char[imageWidth*imageHeight*3];
    
    for (int j = imageHeight - 1; j >= 0; j--) {
        for (int i = 0; i < imageWidth; i++) {
            setCurrRay(i, j);
            index = currRay->getClosestObjectIndex(objects);
            
            if (index != -1) {
                vec3 color = objects.at(index)->getColorCookTorrance(objects,
                                                                     lights,
                                                                     currRay);
                
                // set pixel color to object color
                rgbData[rgbIndex++] = round(std::min(color.r, 1.0f) * 255);
                rgbData[rgbIndex++] = round(std::min(color.g, 1.0f) * 255);
                rgbData[rgbIndex++] = round(std::min(color.b, 1.0f) * 255);
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

void Camera::pixelRay(const float pixelX, const float pixelY)
{
    setCurrRay(pixelX, pixelY);
    
    printf("Pixel: [%.4g, %.4g] ", pixelX, pixelY);
    currRay->printRayInfo();
}

void Camera::firstHit(const vector<shared_ptr<SceneObject>> &objects,
                      const float pixelX, const float pixelY)
{
    pixelRay(pixelX, pixelY);
    
    int index = currRay->getClosestObjectIndex(objects);
    
    if (index != -1) {
        shared_ptr<SceneObject> obj = objects.at(index);
        printf("T = %.4g\n", currRay->getIntersectionTime());
        printf("Object Type: %s\n", obj->getObjectType().c_str());
        printf("Color: %.4g %.4g %.4g\n", obj->getColor().r, obj->getColor().g,
               obj->getColor().b);
    }
    else {
        printf("No Hit\n");
    }
}

void Camera::pixelColor(const vector<shared_ptr<SceneObject>> &objects,
                        const vector<shared_ptr<LightSource>> &lights,
                        const float pixelX, const float pixelY,
                        const string BRDF)
{
    pixelRay(pixelX, pixelY);
    
    int index = currRay->getClosestObjectIndex(objects);
    
    if (index != -1) {
        vec3 color;
        shared_ptr<SceneObject> obj = objects.at(index);
        if (BRDF == "Blinn-Phong") {
            color = obj->getColorBlinnPhong(objects, lights, currRay);
        }
        else {
            color = obj->getColorCookTorrance(objects, lights, currRay);
        }
        
        printf("T = %.4g\n", currRay->getIntersectionTime());
        printf("Object Type: %s\n", obj->getObjectType().c_str());
        printf("BRDF: %s\n", BRDF.c_str());
        printf("Color: (%.4g, %.4g, %.4g)\n",
               round(std::min(color.r, 1.0f) * 255),
               round(std::min(color.g, 1.0f) * 255),
               round(std::min(color.b, 1.0f) * 255));
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

void Camera::setCurrRay(const float pixelX, const float pixelY)
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
