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
center(vec3(0, 0, 0)),
radius(0)
{
    
}

Sphere::Sphere(const vec3 &center, const float radius, const vec3 &color,
               float ambient, float diffuse, float specular, float roughness) :
SceneObject(color, ambient, diffuse),
center(center),
radius(radius),
specular(specular),
roughness(roughness)
{
    
}

bool Sphere::testIntersection(const shared_ptr<Ray> &ray, float &t)
{
    vec3 offsetOrigin = ray->getOrigin() + epsilon*ray->getDirection();
    float A = dot(ray->getDirection(), ray->getDirection());
    float B = 2 * dot(ray->getDirection(), offsetOrigin - center);
    float C = dot(offsetOrigin - center, offsetOrigin - center) - radius*radius;
    
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

vec3 Sphere::getColorBlinnPhong(const vector<shared_ptr<SceneObject>> &objects,
                                const vector<shared_ptr<LightSource>> &lights,
                                const shared_ptr<Ray> &ray)
{
    vec3 colorSum = vec3(0, 0, 0);
    vec3 ka = color * ambient;
    vec3 kd = color * diffuse;
    vec3 ks = color * specular;
    vec3 N = normalize(ray->getIntersectionPoint() - center);
    float power = (2.0/pow(roughness, 2.0)) - 2.0;
    
    for (unsigned int i = 0; i < lights.size(); i++) {
        int index = -1;
        vec3 L = normalize(lights.at(i)->getLocation() -
                           ray->getIntersectionPoint());
        shared_ptr<Ray> shadowTestRay = make_shared<Ray>(ray->getIntersectionPoint(),
                                                         L);
        index = shadowTestRay->getClosestObjectIndex(objects);
        
        if (index == -1) {
            vec3 V = normalize(-ray->getDirection());
            vec3 H = normalize(V + L);
            
            vec3 rd = kd * std::max(0.0f, dot(N, L));
            vec3 rs = ks * pow(std::max(0.0f, dot(H, N)), power);
            
            colorSum += lights.at(i)->getColor() * (rd + rs);
        }
        else {
            colorSum = vec3(0, 0, 0);
        }
    }
    
    return ka + colorSum;
}

vec3 Sphere::getColorCookTorrance(const vector<shared_ptr<SceneObject>> &objects,
                                  const vector<shared_ptr<LightSource>> &lights,
                                  const shared_ptr<Ray> &ray)
{
    return vec3(0, 0, 0);
}

void Sphere::printObjectInfo()
{
    printf("- Type: Sphere\n");
    printf("- Center: {%.4g %.4g %.4g}\n", center.x, center.y, center.z);
    printf("- Radius: %.4g\n", radius);
    SceneObject::printObjectInfo();
    printf("  - Specular: %.4g\n", specular);
    printf("  - Roughness: %.4g\n", roughness);
}

string Sphere::getObjectType()
{
    return "Sphere";
}
