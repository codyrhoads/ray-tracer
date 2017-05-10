//
//  SceneObject.cpp
//  ray-tracer
//
//  Created by Cody Rhoads on 4/4/17.
//
//

#include <stdio.h>

#include "SceneObject.hpp"

using namespace std;
using namespace glm;

SceneObject::SceneObject() :
color(vec3(0, 0, 0)),
ambient(0),
diffuse(0),
specular(0.1),
reflection(0),
roughness(0.1),
metallic(0.1),
ior(1.6)
{
    
}

SceneObject::SceneObject(const vec3 &color, const float ambient, const float diffuse,
                         const float specular, const float reflection,
                         const float roughness, const float metallic, const float ior) :
color(color),
ambient(ambient),
diffuse(diffuse),
specular(specular),
reflection(reflection),
roughness(roughness),
metallic(metallic),
ior(ior)
{
    
}

bool SceneObject::testIntersection(const shared_ptr<Ray> &ray, float &t)
{
    return false;
}

vec3 SceneObject::getShadedColor(const vector<shared_ptr<SceneObject>> &objects,
                                 const vector<shared_ptr<LightSource>> &lights,
                                 const shared_ptr<Ray> &ray, const int bouncesLeft,
                                 const string &BRDF)
{
    vec3 localColor = vec3(0, 0, 0), reflectedColor = vec3(0, 0, 0);
    
    if (BRDF == "Blinn-Phong") {
        localColor = findLocalColorBlinnPhong(objects, lights, ray);
    }
    else {
        localColor = findLocalColorCookTorrance(objects, lights, ray);
    }
    
    if (bouncesLeft > 0) {
        if (reflection > 0) {
            reflectedColor = findReflectedColor(objects, lights, ray, bouncesLeft - 1, BRDF);
        }
        
        // add refraction here
    }
    
    return (1 - reflection) * localColor + reflection * reflectedColor;
}


vec3 SceneObject::findLocalColorBlinnPhong(const vector<shared_ptr<SceneObject>> &objects,
                                           const vector<shared_ptr<LightSource>> &lights,
                                           const shared_ptr<Ray> &ray)
{
    return vec3(0, 0, 0);
}

vec3 SceneObject::findLocalColorCookTorrance(const vector<shared_ptr<SceneObject>> &objects,
                                             const vector<shared_ptr<LightSource>> &lights,
                                             const shared_ptr<Ray> &ray)
{
    return vec3(0, 0, 0);
}

vec3 SceneObject::findReflectedColor(const vector<shared_ptr<SceneObject>> &objects,
                                     const vector<shared_ptr<LightSource>> &lights,
                                     const shared_ptr<Ray> &ray, const int bouncesLeft,
                                     const string &BRDF)
{
    return vec3(0, 0, 0);
}

void SceneObject::printObjectInfo()
{
    printf("- Color: {%.4g %.4g %.4g}\n", color.x, color.y, color.z);
    printf("- Material:\n");
    printf("  - Ambient: %.4g\n", ambient);
    printf("  - Diffuse: %.4g\n", diffuse);
}
