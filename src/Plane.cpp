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

using namespace std;
using namespace glm;

Plane::Plane() :
SceneObject(),
normal(vec3(0, 0, 0)),
distance(0)
{
    
}

Plane::Plane(vec3 normal, int distance, vec3 color, float ambient, float diffuse) :
SceneObject(color, ambient, diffuse),
normal(normal),
distance(distance)
{
    
}

void Plane::print()
{
    printf("- Type: Plane\n");
    printf("- Normal: {%g %g %g}\n", normal.x, normal.y, normal.z);
    printf("- Distance: %d\n", distance);
    SceneObject::print();
}
