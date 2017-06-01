//
//  Box.cpp
//  raytrace
//
//  Created by Cody Rhoads on 5/29/17.
//
//

#include <glm/gtc/matrix_transform.hpp>

#include "Box.hpp"
#include "LightSource.hpp"
#include "Ray.hpp"
#include "IntersectionResults.h"

using namespace std;
using namespace glm;

Box::Box() :
SceneObject()
{
    
}

Box::Box(const int ID, const vec3 &mins, const vec3 &maxes, const vec3 &color,
         const float ambient, const float diffuse, const float specular,
         const float reflection, const float filter, const float roughness,
         const float metallic, const float ior, const mat4 &inverseModelMatrix) :
SceneObject(ID, color, ambient, diffuse, specular, reflection, filter, roughness,
            metallic, ior, inverseModelMatrix),
objMins(mins),
objMaxes(maxes)
{
    if (inverseModelMatrix != mat4(1)) {
        vector<vec4> BBcorners = {
            vec4(mins.x, mins.y, mins.z, 1),
            vec4(maxes.x, mins.y, mins.z, 1),
            vec4(mins.x, maxes.y, mins.z, 1),
            vec4(maxes.x, maxes.y, mins.z, 1),
            vec4(mins.x, mins.y, maxes.z, 1),
            vec4(maxes.x, mins.y, maxes.z, 1),
            vec4(mins.x, maxes.y, maxes.z, 1),
            vec4(maxes.x, maxes.y, maxes.z, 1),
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
        BBmins = mins;
        BBmaxes = maxes;
    }
}

IntersectionResults Box::findIntersection(const shared_ptr<Ray> &ray)
{
    IntersectionResults ir = IntersectionResults();
    // Transforming the ray from world space to object space
    vec3 rayOrigin = vec3(inverseModelMatrix * vec4(ray->getOrigin(), 1.0));
    vec3 rayDirection = vec3(inverseModelMatrix * vec4(ray->getDirection(), 0.0));
    float tgmin = -numeric_limits<float>::max();
    float tgmax = numeric_limits<float>::max();
    float t1, t2;
    
    if (rayDirection.x == 0) {
        if (rayOrigin.x >= objMins.x || rayOrigin.x <= objMaxes.x) {
            return ir;
        }
    }
    else {
        // X component
        t1 = (objMins.x - rayOrigin.x) / rayDirection.x;
        t2 = (objMaxes.x - rayOrigin.x) / rayDirection.x;
        
        if (t1 > t2) {
            swap(t1, t2);
        }
        
        if (t1 > tgmin) {
            tgmin = t1;
        }
        if (t2 < tgmax) {
            tgmax = t2;
        }
    }
    
    if (rayDirection.y == 0) {
        if (rayOrigin.y >= objMins.y || rayOrigin.y <= objMaxes.y) {
            return ir;
        }
    }
    else {
        // Y component
        t1 = (objMins.y - rayOrigin.y) / rayDirection.y;
        t2 = (objMaxes.y - rayOrigin.y) / rayDirection.y;
        
        if (t1 > t2) {
            swap(t1, t2);
        }
        
        if (t1 > tgmin) {
            tgmin = t1;
        }
        if (t2 < tgmax) {
            tgmax = t2;
        }
    }
    
    if (rayDirection.z == 0) {
        if (rayOrigin.z >= objMins.z || rayOrigin.z <= objMaxes.z) {
            return ir;
        }
    }
    else {
        // Z component
        t1 = (objMins.z - rayOrigin.z) / rayDirection.z;
        t2 = (objMaxes.z - rayOrigin.z) / rayDirection.z;
        
        if (t1 > t2) {
            swap(t1, t2);
        }
        
        if (t1 > tgmin) {
            tgmin = t1;
        }
        if (t2 < tgmax) {
            tgmax = t2;
        }
    }
    
    if (tgmin > tgmax) {
        return ir;
    }
    if (tgmax < 0) {
        return ir;
    }
    
    ir.foundIntersection = true;
    if (tgmin > 0) {
        ir.t = tgmin;
    }
    else {
        ir.t = tgmax;
    }
    ir.intersectedObj = shared_from_this();
    return ir;
}

void Box::printObjectInfo() const
{
    printf("- Type: Box\n");
    printf("- Mins: {%.4g %.4g %.4g}\n", objMins.x, objMins.y, objMins.z);
    printf("- Maxes: {%.4g %.4g %.4g}\n", objMaxes.x, objMaxes.y, objMaxes.z);
    SceneObject::printObjectInfo();
}

vec3 Box::getNormalAtPoint(const vec3 &point) const
{
    vec3 pointObjSpace = vec3(inverseModelMatrix * vec4(point, 1.0));
    mat4 transposedInverseModelMat = transpose(inverseModelMatrix);
    float minXDiff = abs(pointObjSpace.x - objMins.x);
    float minYDiff = abs(pointObjSpace.y - objMins.y);
    float minZDiff = abs(pointObjSpace.z - objMins.z);
    float maxXDiff = abs(pointObjSpace.x - objMaxes.x);
    float maxYDiff = abs(pointObjSpace.y - objMaxes.y);
    float maxZDiff = abs(pointObjSpace.z - objMaxes.z);
    
    // Any point on the box surface will have one component (xyz) that matches a
    // min/max. Knowing which min/max it matches provides the normal of that
    // point in object space.
    if (minXDiff < epsilon) {
        return normalize(vec3(transposedInverseModelMat * vec4(-1, 0, 0, 0)));
    }
    else if (minYDiff < epsilon) {
        return normalize(vec3(transposedInverseModelMat * vec4(0, -1, 0, 0)));
    }
    else if (minZDiff < epsilon) {
        return normalize(vec3(transposedInverseModelMat * vec4(0, 0, -1, 0)));
    }
    else if (maxXDiff < epsilon) {
        return normalize(vec3(transposedInverseModelMat * vec4(1, 0, 0, 0)));
    }
    else if (maxYDiff < epsilon) {
        return normalize(vec3(transposedInverseModelMat * vec4(0, 1, 0, 0)));
    }
    else if (maxZDiff < epsilon) {
        return normalize(vec3(transposedInverseModelMat * vec4(0, 0, 1, 0)));
    }
    else {
        printf("ERROR: INVALID POINT\n");
        return vec3(-1);
    }
}
