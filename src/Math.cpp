//
//  Math.cpp
//  raytrace
//
//  Created by Cody Rhoads on 6/6/17.
//
//

#include <glm/gtc/matrix_transform.hpp>
#include <stdlib.h>

#include "Math.hpp"

using namespace std;
using namespace glm;

float Math::schlicksApproximation(const float ior, const vec3 &normal, const vec3 &view)
{
    const float F0 = pow(ior - 1, 2)/pow(ior + 1, 2);
    return F0 + (1 - F0) * pow(1 - dot(normal, view), 5);
}

vec3 Math::getAttenuation(const vec3 &color, const float distance)
{
    return exp((1.0f - color) * 0.15f * -distance);
}

vec3 Math::generateCosineWeightedPoint(const float u, const float v)
{
    const float radial = sqrt(u);
    const float theta = 2.0f * M_PI * v;
    
    const float x = radial * cos(theta);
    const float y = radial * sin(theta);
    
    return vec3(x, y, sqrt(1 - u));
}

vec3 Math::alignSampleVector(const vec3 &sample, const vec3 &up, const vec3 &normal)
{
    const float angle = acos(dot(up, normal));
    const vec3 axis = cross(up, normal);
    
    mat4 matrix = mat4(1);
    matrix = rotate(matrix, angle, axis);
    
    return vec3(matrix * vec4(sample, 1.0));
}

float Math::randFloat(const float low, const float high) {
    float r = rand()/(float)RAND_MAX;
    return (1.0f - r) * low + r * high;
}

