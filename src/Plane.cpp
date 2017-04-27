//
//  Plane.cpp
//  ray-tracer
//
//  Created by Cody Rhoads on 4/4/17.
//
//

#include "Plane.hpp"
#include "Ray.hpp"
#include "LightSource.hpp"

using namespace std;
using namespace glm;

Plane::Plane() :
SceneObject(),
normal(vec3(0, 0, 0)),
distance(0)
{
    
}

Plane::Plane(const vec3 &normal, const float distance, const vec3 &color,
             const float ambient, const float diffuse, const float specular,
             const float roughness, const float metallic, const float ior) :
SceneObject(color, ambient, diffuse, specular, roughness, metallic, ior),
normal(normal),
distance(distance)
{
    
}

bool Plane::testIntersection(const shared_ptr<Ray> &ray, float &t)
{
    float denominator = dot(ray->getDirection(), normal);
    
    if (denominator < epsilon && denominator > -epsilon) {
        return false;
    }
    else {
        t = (distance - dot(ray->getOrigin(), normal))/denominator;
        
        if (t < 0) {
            return false;
        }
        else {
            return true;
        }
    }
}

vec3 Plane::getColorBlinnPhong(const vector<shared_ptr<SceneObject>> &objects,
                               const vector<shared_ptr<LightSource>> &lights,
                               const shared_ptr<Ray> &ray)
{
    vec3 colorSum = vec3(0, 0, 0);
    vec3 ka = color * ambient;
    vec3 kd = color * diffuse;
    vec3 N = normal;
    
    for (unsigned int i = 0; i < lights.size(); i++) {
        int index = -1;
        vec3 L = normalize(lights.at(i)->getLocation() -
                           ray->getIntersectionPoint());
        shared_ptr<Ray> shadowTestRay = make_shared<Ray>(ray->getIntersectionPoint() + epsilon*L,
                                                         L);
        index = shadowTestRay->getClosestObjectIndex(objects);
        
        float lightT = dot(normalize(shadowTestRay->getDirection()),
                           lights.at(i)->getLocation() - shadowTestRay->getOrigin());
        
        if (index == -1 || shadowTestRay->getIntersectionTime() > lightT) {
            vec3 rd = kd * std::max(0.0f, dot(N, L));
            colorSum += lights.at(i)->getColor() * rd;
        }
    }
    
    return ka + colorSum;
}

vec3 Plane::getColorCookTorrance(const vector<shared_ptr<SceneObject>> &objects,
                                 const vector<shared_ptr<LightSource>> &lights,
                                 const shared_ptr<Ray> &ray)
{
    vec3 colorSum = vec3(0, 0, 0);
    vec3 ka = color * ambient;
    vec3 kd = color * diffuse;
    vec3 N = normal;
    float alpha = pow(roughness, 2);
    
    for (unsigned int i = 0; i < lights.size(); i++) {
        int index = -1;
        vec3 L = normalize(lights.at(i)->getLocation() -
                           ray->getIntersectionPoint());
        shared_ptr<Ray> shadowTestRay = make_shared<Ray>(ray->getIntersectionPoint() + epsilon*L,
                                                         L);
        index = shadowTestRay->getClosestObjectIndex(objects);
        
        float lightT = dot(normalize(shadowTestRay->getDirection()),
                           lights.at(i)->getLocation() - shadowTestRay->getOrigin());
        
        if (index == -1 || shadowTestRay->getIntersectionTime() > lightT) {
            vec3 V = normalize(-ray->getDirection());
            vec3 H = normalize(V + L);
            float NdotH = dot(N, H);
            float NdotV = dot(N, V);
            float VdotH = dot(V, H);
            float NdotL = dot(N, L);
            vec3 rd = kd;
            
            float exponent = (pow(NdotH, 2) - 1) / (pow(alpha, 2) * pow(NdotH, 2));
            float D = (1/(M_PI * pow(alpha, 2))) * (exp(exponent)/pow(NdotH, 4));
            
            float G = std::min(1.0f, (2 * NdotH * NdotV)/VdotH);
            G = std::min(G, (2 * NdotH * NdotL)/VdotH);
            
            float F0 = pow(ior-1, 2)/pow(ior+1, 2);
            float F = F0 + (1-F0) * pow(1 - VdotH, 5);
            
            float rs = (D * G * F)/(4 * NdotL * NdotV);
            
            colorSum += lights.at(i)->getColor() * NdotL * ((1-metallic) * rd +
                                                            metallic * rs);
        }
        else {
            colorSum = vec3(0, 0, 0);
        }
    }
    
    return ka + colorSum;
}

void Plane::printObjectInfo()
{
    printf("- Type: Plane\n");
    printf("- Normal: {%.4g %.4g %.4g}\n", normal.x, normal.y, normal.z);
    printf("- Distance: %.4g\n", distance);
    SceneObject::printObjectInfo();
}

string Plane::getObjectType()
{
    return "Plane";
}
