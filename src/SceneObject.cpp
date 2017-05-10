//
//  SceneObject.cpp
//  ray-tracer
//
//  Created by Cody Rhoads on 4/4/17.
//
//

#include <stdio.h>

#include "SceneObject.hpp"
#include "LightSource.hpp"
#include "Ray.hpp"

using namespace std;
using namespace glm;

SceneObject::SceneObject() :
color(vec3(0)),
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
    vec3 localColor = vec3(0), reflectedColor = vec3(0);
    
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
    vec3 colorSum = vec3(0);
    const vec3 ka = color * ambient;
    const vec3 kd = color * diffuse;
    const vec3 ks = color * specular;
    vec3 N;
    if (getObjectType() == "Sphere") {
        N = getNormalAtPoint(ray->getIntersectionPoint());
    }
    else {
        N = getNormal();
    }
    const float power = (2.0/pow(roughness, 2.0)) - 2.0;
    
    for (unsigned int i = 0; i < lights.size(); i++) {
        int index = -1;
        const vec3 L = normalize(lights.at(i)->getLocation() - ray->getIntersectionPoint());
        
        shared_ptr<Ray> shadowTestRay;
        if (getObjectType() == "Sphere") {
            shadowTestRay = make_shared<Ray>(ray->getIntersectionPoint() + sphereEpsilon * L, L);
        }
        else {
            shadowTestRay = make_shared<Ray>(ray->getIntersectionPoint() + epsilon * L, L);
        }
        index = shadowTestRay->findClosestObjectIndex(objects);
        
        const float lightT = dot(normalize(shadowTestRay->getDirection()),
                                 lights.at(i)->getLocation() - shadowTestRay->getOrigin());
        
        if (index == -1 || shadowTestRay->getIntersectionTime() > lightT) {
            const vec3 V = normalize(-ray->getDirection());
            const vec3 H = normalize(V + L);
            
            const vec3 rd = kd * std::max(0.0f, dot(N, L));
            const vec3 rs = ks * pow(std::max(0.0f, dot(H, N)), power);
            
            colorSum += lights.at(i)->getColor() * (rd + rs);
        }
    }
    
    return ka + colorSum;
}

vec3 SceneObject::findLocalColorCookTorrance(const vector<shared_ptr<SceneObject>> &objects,
                                             const vector<shared_ptr<LightSource>> &lights,
                                             const shared_ptr<Ray> &ray)
{
    vec3 colorSum = vec3(0);
    const vec3 ka = color * ambient;
    const vec3 kd = color * diffuse;
    vec3 N;
    if (getObjectType() == "Sphere") {
        N = getNormalAtPoint(ray->getIntersectionPoint());
    }
    else {
        N = getNormal();
    }
    const float alpha = pow(roughness, 2);
    
    for (unsigned int i = 0; i < lights.size(); i++) {
        int index = -1;
        const vec3 L = normalize(lights.at(i)->getLocation() - ray->getIntersectionPoint());
        
        shared_ptr<Ray> shadowTestRay;
        if (getObjectType() == "Sphere") {
            shadowTestRay = make_shared<Ray>(ray->getIntersectionPoint() + sphereEpsilon * L, L);
        }
        else {
            shadowTestRay = make_shared<Ray>(ray->getIntersectionPoint() + epsilon * L, L);
        }
        index = shadowTestRay->findClosestObjectIndex(objects);
        
        const float lightT = dot(normalize(shadowTestRay->getDirection()),
                                 lights.at(i)->getLocation() - shadowTestRay->getOrigin());
        
        if (index == -1 || shadowTestRay->getIntersectionTime() > lightT) {
            const vec3 V = normalize(-ray->getDirection());
            const vec3 H = normalize(V + L);
            const float NdotH = dot(N, H);
            const float NdotV = dot(N, V);
            const float VdotH = dot(V, H);
            const float NdotL = dot(N, L);
            const vec3 rd = kd;
            
            const float exponent = (pow(NdotH, 2) - 1) / (pow(alpha, 2) * pow(NdotH, 2));
            const float D = (1/(M_PI * pow(alpha, 2))) * (exp(exponent)/pow(NdotH, 4));
            
            float G = std::min(1.0f, (2 * NdotH * NdotV)/VdotH);
            G = std::min(G, (2 * NdotH * NdotL)/VdotH);
            
            const float F0 = pow(ior - 1, 2)/pow(ior + 1, 2);
            const float F = F0 + (1 - F0) * pow(1 - VdotH, 5);
            
            const float rs = (D * G * F)/(4 * NdotL * NdotV);
            
            colorSum += lights.at(i)->getColor() * NdotL * ((1 - metallic) * rd + metallic * rs);
        }
    }
    
    return ka + colorSum;
}

vec3 SceneObject::findReflectedColor(const vector<shared_ptr<SceneObject>> &objects,
                                     const vector<shared_ptr<LightSource>> &lights,
                                     const shared_ptr<Ray> &ray, const int bouncesLeft,
                                     const string &BRDF)
{
    return vec3(0);
}

void SceneObject::printObjectInfo()
{
    printf("- Color: {%.4g %.4g %.4g}\n", color.x, color.y, color.z);
    printf("- Material:\n");
    printf("  - Ambient: %.4g\n", ambient);
    printf("  - Diffuse: %.4g\n", diffuse);
}
