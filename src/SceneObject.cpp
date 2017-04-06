//
//  SceneObject.cpp
//  ray-tracer
//
//  Created by Cody Rhoads on 4/4/17.
//
//

#include <stdio.h>

#include "SceneObject.hpp"

using namespace std;
using namespace glm;

SceneObject::SceneObject() :
color(vec3(0, 0, 0)),
ambient(0),
diffuse(0)
{
    
}

SceneObject::SceneObject(vec3 color, float ambient, float diffuse) :
color(color),
ambient(ambient),
diffuse(diffuse)
{
    
}

void SceneObject::print()
{
    printf("- Color: {%g %g %g}\n", color.x, color.y, color.z);
    printf("- Material:\n");
    printf("  - Ambient: %g\n", ambient);
    printf("  - Diffuse: %g\n", diffuse);
}
