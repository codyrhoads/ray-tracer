//
//  Plane.cpp
//  ray-tracer
//
//  Created by Cody Rhoads on 4/4/17.
//
//

#include "Plane.hpp"
#include "Ray.hpp"
#include "LightSource.hpp"

using namespace std;
using namespace glm;

Plane::Plane() :
SceneObject(),
normal(vec3(0)),
distance(0)
{
    
}

Plane::Plane(const vec3 &normal, const float distance, const vec3 &color,
             const float ambient, const float diffuse, const float specular,
             const float reflection, const float roughness, const float metallic,
             const float ior) :
SceneObject(color, ambient, diffuse, specular, reflection, roughness, metallic, ior),
normal(normal),
distance(distance)
{
    
}

bool Plane::testIntersection(const shared_ptr<Ray> &ray, float &t)
{
    const float denominator = dot(ray->getDirection(), normal);
    
    if (denominator < epsilon && denominator > -epsilon) {
        return false;
    }
    else {
        t = (distance - dot(ray->getOrigin(), normal))/denominator;
        
        if (t < 0) {
            return false;
        }
        else {
            return true;
        }
    }
}

vec3 Plane::findReflectedColor(const vector<shared_ptr<SceneObject>> &objects,
                               const vector<shared_ptr<LightSource>> &lights,
                               const shared_ptr<Ray> &ray, const int bouncesLeft,
                               const string &BRDF)
{
    vec3 reflectedColor = vec3(0);
    int index = -1;
    const vec3 n = normal;
    const vec3 d = ray->getDirection();
    const vec3 reflectedDirection = normalize(d - 2 * (dot(d, n)) * n);
    
    shared_ptr<Ray> reflectedRay = make_shared<Ray>(ray->getIntersectionPoint() + reflectedDirection * epsilon,
                                                    reflectedDirection);
    index = reflectedRay->findClosestObjectIndex(objects);
    
    if (index > -1) {
        reflectedColor = objects.at(index)->getShadedColor(objects, lights, reflectedRay,
                                                           bouncesLeft, BRDF);
    }
    
    return reflectedColor;
}

void Plane::printObjectInfo()
{
    printf("- Type: Plane\n");
    printf("- Normal: {%.4g %.4g %.4g}\n", normal.x, normal.y, normal.z);
    printf("- Distance: %.4g\n", distance);
    SceneObject::printObjectInfo();
}
