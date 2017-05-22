//
//  SceneObject.cpp
//  ray-tracer
//
//  Created by Cody Rhoads on 4/4/17.
//
//

#include <stdio.h>
#include <sstream>
#include <iomanip>

#include "SceneObject.hpp"
#include "LightSource.hpp"
#include "Ray.hpp"

using namespace std;
using namespace glm;

SceneObject::SceneObject() :
color(vec3(0)),
ambient(0),
diffuse(0),
specular(0.1),
reflection(0),
roughness(0.1),
metallic(0.1),
ior(1.6)
{
    
}

SceneObject::SceneObject(const vec3 &color, const float ambient, const float diffuse,
                         const float specular, const float reflection, const float filter,
                         const float roughness, const float metallic, const float ior,
                         const mat4 &inverseModelMatrix) :
color(color),
ambient(ambient),
diffuse(diffuse),
specular(specular),
reflection(reflection),
filter(filter),
roughness(roughness),
metallic(metallic),
ior(ior),
inverseModelMatrix(inverseModelMatrix)
{
    
}

bool SceneObject::testIntersection(const shared_ptr<Ray> &ray, float &t) const
{
    return false;
}

void SceneObject::printObjectInfo() const
{
    printf("- Color: {%.4g %.4g %.4g}\n", color.x, color.y, color.z);
    printf("- Material:\n");
    printf("  - Ambient: %.4g\n", ambient);
    printf("  - Diffuse: %.4g\n", diffuse);
    printf("  - Specular: %.4g\n", specular);
    printf("  - Reflection: %.4g\n", reflection);
    printf("  - Filter: %.4g\n", filter);
    printf("  - Roughness: %.4g\n", roughness);
    printf("  - Metallic: %.4g\n", metallic);
    printf("  - Ior: %.4g\n", ior);
}

string SceneObject::getNormalAtPointString(const vec3 &point) const
{
    ostringstream info;
    vec3 normal = getNormalAtPoint(point);
    info << fixed << setprecision(4) << "{" << normal.x << " " << normal.y << " " << normal.z << "}";
    return info.str();
}
