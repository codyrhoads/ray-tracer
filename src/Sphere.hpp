//
//  Sphere.hpp
//  ray-tracer
//
//  Created by Cody Rhoads on 4/4/17.
//
//

#ifndef Sphere_hpp
#define Sphere_hpp

#include "SceneObject.hpp"

class Sphere : public SceneObject
{
public:
    Sphere();
    Sphere(const glm::vec3 &center, float radius, const glm::vec3 &color,
           float ambient, float diffuse);
    
    bool testIntersection(const std::shared_ptr<Ray> &ray, float &t);
    
    glm::vec3 getCenter() const {return center;}
    float getRadius() const {return radius;}
    
    void printObjectInfo();
    std::string getObjectType();
private:
    glm::vec3 center;
    float radius;
};

#endif /* Sphere_hpp */
