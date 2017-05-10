//
//  Triangle.cpp
//  raytrace
//
//  Created by Cody Rhoads on 5/9/17.
//
//

#include "Triangle.hpp"
#include "LightSource.hpp"
#include "Ray.hpp"

using namespace std;
using namespace glm;

Triangle::Triangle() :
SceneObject(),
v0(vec3(0, 0, 0)),
v1(vec3(0, 0, 0)),
v2(vec3(0, 0, 0))
{
    
}

Triangle::Triangle(const vec3 &v0, const vec3 &v1, const vec3 &v2,
                   const vec3 &color, const float ambient, const float diffuse,
                   const float specular, const float reflection,
                   const float roughness, const float metallic, const float ior) :
SceneObject(color, ambient, diffuse, specular, reflection, roughness, metallic, ior),
v0(v0),
v1(v1),
v2(v2)
{
    
}

bool Triangle::testIntersection(const shared_ptr<Ray> &ray, float &t)
{
    // v0 = a, v1 = b, v2 = c
    const float Xa_minus_Px = v0.x - ray->getOrigin().x;
    const float Ya_minus_Py = v0.y - ray->getOrigin().y;
    const float Za_minus_Pz = v0.z - ray->getOrigin().z;
    const float Xa_minus_Xb = v0.x - v1.x;
    const float Ya_minus_Yb = v0.y - v1.y;
    const float Za_minus_Zb = v0.z - v1.z;
    const float Xa_minus_Xc = v0.x - v2.x;
    const float Ya_minus_Yc = v0.y - v2.y;
    const float Za_minus_Zc = v0.z - v2.z;
    const float Xd = ray->getDirection().x;
    const float Yd = ray->getDirection().y;
    const float Zd = ray->getDirection().z;
    
    float topDeterminant;
    const float bottomDeterminant = (Xa_minus_Xb * Ya_minus_Yc * Zd) +
                                    (Xa_minus_Xc * Yd * Za_minus_Zb) +
                                    (Xd * Ya_minus_Yb * Za_minus_Zc) -
                                    (Xa_minus_Xb * Yd * Za_minus_Zc) -
                                    (Xa_minus_Xc * Ya_minus_Yb * Zd) -
                                    (Xd * Ya_minus_Yc * Za_minus_Zb);
    
    // Calculate t
    topDeterminant = (Xa_minus_Xb * Ya_minus_Yc * Za_minus_Pz) +
                     (Xa_minus_Xc * Ya_minus_Py * Za_minus_Zb) +
                     (Xa_minus_Px * Ya_minus_Yb * Za_minus_Zc) -
                     (Xa_minus_Xb * Ya_minus_Py * Za_minus_Zc) -
                     (Xa_minus_Xc * Ya_minus_Yb * Za_minus_Pz) -
                     (Xa_minus_Px * Ya_minus_Yc * Za_minus_Zb);
    const float calcT = topDeterminant/bottomDeterminant;
    
    if (calcT < 0) {
        return false;
    }
    
    // Calculate gamma
    topDeterminant = (Xa_minus_Xb * Ya_minus_Py * Zd) +
                     (Xa_minus_Px * Yd * Za_minus_Zb) +
                     (Xd * Ya_minus_Yb * Za_minus_Pz) -
                     (Xa_minus_Xb * Yd * Za_minus_Pz) -
                     (Xa_minus_Px * Ya_minus_Yb * Zd) -
                     (Xd * Ya_minus_Py * Za_minus_Zb);
    const float gamma = topDeterminant/bottomDeterminant;
    
    if (gamma < 0 || gamma > 1) {
        return false;
    }
    
    // Calculate beta
    topDeterminant = (Xa_minus_Px * Ya_minus_Yc * Zd) +
                     (Xa_minus_Xc * Yd * Za_minus_Pz) +
                     (Xd * Ya_minus_Py * Za_minus_Zc) -
                     (Xa_minus_Px * Yd * Za_minus_Zc) -
                     (Xa_minus_Xc * Ya_minus_Py * Zd) -
                     (Xd * Ya_minus_Yc * Za_minus_Pz);
    const float beta = topDeterminant/bottomDeterminant;
    
    if (beta < 0 || (beta > 1 - gamma)) {
        return false;
    }
    
    t = calcT;
    return true;
}

vec3 Triangle::findLocalColorBlinnPhong(const vector<shared_ptr<SceneObject>> &objects,
                                        const vector<shared_ptr<LightSource>> &lights,
                                        const shared_ptr<Ray> &ray)
{
    vec3 colorSum = vec3(0, 0, 0);
    const vec3 ka = color * ambient;
    const vec3 kd = color * diffuse;
    const vec3 N = normalize(cross(v1 - v0, v2 - v0));
    
    for (unsigned int i = 0; i < lights.size(); i++) {
        int index = -1;
        const vec3 L = normalize(lights.at(i)->getLocation() - ray->getIntersectionPoint());
        
        shared_ptr<Ray> shadowTestRay = make_shared<Ray>(ray->getIntersectionPoint() + epsilon * L, L);
        index = shadowTestRay->findClosestObjectIndex(objects);
        
        const float lightT = dot(normalize(shadowTestRay->getDirection()),
                                 lights.at(i)->getLocation() - shadowTestRay->getOrigin());
        
        if (index == -1 || shadowTestRay->getIntersectionTime() > lightT) {
            const vec3 rd = kd * std::max(0.0f, dot(N, L));
            colorSum += lights.at(i)->getColor() * rd;
        }
    }
    
    return ka + colorSum;
}

vec3 Triangle::findLocalColorCookTorrance(const vector<shared_ptr<SceneObject>> &objects,
                                          const vector<shared_ptr<LightSource>> &lights,
                                          const shared_ptr<Ray> &ray)
{
    vec3 colorSum = vec3(0, 0, 0);
    const vec3 ka = color * ambient;
    const vec3 kd = color * diffuse;
    const vec3 N = normalize(cross(v1 - v0, v2 - v0));
    const float alpha = pow(roughness, 2);
    
    for (unsigned int i = 0; i < lights.size(); i++) {
        int index = -1;
        const vec3 L = normalize(lights.at(i)->getLocation() -
                                 ray->getIntersectionPoint());
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

vec3 Triangle::findReflectedColor(const vector<shared_ptr<SceneObject>> &objects,
                                  const vector<shared_ptr<LightSource>> &lights,
                                  const shared_ptr<Ray> &ray, const int bouncesLeft,
                                  const string &BRDF)
{
    vec3 reflectedColor = vec3(0, 0, 0);
    int index = -1;
    const vec3 n = normalize(cross(v1 - v0, v2 - v0));
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

void Triangle::printObjectInfo()
{
    printf("- Type: Triangle\n");
    printf("- Vertex 0: {%.4g %.4g %.4g}\n", v0.x, v0.y, v0.z);
    printf("- Vertex 1: {%.4g %.4g %.4g}\n", v1.x, v1.y, v1.z);
    printf("- Vertex 2: {%.4g %.4g %.4g}\n", v2.x, v2.y, v2.z);
    SceneObject::printObjectInfo();
}
