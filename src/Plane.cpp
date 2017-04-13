//
//  Plane.cpp
//  ray-tracer
//
//  Created by Cody Rhoads on 4/4/17.
//
//

#include <stdio.h>

#include "SceneObject.hpp"
#include "Plane.hpp"
#include "Ray.hpp"

using namespace std;
using namespace glm;

Plane::Plane() :
SceneObject(),
normal(vec3(0, 0, 0)),
distance(0)
{
    
}

Plane::Plane(vec3 normal, float distance, vec3 color, float ambient,
             float diffuse) :
SceneObject(color, ambient, diffuse),
normal(normal),
distance(distance)
{
    
}

bool Plane::testIntersection(shared_ptr<Ray> &ray, float &t)
{
    float denominator = dot(ray->getDirection(), normal);
    
    if (denominator < epsilon && denominator > -epsilon) {
        return false;
    }
    else {
        t = (distance - dot(ray->getOrigin(), normal))/denominator;
        
        if (t < -epsilon) {
            return false;
        }
        else {
            return true;
        }
    }
}

void Plane::printObjectInfo()
{
    printf("- Type: Plane\n");
    printf("- Normal: {%g %g %g}\n", normal.x, normal.y, normal.z);
    printf("- Distance: %g\n", distance);
    SceneObject::printObjectInfo();
}

void Plane::printObjectType()
{
    printf("Object Type: Plane\n");
}
