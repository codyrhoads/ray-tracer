//
//  Ray.cpp
//  ray-tracer
//
//  Created by Cody Rhoads on 4/11/17.
//
//

#include <stdio.h>

#include "SceneObject.hpp"
#include "Ray.hpp"

using namespace std;
using namespace glm;

Ray::Ray() :
origin(vec3(0, 0, 0)),
direction(vec3(0, 0, 0)),
t(0)
{
    
}

Ray::Ray(const vec3 &origin, const vec3 &dir) :
origin(origin),
direction(dir),
t(0)
{
    
}

int Ray::getClosestObjectIndex(const vector<shared_ptr<SceneObject>> &objects)
{
    int index = -1;
    
    for (unsigned int i = 0; i < objects.size(); i++) {
        float currT;
        if (objects.at(i)->testIntersection(shared_from_this(), currT)) {
            if (index == -1 || t > currT) {
                t = currT;
                index = i;
            }
        }
    }
    
    return index;
}

void Ray::printRayInfo()
{
    printf("Ray: {%.4g %.4g %.4g} -> {%.4g %.4g %.4g}\n", origin.x, origin.y, origin.z,
           direction.x, direction.y, direction.z);
}
