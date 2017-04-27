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
normal(vec3(0, 0, 0)),
distance(0)
{
    
}

Plane::Plane(const vec3 &normal, const float distance, const vec3 &color,
             const float ambient, const float diffuse) :
SceneObject(color, ambient, diffuse),
normal(normal),
distance(distance)
{
    
}

bool Plane::testIntersection(const shared_ptr<Ray> &ray, float &t)
{
    vec3 offsetOrigin = ray->getOrigin() + epsilon*ray->getDirection();
    float denominator = dot(ray->getDirection(), normal);
    
    if (denominator < epsilon && denominator > -epsilon) {
        return false;
    }
    else {
        t = (distance - dot(offsetOrigin, normal))/denominator;
        
        if (t < -epsilon) {
            return false;
        }
        else {
            return true;
        }
    }
}

vec3 Plane::getColorBlinnPhong(const vector<shared_ptr<SceneObject>> &objects,
                               const vector<shared_ptr<LightSource>> &lights,
                               const shared_ptr<Ray> &ray)
{
    vec3 colorSum = vec3(0, 0, 0);
    vec3 ka = color * ambient;
    vec3 kd = color * diffuse;
    vec3 N = normalize(normal);
    
    for (unsigned int i = 0; i < lights.size(); i++) {
        int index = -1;
        vec3 L = normalize(lights.at(i)->getLocation() -
                           ray->getIntersectionPoint());
        shared_ptr<Ray> shadowTestRay = make_shared<Ray>(ray->getIntersectionPoint(),
                                                         L);
        index = shadowTestRay->getClosestObjectIndex(objects);
        
        if (index == -1) {
            vec3 rd = kd * std::max(0.0f, dot(N, L));
            colorSum += lights.at(i)->getColor() * rd;
        }
    }
    
    return ka + colorSum;
}

vec3 Plane::getColorCookTorrance(const vector<shared_ptr<SceneObject>> &objects,
                                 const vector<shared_ptr<LightSource>> &lights,
                                 const shared_ptr<Ray> &ray)
{
    return vec3(0, 0, 0);
}

void Plane::printObjectInfo()
{
    printf("- Type: Plane\n");
    printf("- Normal: {%.4g %.4g %.4g}\n", normal.x, normal.y, normal.z);
    printf("- Distance: %.4g\n", distance);
    SceneObject::printObjectInfo();
}

string Plane::getObjectType()
{
    return "Plane";
}
