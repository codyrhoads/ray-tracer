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
#include "Shader.hpp"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"
#include "OptionalArgs.hpp"

using namespace std;
using namespace glm;

Camera::Camera() :
location(vec3(0)),
up(vec3(0)),
right(vec3(0)),
lookAt(vec3(0))
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

void Camera::render(const vector<shared_ptr<SceneObject>> &objects,
                    const vector<shared_ptr<LightSource>> &lights,
                    const OptionalArgs &optArgs)
{
    int rgbIndex = 0;
    unsigned char *rgbData = new unsigned char[imageWidth * imageHeight * 3];
    string trace = "";
    Shader shader = Shader(objects, lights, optArgs, false);
    shared_ptr<SceneObject> hitObj = nullptr;
    
    for (int j = imageHeight - 1; j >= 0; j--) {
        for (int i = 0; i < imageWidth; i++) {
            // superSampleN > 0 means that we are doing super sampling
            if (optArgs.superSampleN > 0) {
                vec3 totalColor = vec3(0);
                
                // super sampling
                for (int n = 0; n < optArgs.superSampleN; n++) {
                    for (int m = 0; m < optArgs.superSampleN; m++) {
                        setCurrRaySuperSampling(i, j, m, n, optArgs.superSampleN);
                        hitObj = currRay->findClosestObject(objects, -1);
                        
                        if (hitObj != nullptr) {
                            vec3 color = shader.getShadedColor(currRay, 0, trace);
                            totalColor += color;
                        }
                    }
                }
                
                // take the average color of all the super samples
                totalColor /= pow(optArgs.superSampleN, 2);
                
                // set pixel color to average object color
                rgbData[rgbIndex++] = round(std::min(totalColor.r, 1.0f) * 255);
                rgbData[rgbIndex++] = round(std::min(totalColor.g, 1.0f) * 255);
                rgbData[rgbIndex++] = round(std::min(totalColor.b, 1.0f) * 255);
            }
            else {
                setCurrRay(i, j);
                hitObj = currRay->findClosestObject(objects, -1);
                
                vec3 color = vec3(0);
                if (hitObj != nullptr) {
                    color = shader.getShadedColor(currRay, 0, trace);
                }
                
                // set pixel color to object color
                rgbData[rgbIndex++] = round(std::min(color.r, 1.0f) * 255);
                rgbData[rgbIndex++] = round(std::min(color.g, 1.0f) * 255);
                rgbData[rgbIndex++] = round(std::min(color.b, 1.0f) * 255);
            }
            
            hitObj = nullptr;
        }
    }
    
    stbi_write_png("output.png", imageWidth, imageHeight, 3, rgbData,
                   sizeof(unsigned char) * 3 * imageWidth);
    
    delete[] rgbData;
}

void Camera::pixelRay(const float pixelX, const float pixelY)
{
    setCurrRay(pixelX, pixelY);
    
    printf("Pixel: [%.4g, %.4g] ", pixelX, pixelY);
    printf("%s\n", currRay->getRayInfo().c_str());
}

void Camera::firstHit(const vector<shared_ptr<SceneObject>> &objects,
                      const float pixelX, const float pixelY)
{
    pixelRay(pixelX, pixelY);
    
    shared_ptr<SceneObject> hitObj = currRay->findClosestObject(objects, -1);
    
    if (hitObj != nullptr) {
        printf("T = %.4g\n", currRay->getIntersectionTime());
        printf("Object Type: %s\n", hitObj->getObjectType().c_str());
        printf("Color: %.4g %.4g %.4g\n", hitObj->getColor().r, hitObj->getColor().g,
               hitObj->getColor().b);
    }
    else {
        printf("No Hit\n");
    }
}

void Camera::pixelColor(const vector<shared_ptr<SceneObject>> &objects,
                        const vector<shared_ptr<LightSource>> &lights,
                        const float pixelX, const float pixelY,
                        const OptionalArgs &optArgs)
{
    string trace = "";
    Shader shader = Shader(objects, lights, optArgs, false);
    
    pixelRay(pixelX, pixelY);
    
    shared_ptr<SceneObject> hitObj = currRay->findClosestObject(objects, -1);
    
    if (hitObj != nullptr) {
        vec3 color;
        color = shader.getShadedColor(currRay, 0, trace);
        
        printf("T = %.4g\n", currRay->getIntersectionTime());
        printf("Object Type: %s\n", hitObj->getObjectType().c_str());
        printf("BRDF: %s\n", (optArgs.BRDF).c_str());
        printf("Color: (%.4g, %.4g, %.4g)\n",
               round(std::min(color.r, 1.0f) * 255),
               round(std::min(color.g, 1.0f) * 255),
               round(std::min(color.b, 1.0f) * 255));
    }
    else {
        printf("No Hit\n");
    }
}

void Camera::printRays(const vector<shared_ptr<SceneObject>> &objects,
                       const vector<shared_ptr<LightSource>> &lights,
                       const float pixelX, const float pixelY,
                       const OptionalArgs &optArgs)
{
    string trace = "";
    Shader shader = Shader(objects, lights, optArgs, true);
    
    setCurrRay(pixelX, pixelY);
    
    shared_ptr<SceneObject> hitObj = currRay->findClosestObject(objects, -1);
    
    if (hitObj != nullptr) {
        vec3 color = shader.getShadedColor(currRay, 0, trace);
        
        printf("Pixel: [%.4g, %.4g] ", pixelX, pixelY);
        printf("Color: (%.4g, %.4g, %.4g)\n",
               round(std::min(color.r, 1.0f) * 255),
               round(std::min(color.g, 1.0f) * 255),
               round(std::min(color.b, 1.0f) * 255));
        printf("----");
        printf("\n  Iteration type: Primary");
        printf("%s\n", trace.c_str());
        printf("\n\n--------------------------------------------------------------------------------\n");
    }
    else {
        printf("No Hit\n");
    }
}

void Camera::printCameraInfo() const
{
    printf("- Location: {%.4g %.4g %.4g}\n", location.x, location.y, location.z);
    printf("- Up: {%.4g %.4g %.4g}\n", up.x, up.y, up.z);
    printf("- Right: {%.4g %.4g %.4g}\n", right.x, right.y, right.z);
    printf("- Look at: {%.4g %.4g %.4g}\n", lookAt.x, lookAt.y, lookAt.z);
}

void Camera::setCurrRay(const int pixelX, const int pixelY)
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

void Camera::setCurrRaySuperSampling(const int pixelX, const int pixelY,
                                     const int subPixelX, const int subPixelY,
                                     const int numSubPixels)
{
    float screenU, screenV;
    vec3 screenIntersect, dir;
    
    screenU = (pixelX + ((subPixelX + 0.5)/numSubPixels))/imageWidth - 0.5;
    screenV = (pixelY + ((subPixelY + 0.5)/numSubPixels))/imageHeight - 0.5;
    
    screenIntersect = location + screenU * right + screenV * up
    + normalize(lookAt - location);
    dir = normalize(screenIntersect - location);
    
    currRay = make_shared<Ray>(location, dir);
}
