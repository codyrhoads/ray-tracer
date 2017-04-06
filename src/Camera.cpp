//
//  Camera.cpp
//  ray-tracer
//
//  Created by Cody Rhoads on 4/4/17.
//
//

#include <stdio.h>

#include "Camera.hpp"

using namespace std;
using namespace glm;

Camera::Camera() :
location(vec3(0, 0, 0)),
up(vec3(0, 0, 0)),
right(vec3(0, 0, 0)),
lookAt(vec3(0, 0, 0))
{
    
}

Camera::Camera(vec3 location, vec3 up, vec3 right, vec3 lookAt) :
location(location),
up(up),
right(right),
lookAt(lookAt)
{
    
}

void Camera::print()
{
    printf("- Location: {%g %g %g}\n", location.x, location.y, location.z);
    printf("- Up: {%g %g %g}\n", up.x, up.y, up.z);
    printf("- Right: {%g %g %g}\n", right.x, right.y, right.z);
    printf("- Look at: {%g %g %g}\n", lookAt.x, lookAt.y, lookAt.z);
}
