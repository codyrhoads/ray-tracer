//
//  Shader.cpp
//  raytrace
//
//  Created by Cody Rhoads on 5/10/17.
//
//

#include "Shader.hpp"
#include "SceneObject.hpp"
#include "LightSource.hpp"
#include "Ray.hpp"

#define MAX_BOUNCES 6
#define SPHERE_EPSILON 0.001f
#define EPSILON 0.0001f

using namespace std;
using namespace glm;

vec3 Shader::getShadedColor(const vector<shared_ptr<SceneObject>> &objects,
                            const vector<shared_ptr<LightSource>> &lights,
                            const shared_ptr<Ray> &ray, const int bounces,
                            const string &BRDF, string &trace)
{
    vec3 localColor = vec3(0), reflectedColor = vec3(0), refractedColor = vec3(0);
    string indent = "";
    shared_ptr<SceneObject> obj = objects.at(ray->getIndexOfIntersectedObject());
    
    if (BRDF == "Blinn-Phong") {
        localColor = findLocalColorBlinnPhong(objects, lights, ray);
    }
    else {
        localColor = findLocalColorCookTorrance(objects, lights, ray);
    }
    
    for (int i = 0; i < bounces; i++) {
        indent += "| ";
    }
    trace += "\n" + indent + "|   Normal " + obj->getNormalAtPointString(ray->getIntersectionPoint());
    trace += "\n" + indent + "|   Transformed " + ray->getRayInfo();
    trace += "\n" + indent + "|   Ambient: " + obj->getAmbientString();
    trace += "\n" + indent + "|   Diffuse: " + obj->getDiffuseString();
    trace += "\n" + indent + "|   Specular: " + obj->getSpecularString();
    
    if (bounces < MAX_BOUNCES) {
        if (obj->getFilter() > 0) {
            refractedColor = findRefractedColor(objects, lights, ray, bounces + 1, BRDF, trace);
        }
        
        if (obj->getReflection() > 0) {
            reflectedColor = obj->getColor() * findReflectedColor(objects, lights, ray,
                                                                  bounces + 1, BRDF, trace);
        }
    }
    
    vec3 N;
    if (obj->getObjectType() == "Sphere") {
        N = obj->getNormalAtPoint(ray->getIntersectionPoint());
    }
    else {
        N = obj->getNormal();
    }
    const float fresnelReflectance = schlicksApproximation(obj->getIOR(), N, -ray->getDirection());
    
    const float localContribution = (1 - obj->getFilter()) * (1 - obj->getReflection());
    const float reflectionContribution = (1 - obj->getFilter()) * obj->getReflection() +
                                         obj->getFilter() * fresnelReflectance;
    const float refractionContribution = obj->getFilter() * (1 - fresnelReflectance);
    
    return localContribution * localColor +
           reflectionContribution * reflectedColor +
           refractionContribution * refractedColor;
}

vec3 Shader::findLocalColorBlinnPhong(const vector<shared_ptr<SceneObject>> &objects,
                                      const vector<shared_ptr<LightSource>> &lights,
                                      const shared_ptr<Ray> &ray)
{
    vec3 colorSum = vec3(0);
    shared_ptr<SceneObject> obj = objects.at(ray->getIndexOfIntersectedObject());
    const vec3 ka = obj->getColor() * obj->getAmbient();
    const vec3 kd = obj->getColor() * obj->getDiffuse();
    const vec3 ks = obj->getColor() * obj->getSpecular();
    vec3 N;
    if (obj->getObjectType() == "Sphere") {
        N = obj->getNormalAtPoint(ray->getIntersectionPoint());
    }
    else {
        N = obj->getNormal();
    }
    const float power = (2.0/pow(obj->getRoughness(), 2.0)) - 2.0;
    
    for (unsigned int i = 0; i < lights.size(); i++) {
        int index = -1;
        const vec3 L = normalize(lights.at(i)->getLocation() - ray->getIntersectionPoint());
        
        shared_ptr<Ray> shadowTestRay;
        if (obj->getObjectType() == "Sphere") {
            shadowTestRay = make_shared<Ray>(ray->getIntersectionPoint() + SPHERE_EPSILON * L, L);
        }
        else {
            shadowTestRay = make_shared<Ray>(ray->getIntersectionPoint() + EPSILON * L, L);
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

vec3 Shader::findLocalColorCookTorrance(const vector<shared_ptr<SceneObject>> &objects,
                                        const vector<shared_ptr<LightSource>> &lights,
                                        const shared_ptr<Ray> &ray)
{
    vec3 colorSum = vec3(0);
    shared_ptr<SceneObject> obj = objects.at(ray->getIndexOfIntersectedObject());
    const vec3 ka = obj->getColor() * obj->getAmbient();
    const vec3 kd = obj->getColor() * obj->getDiffuse();
    vec3 N;
    if (obj->getObjectType() == "Sphere") {
        N = obj->getNormalAtPoint(ray->getIntersectionPoint());
    }
    else {
        N = obj->getNormal();
    }
    const float alpha = pow(obj->getRoughness(), 2);
    
    for (unsigned int i = 0; i < lights.size(); i++) {
        int index = -1;
        const vec3 L = normalize(lights.at(i)->getLocation() - ray->getIntersectionPoint());
        
        shared_ptr<Ray> shadowTestRay;
        if (obj->getObjectType() == "Sphere") {
            shadowTestRay = make_shared<Ray>(ray->getIntersectionPoint() + SPHERE_EPSILON * L, L);
        }
        else {
            shadowTestRay = make_shared<Ray>(ray->getIntersectionPoint() + EPSILON * L, L);
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
            
            //const float F0 = pow(obj->getIOR() - 1, 2)/pow(obj->getIOR() + 1, 2);
            //const float F = F0 + (1 - F0) * pow(1 - VdotH, 5);
            const float F = schlicksApproximation(obj->getIOR(), H, V);
            
            const float rs = (D * G * F)/(4 * NdotL * NdotV);
            
            colorSum += lights.at(i)->getColor() * NdotL * ((1 - obj->getMetallic()) * rd
                                                            + obj->getMetallic() * rs);
        }
    }
    
    return ka + colorSum;
}

vec3 Shader::findRefractedColor(const vector<shared_ptr<SceneObject>> &objects,
                                const vector<shared_ptr<LightSource>> &lights,
                                const shared_ptr<Ray> &ray, const int bounces,
                                const string &BRDF, string &trace)
{
    vec3 refractedColor = vec3(0);
    int index = -1;
    shared_ptr<SceneObject> obj = objects.at(ray->getIndexOfIntersectedObject());
    vec3 n;
    if (obj->getObjectType() == "Sphere") {
        n = obj->getNormalAtPoint(ray->getIntersectionPoint());
    }
    else {
        n = obj->getNormal();
    }
    const vec3 d = ray->getDirection();
    
    float d_dot_n = dot(d, n);
    
    float n1_divide_n2;
    // Checking to see if are entering or exiting the shape.
    if (d_dot_n < 0) {
        // If negative, they are in opposite directions, which means entering.
        n1_divide_n2 = 1/obj->getIOR();
    }
    else {
        // If positive, they are in same direction, which means exiting.
        n1_divide_n2 = obj->getIOR();
        // We also need to flip the normal.
        n = -n;
        d_dot_n = dot(d, n);
    }
    
    const float sqroot = sqrt(1 - pow(n1_divide_n2, 2) * (1 - pow(d_dot_n, 2)));
    const vec3 refractedDirection = n1_divide_n2 * (d - d_dot_n * n) - n * sqroot;
    
    shared_ptr<Ray> refractedRay;
    if (obj->getObjectType() == "Sphere") {
        refractedRay = make_shared<Ray>(ray->getIntersectionPoint() + refractedDirection * SPHERE_EPSILON,
                                        refractedDirection);
    }
    else {
        refractedRay = make_shared<Ray>(ray->getIntersectionPoint() + refractedDirection * EPSILON,
                                        refractedDirection);
    }
    index = refractedRay->findClosestObjectIndex(objects);
    
    string indent;
    for (int i = 0; i < bounces; i++) {
        indent += "  ";
    }
    trace += "\n|\n \\";
    trace += "\n" + indent + "o - Iteration type: Refraction";
    trace += "\n" + indent + "|   " + ray->getRayInfo();
    
    // If the refraction hits an object.
    if (index > -1) {
        trace += "\n|   Hit Object ID (" + to_string(index + 1) + " - "
        + objects.at(index)->getObjectType() + ") at T = "
        + ray->getIntersectionTimeString() + ", Intersection = "
        + ray->getIntersectionPointString();
        
        refractedColor = Shader::getShadedColor(objects, lights, refractedRay,
                                                bounces, BRDF, trace);
    }
    else {
        trace += "\n" + indent + "|   No intersection.";
        trace += "\n" + indent + "|----";
    }
    
    return refractedColor;
}

vec3 Shader::findReflectedColor(const vector<shared_ptr<SceneObject>> &objects,
                                const vector<shared_ptr<LightSource>> &lights,
                                const shared_ptr<Ray> &ray, const int bounces,
                                const string &BRDF, string &trace)
{
    vec3 reflectedColor = vec3(0);
    int index = -1;
    shared_ptr<SceneObject> obj = objects.at(ray->getIndexOfIntersectedObject());
    vec3 n;
    if (obj->getObjectType() == "Sphere") {
        n = obj->getNormalAtPoint(ray->getIntersectionPoint());
    }
    else {
        n = obj->getNormal();
    }
    const vec3 d = ray->getDirection();
    const vec3 reflectedDirection = normalize(d - 2 * (dot(d, n)) * n);
    
    shared_ptr<Ray> reflectedRay;
    if (obj->getObjectType() == "Sphere") {
        reflectedRay = make_shared<Ray>(ray->getIntersectionPoint() + reflectedDirection * SPHERE_EPSILON,
                                        reflectedDirection);
    }
    else {
        reflectedRay = make_shared<Ray>(ray->getIntersectionPoint() + reflectedDirection * EPSILON,
                                        reflectedDirection);
    }
    index = reflectedRay->findClosestObjectIndex(objects);
    
    string indent;
    for (int i = 0; i < bounces; i++) {
        indent += "  ";
    }
    trace += "\n|\n \\";
    trace += "\n" + indent + "o - Iteration type: Reflection";
    trace += "\n" + indent + "|   " + ray->getRayInfo();
    
    // If the reflection hits an object.
    if (index > -1) {
        trace += "\n|   Hit Object ID (" + to_string(index + 1) + " - "
        + objects.at(index)->getObjectType() + ") at T = "
        + ray->getIntersectionTimeString() + ", Intersection = "
        + ray->getIntersectionPointString();
        
        reflectedColor = Shader::getShadedColor(objects, lights, reflectedRay,
                                                bounces, BRDF, trace);
    }
    else {
        trace += "\n" + indent + "|   No intersection.";
        trace += "\n" + indent + "|----";
    }
    
    return reflectedColor;
}

float Shader::schlicksApproximation(const float ior, const vec3 &normal, const vec3 &view)
{
    const float F0 = pow(ior - 1, 2)/pow(ior + 1, 2);
    
    return F0 + (1 - F0) * pow(1 - dot(normal, view), 5);
}
