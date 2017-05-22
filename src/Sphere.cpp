//
//  Sphere.cpp
//  ray-tracer
//
//  Created by Cody Rhoads on 4/4/17.
//
//

#include "Sphere.hpp"
#include "LightSource.hpp"
#include "Ray.hpp"

using namespace std;
using namespace glm;

Sphere::Sphere() :
SceneObject(),
center(vec3(0)),
radius(0)
{
    
}

Sphere::Sphere(const vec3 &center, const float radius, const vec3 &color,
               const float ambient, const float diffuse, const float specular,
               const float reflection, const float filter, const float roughness,
               const float metallic, const float ior, const mat4 &inverseModelMatrix) :
SceneObject(color, ambient, diffuse, specular, reflection, filter, roughness, metallic,
            ior, inverseModelMatrix),
center(center),
radius(radius)
{
    
}

bool Sphere::testIntersection(const shared_ptr<Ray> &ray, float &t) const
{
    // Transforming the ray from world space to object space
    vec3 rayOrigin = vec3(inverseModelMatrix * vec4(ray->getOrigin(), 1.0));
    vec3 rayDirection = vec3(inverseModelMatrix * vec4(ray->getDirection(), 0.0));
    
    const float A = dot(rayDirection, rayDirection);
    const float B = 2 * dot(rayDirection, rayOrigin - center);
    const float C = dot(rayOrigin - center, rayOrigin - center) - pow(radius, 2);
    
    const float discriminant = (B * B) - (4 * A * C);
    
    if (discriminant > 0) {
        t = (-B - sqrt(discriminant))/(2 * A);
        if (t < 0) {
            t = (-B + sqrt(discriminant))/(2 * A);
            if (t > 0) {
                return true;
            }
        }
        else {
            return true;
        }
    }
    
    return false;
}

void Sphere::printObjectInfo() const
{
    printf("- Type: Sphere\n");
    printf("- Center: {%.4g %.4g %.4g}\n", center.x, center.y, center.z);
    printf("- Radius: %.4g\n", radius);
    SceneObject::printObjectInfo();
}

vec3 Sphere::getNormalAtPoint(const vec3 &point) const
{
    vec3 pointObjSpace = vec3(inverseModelMatrix * vec4(point, 1.0));
    mat4 transposedInverseModelMat = glm::transpose(inverseModelMatrix);
    return normalize(vec3(transposedInverseModelMat * vec4(pointObjSpace - center, 0.0)));
}
