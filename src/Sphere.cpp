//
//  Sphere.cpp
//  ray-tracer
//
//  Created by Cody Rhoads on 4/4/17.
//
//

#include "Sphere.hpp"
#include "Ray.hpp"

using namespace std;
using namespace glm;

Sphere::Sphere() :
SceneObject(),
center(vec3(0, 0, 0)),
radius(0)
{
    
}

Sphere::Sphere(const vec3 &center, float radius, const vec3 &color,
               float ambient, float diffuse) :
SceneObject(color, ambient, diffuse),
center(center),
radius(radius)
{
    
}

bool Sphere::testIntersection(const shared_ptr<Ray> &ray, float &t)
{
    float A = dot(ray->getDirection(), ray->getDirection());
    float B = 2 * dot(ray->getDirection(), ray->getOrigin() - center);
    float C = dot(ray->getOrigin() - center,
                  ray->getOrigin() - center) - radius*radius;
    
    float discriminant = B*B - 4*A*C;
    
    if (discriminant > epsilon) {
        t = (-B - sqrt(discriminant))/(2*A);
        if (t < -epsilon) {
            t = (-B + sqrt(discriminant))/(2*A);
            if (t > epsilon) {
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
}

string Sphere::getObjectType()
{
    return "Sphere";
}
