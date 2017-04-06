//
//  Sphere.cpp
//  ray-tracer
//
//  Created by Cody Rhoads on 4/4/17.
//
//

#include <stdio.h>

#include "SceneObject.hpp"
#include "Sphere.hpp"

using namespace std;
using namespace glm;

Sphere::Sphere() :
SceneObject(),
center(vec3(0, 0, 0)),
radius(0)
{
    
}

Sphere::Sphere(vec3 center, float radius, vec3 color, float ambient, float diffuse) :
SceneObject(color, ambient, diffuse),
center(center),
radius(radius)
{
    
}

void Sphere::print()
{
    printf("- Type: Sphere\n");
    printf("- Center: {%g %g %g}\n", center.x, center.y, center.z);
    printf("- Radius: %g\n", radius);
    SceneObject::print();
}
