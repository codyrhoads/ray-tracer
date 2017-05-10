//
//  Sphere.cpp
//  ray-tracer
//
//  Created by Cody Rhoads on 4/4/17.
//
//

#include "Sphere.hpp"
#include "LightSource.hpp"
#include "Ray.hpp"

using namespace std;
using namespace glm;

Sphere::Sphere() :
SceneObject(),
center(vec3(0, 0, 0)),
radius(0)
{
    
}

Sphere::Sphere(const vec3 &center, const float radius, const vec3 &color,
               const float ambient, const float diffuse, const float specular,
               const float reflection, const float roughness, const float metallic,
               const float ior) :
SceneObject(color, ambient, diffuse, specular, reflection, roughness, metallic, ior),
center(center),
radius(radius)
{
    
}

bool Sphere::testIntersection(const shared_ptr<Ray> &ray, float &t)
{
    const float A = dot(ray->getDirection(), ray->getDirection());
    const float B = 2 * dot(ray->getDirection(), ray->getOrigin() - center);
    const float C = dot(ray->getOrigin() - center, ray->getOrigin() - center) - pow(radius, 2);
    
    const float discriminant = (B * B) - (4 * A * C);
    
    if (discriminant > 0) {
        t = (-B - sqrt(discriminant))/(2 * A);
        if (t < 0) {
            t = (-B + sqrt(discriminant))/(2 * A);
            if (t > 0) {
                return true;
            }
        }
        else {
            return true;
        }
    }
    
    return false;
}

vec3 Sphere::findLocalColorBlinnPhong(const vector<shared_ptr<SceneObject>> &objects,
                                      const vector<shared_ptr<LightSource>> &lights,
                                      const shared_ptr<Ray> &ray)
{
    vec3 colorSum = vec3(0, 0, 0);
    const vec3 ka = color * ambient;
    const vec3 kd = color * diffuse;
    const vec3 ks = color * specular;
    const vec3 N = normalize(ray->getIntersectionPoint() - center);
    const float power = (2.0/pow(roughness, 2.0)) - 2.0;
    
    for (unsigned int i = 0; i < lights.size(); i++) {
        int index = -1;
        const vec3 L = normalize(lights.at(i)->getLocation() - ray->getIntersectionPoint());
        
        shared_ptr<Ray> shadowTestRay = make_shared<Ray>(ray->getIntersectionPoint() + epsilon * L, L);
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

vec3 Sphere::findLocalColorCookTorrance(const vector<shared_ptr<SceneObject>> &objects,
                                        const vector<shared_ptr<LightSource>> &lights,
                                        const shared_ptr<Ray> &ray)
{
    vec3 colorSum = vec3(0, 0, 0);
    const vec3 ka = color * ambient;
    const vec3 kd = color * diffuse;
    const vec3 N = normalize(ray->getIntersectionPoint() - center);
    const float alpha = pow(roughness, 2);
    
    for (unsigned int i = 0; i < lights.size(); i++) {
        int index = -1;
        const vec3 L = normalize(lights.at(i)->getLocation() - ray->getIntersectionPoint());
        
        shared_ptr<Ray> shadowTestRay = make_shared<Ray>(ray->getIntersectionPoint() + epsilon * L, L);
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

vec3 Sphere::findReflectedColor(const vector<shared_ptr<SceneObject>> &objects,
                                const vector<shared_ptr<LightSource>> &lights,
                                const shared_ptr<Ray> &ray, const int bouncesLeft,
                                const string &BRDF)
{
    vec3 reflectedColor = vec3(0, 0, 0);
    int index = -1;
    const vec3 n = normalize(ray->getIntersectionPoint() - center);
    const vec3 d = ray->getDirection();
    const vec3 reflectedDirection = normalize(d - 2 * (dot(d, n)) * n);
    
    shared_ptr<Ray> reflectedRay = make_shared<Ray>(ray->getIntersectionPoint() + reflectedDirection * epsilon,
                                                    reflectedDirection);
    index = reflectedRay->findClosestObjectIndex(objects);
    
    if (index > -1) {
        reflectedColor = objects.at(index)->getShadedColor(objects, lights, reflectedRay,
                                                           bouncesLeft, BRDF);
    }
    
    return reflectedColor;
}

void Sphere::printObjectInfo()
{
    printf("- Type: Sphere\n");
    printf("- Center: {%.4g %.4g %.4g}\n", center.x, center.y, center.z);
    printf("- Radius: %.4g\n", radius);
    SceneObject::printObjectInfo();
    printf("  - Specular: %.4g\n", specular);
    printf("  - Roughness: %.4g\n", roughness);
    printf("  - Metallic: %.4g\n", metallic);
    printf("  - Ior: %.4g\n", ior);
}
