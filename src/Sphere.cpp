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
               const float reflection, const float roughness, const float metallic,
               const float ior) :
SceneObject(color, ambient, diffuse, specular, reflection, roughness, metallic, ior),
center(center),
radius(radius)
{
    
}

bool Sphere::testIntersection(const shared_ptr<Ray> &ray, float &t)
{
    const float A = dot(ray->getDirection(), ray->getDirection());
    const float B = 2 * dot(ray->getDirection(), ray->getOrigin() - center);
    const float C = dot(ray->getOrigin() - center, ray->getOrigin() - center) - pow(radius, 2);
    
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

void Sphere::printObjectInfo()
{
    printf("- Type: Sphere\n");
    printf("- Center: {%.4g %.4g %.4g}\n", center.x, center.y, center.z);
    printf("- Radius: %.4g\n", radius);
    SceneObject::printObjectInfo();
    printf("  - Specular: %.4g\n", specular);
    printf("  - Roughness: %.4g\n", roughness);
    printf("  - Metallic: %.4g\n", metallic);
    printf("  - Ior: %.4g\n", ior);
}
