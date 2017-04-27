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
               const float roughness, const float metallic, const float ior) :
SceneObject(color, ambient, diffuse, specular, roughness, metallic, ior),
center(center),
radius(radius)
{
    
}

bool Sphere::testIntersection(const shared_ptr<Ray> &ray, float &t)
{
    float A = dot(ray->getDirection(), ray->getDirection());
    float B = 2 * dot(ray->getDirection(), ray->getOrigin() - center);
    float C = dot(ray->getOrigin() - center, ray->getOrigin() - center) - radius*radius;
    
    float discriminant = B*B - 4*A*C;
    
    if (discriminant > 0) {
        t = (-B - sqrt(discriminant))/(2*A);
        if (t < 0) {
            t = (-B + sqrt(discriminant))/(2*A);
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

vec3 Sphere::getColorBlinnPhong(const vector<shared_ptr<SceneObject>> &objects,
                                const vector<shared_ptr<LightSource>> &lights,
                                const shared_ptr<Ray> &ray)
{
    vec3 colorSum = vec3(0, 0, 0);
    vec3 ka = color * ambient;
    vec3 kd = color * diffuse;
    vec3 ks = color * specular;
    vec3 N = normalize(ray->getIntersectionPoint() - center);
    float power = (2.0/pow(roughness, 2.0)) - 2.0;
    
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
            
            vec3 rd = kd * std::max(0.0f, dot(N, L));
            vec3 rs = ks * pow(std::max(0.0f, dot(H, N)), power);
            
            colorSum += lights.at(i)->getColor() * (rd + rs);
        }
    }
    
    return ka + colorSum;
}

vec3 Sphere::getColorCookTorrance(const vector<shared_ptr<SceneObject>> &objects,
                                  const vector<shared_ptr<LightSource>> &lights,
                                  const shared_ptr<Ray> &ray)
{
    vec3 colorSum = vec3(0, 0, 0);
    vec3 ka = color * ambient;
    vec3 kd = color * diffuse;
    vec3 N = normalize(ray->getIntersectionPoint() - center);
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
    }
    
    return ka + colorSum;
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

string Sphere::getObjectType()
{
    return "Sphere";
}
