//
//  Ray.cpp
//  ray-tracer
//
//  Created by Cody Rhoads on 4/11/17.
//
//

#include <stdio.h>

#include "Ray.hpp"

using namespace std;
using namespace glm;

Ray::Ray() :
origin(vec3(0, 0, 0)),
direction(vec3(0, 0, 0))
{
    
}

Ray::Ray(const vec3 &origin, const vec3 &dir) :
origin(origin),
direction(dir)
{
    
}

vec3 Ray::getPointAtTime(float t)
{
    return origin + t * direction;
}

void Ray::printRayInfo()
{
    printf("Ray: {%.4g %.4g %.4g} -> {%.4g %.4g %.4g}\n", origin.x, origin.y, origin.z,
           direction.x, direction.y, direction.z);
}
