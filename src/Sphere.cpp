//
//  Sphere.cpp
//  ray-tracer
//
//  Created by Cody Rhoads on 4/4/17.
//
//

#include <glm/gtc/matrix_transform.hpp>

#include "Sphere.hpp"
#include "LightSource.hpp"
#include "Ray.hpp"
#include "IntersectionResults.h"

using namespace std;
using namespace glm;

Sphere::Sphere() :
SceneObject(),
center(vec3(0)),
radius(0)
{
    
}

Sphere::Sphere(const int ID, const vec3 &center, const float radius, const vec3 &color,
               const float ambient, const float diffuse, const float specular,
               const float reflection, const float filter, const float roughness,
               const float metallic, const float ior, const mat4 &inverseModelMatrix) :
SceneObject(ID, color, ambient, diffuse, specular, reflection, filter, roughness,
            metallic, ior, inverseModelMatrix),
center(center),
radius(radius)
{
    if (inverseModelMatrix != mat4(1)) {
        vector<vec4> BBcorners = {
            vec4(center.x - radius, center.y - radius, center.z - radius, 1),
            vec4(center.x + radius, center.y - radius, center.z - radius, 1),
            vec4(center.x - radius, center.y + radius, center.z - radius, 1),
            vec4(center.x + radius, center.y + radius, center.z - radius, 1),
            vec4(center.x - radius, center.y - radius, center.z + radius, 1),
            vec4(center.x + radius, center.y - radius, center.z + radius, 1),
            vec4(center.x - radius, center.y + radius, center.z + radius, 1),
            vec4(center.x + radius, center.y + radius, center.z + radius, 1),
        };
        
        const mat4 modelMatrix = inverse(inverseModelMatrix);
        for (unsigned int i = 0; i < BBcorners.size(); i++) {
            BBcorners.at(i) = modelMatrix * BBcorners.at(i);
            if (i == 0) {
                BBmins = vec3(BBcorners.at(i));
                BBmaxes = vec3(BBcorners.at(i));
            }
            else {
                BBmins = vec3(std::min(BBmins.x, BBcorners.at(i).x),
                              std::min(BBmins.y, BBcorners.at(i).y),
                              std::min(BBmins.z, BBcorners.at(i).z));
                BBmaxes = vec3(std::max(BBmaxes.x, BBcorners.at(i).x),
                               std::max(BBmaxes.y, BBcorners.at(i).y),
                               std::max(BBmaxes.z, BBcorners.at(i).z));
            }
        }
    }
    else {
        BBmins = vec3(center.x - radius, center.y - radius, center.z - radius);
        BBmaxes = vec3(center.x + radius, center.y + radius, center.z + radius);
    }
}

IntersectionResults Sphere::findIntersection(const shared_ptr<Ray> &ray)
{
    IntersectionResults ir = IntersectionResults();
    // Transforming the ray from world space to object space
    vec3 rayOrigin = vec3(inverseModelMatrix * vec4(ray->getOrigin(), 1.0));
    vec3 rayDirection = vec3(inverseModelMatrix * vec4(ray->getDirection(), 0.0));
    float t;
    
    const float A = dot(rayDirection, rayDirection);
    const float B = 2 * dot(rayDirection, rayOrigin - center);
    const float C = dot(rayOrigin - center, rayOrigin - center) - pow(radius, 2);
    
    const float discriminant = (B * B) - (4 * A * C);
    
    if (discriminant > 0) {
        t = (-B - sqrt(discriminant))/(2 * A);
        if (t < 0) {
            t = (-B + sqrt(discriminant))/(2 * A);
            if (t > 0) {
                ir.foundIntersection = true;
                ir.t = t;
                ir.intersectedObj = shared_from_this();
            }
        }
        else {
            ir.foundIntersection = true;
            ir.t = t;
            ir.intersectedObj = shared_from_this();
        }
    }
    
    return ir;
}

void Sphere::printObjectInfo() const
{
    printf("- Type: Sphere\n");
    printf("- Center: {%.4g %.4g %.4g}\n", center.x, center.y, center.z);
    printf("- Radius: %.4g\n", radius);
    SceneObject::printObjectInfo();
}

vec3 Sphere::getNormalAtPoint(const vec3 &point) const
{
    vec3 pointObjSpace = vec3(inverseModelMatrix * vec4(point, 1.0));
    mat4 transposedInverseModelMat = transpose(inverseModelMatrix);
    return normalize(vec3(transposedInverseModelMat * vec4(pointObjSpace - center, 0.0)));
}
