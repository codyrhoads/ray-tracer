//
//  LightSource.cpp
//  ray-tracer
//
//  Created by Cody Rhoads on 4/4/17.
//
//

#include <stdio.h>

#include "LightSource.hpp"

using namespace std;
using namespace glm;

LightSource::LightSource() :
location(vec3(0, 0, 0)),
color(vec3(0, 0, 0))
{
    
}

LightSource::LightSource(vec3 location, vec3 color) :
location(location),
color(color)
{
    
}

void LightSource::printLightInfo()
{
    printf("- Location: {%g %g %g}\n", location.x, location.y, location.z);
    printf("- Color: {%g %g %g}\n", color.x, color.y, color.z);
}
