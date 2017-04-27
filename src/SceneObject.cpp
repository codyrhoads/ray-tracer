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
specular(0),
roughness(0.0001)
{
    
}

SceneObject::SceneObject(const vec3 &color, const float ambient,
                         const float diffuse, const float specular,
                         const float roughness) :
color(color),
ambient(ambient),
diffuse(diffuse),
specular(specular),
roughness(roughness)
{
    
}

bool SceneObject::testIntersection(const shared_ptr<Ray> &ray, float &t)
{
    return false;
}

vec3 SceneObject::getColorBlinnPhong(const vector<shared_ptr<SceneObject>> &objects,
                                     const vector<shared_ptr<LightSource>> &lights,
                                     const shared_ptr<Ray> &ray)
{    
    return vec3(0, 0, 0);
}

vec3 SceneObject::getColorCookTorrance(const vector<shared_ptr<SceneObject>> &objects,
                                       const vector<shared_ptr<LightSource>> &lights,
                                       const shared_ptr<Ray> &ray)
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

string SceneObject::getObjectType()
{
    return "NULL";
}
