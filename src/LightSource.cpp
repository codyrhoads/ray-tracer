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
location(vec3(0)),
color(vec3(0))
{
    
}

LightSource::LightSource(const vec3 &location, const vec3 &color) :
location(location),
color(color)
{
    
}

void LightSource::printLightInfo()
{
    printf("- Location: {%.4g %.4g %.4g}\n", location.x, location.y, location.z);
    printf("- Color: {%.4g %.4g %.4g}\n", color.x, color.y, color.z);
}
