//
//  Sphere.hpp
//  ray-tracer
//
//  Created by Cody Rhoads on 4/4/17.
//
//

#ifndef Sphere_hpp
#define Sphere_hpp

#include <fstream>
#include <glm/gtc/type_ptr.hpp>

#include "SceneObject.hpp"

class Sphere : public SceneObject
{
public:
    Sphere();
    Sphere(glm::vec3 center, float radius, glm::vec3 color, float ambient, float diffuse);
    
    void print();
    
    glm::vec3 getCenter() const {return center;}
    float getRadius() const {return radius;}
private:
    glm::vec3 center;
    float radius;
};

#endif /* Sphere_hpp */
