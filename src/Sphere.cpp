//
//  Sphere.cpp
//  ray-tracer
//
//  Created by Cody Rhoads on 4/4/17.
//
//

#include <stdio.h>
#include <glm/gtc/type_ptr.hpp>

#include "SceneObject.hpp"
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

Sphere::Sphere(vec3 center, float radius, vec3 color, float ambient,
               float diffuse) :
SceneObject(color, ambient, diffuse),
center(center),
radius(radius)
{
    
}

bool Sphere::testIntersection(shared_ptr<Ray> &ray, float &t)
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
    printf("- Center: {%g %g %g}\n", center.x, center.y, center.z);
    printf("- Radius: %g\n", radius);
    SceneObject::printObjectInfo();
}

void Sphere::printObjectType()
{
    printf("Object Type: Sphere\n");
}
