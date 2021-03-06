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
#include "Math.hpp"

#define MAX_BOUNCES 6
#define AO_SAMPLES 64
#define AO_DISTANCE 0.3f
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
    vec3 reflectedColor = vec3(0), refractedColor = vec3(0);
    shared_ptr<SceneObject> obj = ray->getIntersectedObject();
    IlluminationValues iv;
    bool totalInternalReflection = false;
    
    vec3 N = obj->getNormalAtPoint(ray->getIntersectionPoint());
    // If ray is exiting a shape, then flip the normal.
    if (dot(ray->getDirection(), N) > 0) {
        N = -N;
    }
    
    if (optArgs.BRDF == "Blinn-Phong") {
        iv = findLocalColorBlinnPhong(ray);
    }
    else {
        iv = findLocalColorCookTorrance(ray);
    }
    
    float fresnelReflectance = 0;
    if (bounces < MAX_BOUNCES) {
        if (obj->getFilter() > 0) {
            refractedColor = findRefractedColor(ray, bounces + 1, trace);
            if (refractedColor.x < 0) {
                totalInternalReflection = true;
            }
        }
        if (optArgs.fresnel) {
            fresnelReflectance = Math::schlicksApproximation(obj->getIOR(), N, -ray->getDirection());
        }
        if (obj->getReflection() > 0 || fresnelReflectance > 0 || totalInternalReflection) {
            reflectedColor = obj->getColor() * findReflectedColor(ray, bounces + 1,
                                                                  trace);
        }
    }
    
    // If Monte Carlo global illumination is enabled, calculate the new ambient.
    if (optArgs.globalIllum && bounces < optArgs.giBounces) {
        iv.ambient = findAmbientMonteCarlo(ray, bounces, N, trace);
    }
    
    Contributions contrib = findContributions(obj, totalInternalReflection, fresnelReflectance);
    
    const vec3 finalColor = contrib.localContribution * iv.getColor() +
                            contrib.reflectionContribution * reflectedColor +
                            contrib.refractionContribution * refractedColor;
    
    if (isPrintRays) {
        addToPrintRays(obj, ray, contrib, iv, reflectedColor, refractedColor,
                       bounces, N, finalColor, trace);
    }
    
    return finalColor;
}

IlluminationValues Shader::findLocalColorBlinnPhong(const shared_ptr<Ray> &ray)
{
    IlluminationValues iv;
    vec3 totalDiffuse = vec3(0), totalSpecular = vec3(0);
    shared_ptr<SceneObject> obj = ray->getIntersectedObject();
    const vec3 ka = obj->getColor() * obj->getAmbient();
    const vec3 kd = obj->getColor() * obj->getDiffuse();
    const vec3 ks = obj->getColor() * obj->getSpecular();
    
    vec3 N = obj->getNormalAtPoint(ray->getIntersectionPoint());
    // If ray is exiting a shape, then flip the normal.
    if (dot(ray->getDirection(), N) > 0) {
        N = -N;
    }
    
    const float power = (2.0/pow(obj->getRoughness(), 2.0)) - 2.0;

    for (unsigned int i = 0; i < lights.size(); i++) {
        const vec3 L = normalize(lights.at(i)->getLocation() - ray->getIntersectionPoint());
        const vec3 V = normalize(-ray->getDirection());
        const vec3 H = normalize(V + L);
        const float shadowFactor = findShadowFactor(ray, lights.at(i), N);
        
        const vec3 rd = kd * std::max(0.0f, dot(N, L));
        const vec3 rs = ks * pow(std::max(0.0f, dot(H, N)), power);
        
        totalDiffuse += lights.at(i)->getColor() * rd * shadowFactor;
        totalSpecular += lights.at(i)->getColor() * rs * shadowFactor;
    }

    iv.ambient = ka;
    iv.diffuse = totalDiffuse;
    iv.specular = totalSpecular;
    
    return iv;
}

IlluminationValues Shader::findLocalColorCookTorrance(const shared_ptr<Ray> &ray)
{
    IlluminationValues iv;
    vec3 totalDiffuse = vec3(0), totalSpecular = vec3(0);
    shared_ptr<SceneObject> obj = ray->getIntersectedObject();
    const vec3 ka = obj->getColor() * obj->getAmbient();
    const vec3 kd = obj->getColor() * obj->getDiffuse();
    
    vec3 N = obj->getNormalAtPoint(ray->getIntersectionPoint());
    // If ray is exiting a shape, then flip the normal.
    if (dot(ray->getDirection(), N) > 0) {
        N = -N;
    }
    const float alpha = pow(obj->getRoughness(), 2);
    
    for (unsigned int i = 0; i < lights.size(); i++) {
        const vec3 L = normalize(lights.at(i)->getLocation() - ray->getIntersectionPoint());
        const vec3 V = normalize(-ray->getDirection());
        const vec3 H = normalize(V + L);
        const float NdotH = dot(N, H);
        const float NdotV = dot(N, V);
        const float VdotH = dot(V, H);
        const float NdotL = dot(N, L);
        const vec3 rd = kd;
        const float shadowFactor = findShadowFactor(ray, lights.at(i), N);
        
        const float exponent = (pow(NdotH, 2) - 1) / (pow(alpha, 2) * pow(NdotH, 2));
        const float D = (1/(M_PI * pow(alpha, 2))) * (exp(exponent)/pow(NdotH, 4));
        
        float G = std::min(1.0f, (2 * NdotH * NdotV)/VdotH);
        G = std::min(G, (2 * NdotH * NdotL)/VdotH);
        
        const float F = Math::schlicksApproximation(obj->getIOR(), H, V);
        
        const float rs = (D * G * F)/(4 * NdotL * NdotV);
        
        totalDiffuse += lights.at(i)->getColor() * NdotL * ((1 - obj->getMetallic()) * rd) * shadowFactor;
        totalSpecular += lights.at(i)->getColor() * NdotL * (obj->getMetallic() * rs) * shadowFactor;
    }
    
    iv.ambient = ka;
    iv.diffuse = totalDiffuse;
    iv.specular = totalSpecular;
    
    return iv;
}

vec3 Shader::findRefractedColor(const shared_ptr<Ray> &ray, const int bounces,
                                string &trace)
{
    vec3 refractedColor = vec3(0);
    bool enteringObj = true;
    
    shared_ptr<SceneObject> obj = ray->getIntersectedObject();
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
    
    const float inSqroot = 1 - pow(n1_divide_n2, 2) * (1 - pow(d_dot_n, 2));
    if (inSqroot >= 0) {
        const vec3 refractedDirection = n1_divide_n2 * (d - d_dot_n * n) - n * sqrt(inSqroot);
        
        shared_ptr<Ray> refractedRay = make_shared<Ray>(ray->getIntersectionPoint() + refractedDirection * EPSILON, refractedDirection);
        shared_ptr<SceneObject> hitObjFromRefract = refractedRay->findClosestObject(objects, -1);
        
        // If the refraction hits an object.
        if (hitObjFromRefract != nullptr) {
            refractedColor = getShadedColor(refractedRay, bounces, trace);
            
            if (isPrintRays) {
                string indent;
                indent = to_string(bounces) + "  ";
                trace = "\n" + indent + "  Iteration type: Refraction" + trace;
            }
            
            // Calculate attenuation. We only do this while the ray is going inside the obj
            // (hence only doing it when enteringObj is true).
            if (enteringObj) {
                vec3 attenuation = vec3(1);
                attenuation = Math::getAttenuation(obj->getColor(),
                                                   glm::distance(refractedRay->getIntersectionPoint(),
                                                                 refractedRay->getOrigin()));
                refractedColor *= attenuation;
            }
        }
    }
    else {
        refractedColor = vec3(-1);
    }
    
    return refractedColor;
}

vec3 Shader::findReflectedColor(const shared_ptr<Ray> &ray, const int bounces,
                                string &trace)
{
    vec3 reflectedColor = vec3(0);
    
    shared_ptr<SceneObject> obj = ray->getIntersectedObject();
    vec3 n = obj->getNormalAtPoint(ray->getIntersectionPoint());
    const vec3 d = ray->getDirection();
    const vec3 reflectedDirection = normalize(d - 2 * (dot(d, n)) * n);
    
    shared_ptr<Ray> reflectedRay = make_shared<Ray>(ray->getIntersectionPoint() + reflectedDirection * EPSILON, reflectedDirection);
    shared_ptr<SceneObject> hitObjFromReflect = reflectedRay->findClosestObject(objects, -1);
    
    // If the reflection hits an object.
    if (hitObjFromReflect != nullptr) {
        reflectedColor = getShadedColor(reflectedRay, bounces, trace);
        if (isPrintRays) {
            string indent;
            indent = to_string(bounces) + "  ";
            trace = "\n" + indent + "  Iteration type: Reflection" + trace;
        }
    }
    
    return reflectedColor;
}

vec3 Shader::findAmbientMonteCarlo(const shared_ptr<Ray> &ray, const int bounces,
                                   const vec3 &N, string &trace)
{
    vec3 ambient = vec3(0);
    int numSamples;
    if (bounces == 0) {
        numSamples = optArgs.giSamples;
    }
    else {
        numSamples = optArgs.giSamples/(bounces * optArgs.giRatio);
    }
    
    const int sqrtSamples = sqrt(numSamples);
    for (int i = 0; i < sqrtSamples; i++) {
        for (int j = 0; j < sqrtSamples; j++) {
            const float u = ((float)i / sqrtSamples) + (0.5 + Math::randFloat(-1, 1)/2)/sqrtSamples;
            const float v = ((float)j / sqrtSamples) + (0.5 + Math::randFloat(-1, 1)/2)/sqrtSamples;
            
            vec3 samplePoint = Math::generateCosineWeightedPoint(u, v);
            vec3 alignedPoint = Math::alignSampleVector(samplePoint, vec3(0, 0, 1), N);
            
            // Calculate the ambient color by making a ray from the intersection
            // point to the direction of the sampled point.
            shared_ptr<Ray> monteCarloRay = make_shared<Ray>(ray->getIntersectionPoint() + alignedPoint * EPSILON, alignedPoint);
            shared_ptr<SceneObject> monteObj = monteCarloRay->findClosestObject(objects, ray->getIntersectedObjectID());
            if (monteObj != nullptr) {
                ambient += getShadedColor(monteCarloRay, bounces + 1, trace)/(float)numSamples;
            }
        }
    }
    
    return ambient;
}

float Shader::findShadowFactor(const shared_ptr<Ray> &ray, const shared_ptr<LightSource> &light,
                               const vec3 &N)
{
    float shadowFactor = 0.0f;
    const vec3 L = normalize(light->getLocation() - ray->getIntersectionPoint());
    
    shared_ptr<Ray> shadowTestRay = make_shared<Ray>(ray->getIntersectionPoint() + EPSILON * L, L);
    shared_ptr<SceneObject> blockingObj = shadowTestRay->findClosestObject(objects, ray->getIntersectedObjectID());
    const float lightT = dot(normalize(shadowTestRay->getDirection()),
                             light->getLocation() - shadowTestRay->getOrigin());
    const bool isBlocked = (blockingObj != nullptr && shadowTestRay->getIntersectionTime() < lightT);
    
    if (optArgs.ambientOcc && !isBlocked) {
        const int sqrtSamples = sqrt(AO_SAMPLES);
        for (int i = 0; i < sqrtSamples; i++) {
            for (int j = 0; j < sqrtSamples; j++) {
                const float u = ((float)i / sqrtSamples) + (0.5 + Math::randFloat(-1, 1)/2)/sqrtSamples;
                const float v = ((float)j / sqrtSamples) + (0.5 + Math::randFloat(-1, 1)/2)/sqrtSamples;
                
                vec3 samplePoint = Math::generateCosineWeightedPoint(u, v);
                vec3 alignedPoint = Math::alignSampleVector(samplePoint, vec3(0, 0, 1), N);
                
                // Calculate the ambient color by making a ray from the intersection
                // point to the direction of the sampled point.
                shared_ptr<Ray> aoRay = make_shared<Ray>(ray->getIntersectionPoint() + alignedPoint * EPSILON, alignedPoint);
                shared_ptr<SceneObject> intersectedObj = aoRay->findClosestObject(objects, ray->getIntersectedObjectID());
                if (intersectedObj == nullptr || aoRay->getIntersectionTime() > AO_DISTANCE) {
                    shadowFactor += 1.0f/(AO_SAMPLES);
                }
            }
        }
    }
    else if (!isBlocked) {
        shadowFactor = 1.0f;
    }
    
    return shadowFactor;
}

Contributions Shader::findContributions(const shared_ptr<SceneObject> &obj,
                                        const bool totalInternalReflection,
                                        const float fresnelReflectance)
{
    Contributions contrib;
    contrib.localContribution = (1 - obj->getFilter()) * (1 - obj->getReflection());
    
    if (totalInternalReflection) {
        contrib.reflectionContribution = 1 - contrib.localContribution;
        contrib.refractionContribution = 0;
    }
    else {
        contrib.reflectionContribution = (1 - obj->getFilter()) * obj->getReflection() +
            obj->getFilter() * fresnelReflectance;
        contrib.refractionContribution = obj->getFilter() * (1 - fresnelReflectance);
    }
    
    return contrib;
}

void Shader::addToPrintRays(const shared_ptr<SceneObject> &obj, const shared_ptr<Ray> &ray,
                            const Contributions contrib, const IlluminationValues &iv,
                            const vec3 &reflectedColor, const vec3 &refractedColor,
                            const int bounces, const vec3 &N, const vec3 &finalColor,
                            string &trace)
{
    string indent;
    ostringstream info;
    const vec3 transformedRayOrigin = vec3(obj->getInverseModelMatrix() * vec4(ray->getOrigin(), 1.0));
    const vec3 transformedRayDirection = vec3(obj->getInverseModelMatrix() * vec4(ray->getDirection(), 0.0));
    const vec3 ambient = contrib.localContribution * iv.ambient;
    const vec3 diffuse = contrib.localContribution * iv.diffuse;
    const vec3 specular = contrib.localContribution * iv.specular;
    const vec3 reflect = contrib.reflectionContribution * reflectedColor;
    const vec3 refract = contrib.refractionContribution * refractedColor;
    
    indent = to_string(bounces) + "  ";
    
    info << fixed << setprecision(4) <<
    "\n"+indent+"             Ray: " << ray->getRayInfo() <<
    "\n"+indent+" Transformed Ray: {" << transformedRayOrigin.x << " " << transformedRayOrigin.y << " "
    << transformedRayOrigin.z << "} -> {" << transformedRayDirection.x << " "
    << transformedRayDirection.y << " " << transformedRayDirection.z << "}" <<
    "\n"+indent+"      Hit Object: (ID #" << obj->getID() << " - " << obj->getObjectType() << ")" <<
    "\n"+indent+"    Intersection: " << ray->getIntersectionPointString() << " at T = "
    << ray->getIntersectionTime() <<
    "\n"+indent+"          Normal: {" << N.x << " " << N.y << " " << N.z << "}" <<
    "\n"+indent+"     Final Color: {" << finalColor.x << " " << finalColor.y << " " << finalColor.z << "}" <<
    "\n"+indent+"         Ambient: {" << ambient.x << " " << ambient.y << " " << ambient.z << "}" <<
    "\n"+indent+"         Diffuse: {" << diffuse.x << " " << diffuse.y << " " << diffuse.z << "}" <<
    "\n"+indent+"        Specular: {" << specular.x << " " << specular.y << " " << specular.z << "}" <<
    "\n"+indent+"      Reflection: {" << reflect.x << " " << reflect.y << " " << reflect.z << "}" <<
    "\n"+indent+"      Refraction: {" << refract.x << " " << refract.y << " " << refract.z << "}" <<
    "\n"+indent+"   Contributions: " << contrib.localContribution << " Local, " << contrib.reflectionContribution << " Reflection, " << contrib.refractionContribution << " Transmission" <<
    "\n---------------------------------------------";
    
    trace = info.str() + trace;
}
