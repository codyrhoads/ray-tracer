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
    Sphere(const glm::vec3 &center, const float radius, const glm::vec3 &color,
           const float ambient, const float diffuse, const float specular,
           const float reflection, const float filter, const float roughness,
           const float metallic, const float ior, const glm::mat4 &inverseModelMatrix);
    
    bool testIntersection(const std::shared_ptr<Ray> &ray, float &t) const;
    
    glm::vec3 getNormalAtPoint(const glm::vec3 &point) const;
    
    void printObjectInfo() const;
    std::string getObjectType() const {return "Sphere";}
private:
    glm::vec3 center;
    float radius;
};

#endif /* Sphere_hpp */
