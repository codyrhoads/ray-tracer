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
#include "IntersectionResults.h"

using namespace std;
using namespace glm;

Plane::Plane() :
SceneObject(),
normal(vec3(0)),
distance(0)
{
    
}

Plane::Plane(const int ID, const vec3 &normal, const float distance, const vec3 &color,
             const float ambient, const float diffuse, const float specular,
             const float reflection, const float filter, const float roughness,
             const float metallic, const float ior, const glm::mat4 &inverseModelMatrix) :
SceneObject(ID, color, ambient, diffuse, specular, reflection, filter, roughness,
            metallic, ior, inverseModelMatrix),
normal(normal),
distance(distance)
{
    
}

IntersectionResults Plane::findIntersection(const shared_ptr<Ray> &ray)
{
    IntersectionResults ir = IntersectionResults();
    float t;
    const float denominator = dot(ray->getDirection(), normal);
    
    if (denominator < epsilon && denominator > -epsilon) {
        return ir;
    }
    else {
        t = (distance - dot(ray->getOrigin(), normal))/denominator;
        
        if (t < 0) {
            return ir;
        }
        else {
            ir.foundIntersection = true;
            ir.t = t;
            ir.intersectedObj = shared_from_this();
            return ir;
        }
    }
}

void Plane::printObjectInfo() const
{
    printf("- Type: Plane\n");
    printf("- Normal: {%.4g %.4g %.4g}\n", normal.x, normal.y, normal.z);
    printf("- Distance: %.4g\n", distance);
    SceneObject::printObjectInfo();
}
