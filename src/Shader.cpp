//
//  Shader.cpp
//  raytrace
//
//  Created by Cody Rhoads on 5/10/17.
//
//

#include <sstream>
#include <iomanip>

#include "Shader.hpp"
#include "SceneObject.hpp"
#include "LightSource.hpp"
#include "Ray.hpp"

#define MAX_BOUNCES 6
#define EPSILON 0.005f

using namespace std;
using namespace glm;

Shader::Shader() :
objects(vector<shared_ptr<SceneObject>>()),
lights(vector<shared_ptr<LightSource>>()),
optArgs(OptionalArgs()),
isPrintRays(false)
{
    
}

Shader::Shader(const vector<shared_ptr<SceneObject>> &objects,
               const vector<shared_ptr<LightSource>> &lights,
               const OptionalArgs &optArgs, const bool isPrintRays) :
objects(objects),
lights(lights),
optArgs(optArgs),
isPrintRays(isPrintRays)
{
    
}

vec3 Shader::getShadedColor(const shared_ptr<Ray> &ray, const int bounces,
                            string &trace)
{
    vec3 localColor = vec3(0), reflectedColor = vec3(0), refractedColor = vec3(0);
    shared_ptr<SceneObject> obj = objects.at(ray->getIndexOfIntersectedObject());
    IlluminationValues iv = IlluminationValues();
    
    vec3 N = obj->getNormalAtPoint(ray->getIntersectionPoint());
    
    // If ray is exiting a shape, then flip the normal.
    if (dot(ray->getDirection(), N) > 0) {
        N = -N;
    }
    
    if (optArgs.BRDF == "Blinn-Phong") {
        localColor = findLocalColorBlinnPhong(ray, iv);
    }
    else {
        localColor = findLocalColorCookTorrance(ray);
    }
    
    float fresnelReflectance = 0;
    if (bounces < MAX_BOUNCES) {
        if (obj->getFilter() > 0) {
            refractedColor = findRefractedColor(ray, bounces + 1, trace);
        }
        if (optArgs.fresnel) {
            fresnelReflectance = schlicksApproximation(obj->getIOR(), N, -ray->getDirection());
        }
        if (obj->getReflection() > 0 || fresnelReflectance > 0) {
            reflectedColor = obj->getColor() * findReflectedColor(ray, bounces + 1,
                                                                  trace);
        }
    }
    
    const float localContribution = (1 - obj->getFilter()) * (1 - obj->getReflection());
    const float reflectionContribution = (1 - obj->getFilter()) * obj->getReflection() +
                                         obj->getFilter() * fresnelReflectance;
    const float refractionContribution = obj->getFilter() * (1 - fresnelReflectance);
    
    const vec3 finalColor = localContribution * localColor +
                            reflectionContribution * reflectedColor +
                            refractionContribution * refractedColor;
    
    if (isPrintRays) {
        ostringstream info;
        vec3 transformedRayOrigin = vec3(obj->getInverseModelMatrix() * vec4(ray->getOrigin(), 1.0));
        vec3 transformedRayDirection = vec3(obj->getInverseModelMatrix() * vec4(ray->getDirection(), 0.0));
        
        info << fixed << setprecision(4) <<
        "\n             Ray: " << ray->getRayInfo() <<
        "\n Transformed Ray: {" << transformedRayOrigin.x << " " << transformedRayOrigin.y << " "
            << transformedRayOrigin.z << "} -> {" << transformedRayDirection.x << " "
            << transformedRayDirection.y << " " << transformedRayDirection.z << "}" <<
        "\n      Hit Object: (ID #" << ray->getIndexOfIntersectedObject() + 1 << " - "
            << obj->getObjectType() << ")" <<
        "\n    Intersection: " << ray->getIntersectionPointString() << " at T = "
            << ray->getIntersectionTime() <<
        "\n          Normal: {" << N.x << " " << N.y << " " << N.z << "}" <<
        "\n     Final Color: {" << finalColor.x << " " << finalColor.y << " " << finalColor.z << "}" <<
        "\n         Ambient: {" << (iv.ambient).x << " " << (iv.ambient).y << " " << (iv.ambient).z << "}" <<
        "\n         Diffuse: {" << (iv.diffuse).x << " " << (iv.diffuse).y << " " << (iv.diffuse).z << "}" <<
        "\n        Specular: {" << (iv.specular).x << " " << (iv.specular).y << " " << (iv.specular).z << "}" <<
        "\n      Reflection: {" << reflectedColor.x << " " << reflectedColor.y << " "
            << reflectedColor.z << "}" <<
        "\n      Refraction: {" << refractedColor.x << " " << refractedColor.y
            << " " << refractedColor.z << "}" <<
        "\n   Contributions: " << localContribution << " Local, " << reflectionContribution << " Reflection, "
            << refractionContribution << " Transmission";
        
        trace = info.str() + trace;
//        
//        if (enteringObj) {
//            trace = "\n      Extra Info: into-object" + trace;
//        }
//        else {
//            trace = "\n      Extra Info: into-air" + trace;
//        }
    }
    
    return finalColor;
}

vec3 Shader::findLocalColorBlinnPhong(const shared_ptr<Ray> &ray,
                                      IlluminationValues &iv)
{
    vec3 colorSum = vec3(0), totalDiffuse = vec3(0), totalSpecular = vec3(0);
    shared_ptr<SceneObject> obj = objects.at(ray->getIndexOfIntersectedObject());
    const vec3 ka = obj->getColor() * obj->getAmbient();
    const vec3 kd = obj->getColor() * obj->getDiffuse();
    const vec3 ks = obj->getColor() * obj->getSpecular();
    const vec3 N = obj->getNormalAtPoint(ray->getIntersectionPoint());
    const float power = (2.0/pow(obj->getRoughness(), 2.0)) - 2.0;
    
    for (unsigned int i = 0; i < lights.size(); i++) {
        int index = -1;
        const vec3 L = normalize(lights.at(i)->getLocation() - ray->getIntersectionPoint());
        
        shared_ptr<Ray> shadowTestRay = make_shared<Ray>(ray->getIntersectionPoint() + EPSILON * L, L);
        index = shadowTestRay->findClosestObjectIndex(objects);
        
        const float lightT = dot(normalize(shadowTestRay->getDirection()),
                                 lights.at(i)->getLocation() - shadowTestRay->getOrigin());
        
        if (index == -1 || shadowTestRay->getIntersectionTime() > lightT) {
            const vec3 V = normalize(-ray->getDirection());
            const vec3 H = normalize(V + L);
            
            const vec3 rd = kd * std::max(0.0f, dot(N, L));
            const vec3 rs = ks * pow(std::max(0.0f, dot(H, N)), power);
            
            colorSum += lights.at(i)->getColor() * (rd + rs);
            totalDiffuse += lights.at(i)->getColor() * rd;
            totalSpecular += lights.at(i)->getColor() * rs;
        }
    }
    
    iv.ambient = ka;
    iv.diffuse = totalDiffuse;
    iv.specular = totalSpecular;
    
    return ka + colorSum;
}

vec3 Shader::findLocalColorCookTorrance(const shared_ptr<Ray> &ray)
{
    vec3 colorSum = vec3(0);
    shared_ptr<SceneObject> obj = objects.at(ray->getIndexOfIntersectedObject());
    const vec3 ka = obj->getColor() * obj->getAmbient();
    const vec3 kd = obj->getColor() * obj->getDiffuse();
    const vec3 N = obj->getNormalAtPoint(ray->getIntersectionPoint());
    const float alpha = pow(obj->getRoughness(), 2);
    
    for (unsigned int i = 0; i < lights.size(); i++) {
        int index = -1;
        const vec3 L = normalize(lights.at(i)->getLocation() - ray->getIntersectionPoint());
        
        shared_ptr<Ray> shadowTestRay = make_shared<Ray>(ray->getIntersectionPoint() + EPSILON * L, L);
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
            
            const float F = schlicksApproximation(obj->getIOR(), H, V);
            
            const float rs = (D * G * F)/(4 * NdotL * NdotV);
            
            colorSum += lights.at(i)->getColor() * NdotL * ((1 - obj->getMetallic()) * rd
                                                            + obj->getMetallic() * rs);
        }
    }
    
    return ka + colorSum;
}

vec3 Shader::findRefractedColor(const shared_ptr<Ray> &ray, const int bounces,
                                string &trace)
{
    vec3 refractedColor = vec3(0);
    int index = -1;
    bool enteringObj = true;
    
    shared_ptr<SceneObject> obj = objects.at(ray->getIndexOfIntersectedObject());
    vec3 n = obj->getNormalAtPoint(ray->getIntersectionPoint());
    const vec3 d = ray->getDirection();
    
    float d_dot_n = dot(d, n);
    
    float n1_divide_n2;
    // Checking to see if are entering or exiting the shape.
    if (d_dot_n < 0) {
        // If negative, they are in opposite directions, which means entering.
        n1_divide_n2 = 1/obj->getIOR();
    }
    else {
        enteringObj = false;
        // If positive, they are in same direction, which means exiting.
        n1_divide_n2 = obj->getIOR();
        // We also need to flip the normal.
        n = -n;
        d_dot_n = dot(d, n);
    }
    
    const float sqroot = sqrt(1 - pow(n1_divide_n2, 2) * (1 - pow(d_dot_n, 2)));
    const vec3 refractedDirection = n1_divide_n2 * (d - d_dot_n * n) - n * sqroot;
    
    shared_ptr<Ray> refractedRay = make_shared<Ray>(ray->getIntersectionPoint() + refractedDirection * EPSILON,
                                                    refractedDirection);
    index = refractedRay->findClosestObjectIndex(objects);
    
    // If the refraction hits an object.
    if (index > -1) {
        refractedColor = getShadedColor(refractedRay, bounces, trace);
    }
    
    // Calculate attenuation. We only do this while the ray is going inside the obj
    // (hence only doing it when enteringObj is true).
    if (enteringObj) {
        vec3 attenuation = vec3(1);
        attenuation = getAttenuation(obj->getColor(), glm::distance(refractedRay->getIntersectionPoint(),
                                                                    refractedRay->getOrigin()));
        refractedColor *= attenuation;
    }
    
    if (isPrintRays) {
        if (enteringObj) {
            trace = "\n      Extra Info: into-object" + trace;
        }
        else {
            trace = "\n      Extra Info: into-air" + trace;
        }
    }
    
    return refractedColor;
}

vec3 Shader::findReflectedColor(const shared_ptr<Ray> &ray, const int bounces,
                                string &trace)
{
    vec3 reflectedColor = vec3(0);
    int index = -1;
    
    shared_ptr<SceneObject> obj = objects.at(ray->getIndexOfIntersectedObject());
    const vec3 n = obj->getNormalAtPoint(ray->getIntersectionPoint());
    const vec3 d = ray->getDirection();
    const vec3 reflectedDirection = normalize(d - 2 * (dot(d, n)) * n);
    
    shared_ptr<Ray> reflectedRay = make_shared<Ray>(ray->getIntersectionPoint() + reflectedDirection * EPSILON,
                                                    reflectedDirection);
    index = reflectedRay->findClosestObjectIndex(objects);
    
    // If the reflection hits an object.
    if (index > -1) {
        reflectedColor = getShadedColor(reflectedRay, bounces, trace);
    }
    
    return reflectedColor;
}

float Shader::schlicksApproximation(const float ior, const vec3 &normal, const vec3 &view)
{
    const float F0 = pow(ior - 1, 2)/pow(ior + 1, 2);
    
    return F0 + (1 - F0) * pow(1 - dot(normal, view), 5);
}

vec3 Shader::getAttenuation(const vec3 &color, const float distance)
{
    return exp((1.0f - color) * 0.15f * -distance);
}
