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
           const float roughness, const float metallic, const float ior);
    
    bool testIntersection(const std::shared_ptr<Ray> &ray, float &t);
    glm::vec3 getColorBlinnPhong(const std::vector<std::shared_ptr<SceneObject>> &objects,
                                 const std::vector<std::shared_ptr<LightSource>> &lights,
                                 const std::shared_ptr<Ray> &ray);
    glm::vec3 getColorCookTorrance(const std::vector<std::shared_ptr<SceneObject>> &objects,
                                   const std::vector<std::shared_ptr<LightSource>> &lights,
                                   const std::shared_ptr<Ray> &ray);
    
    glm::vec3 getCenter() const {return center;}
    float getRadius() const {return radius;}
    
    void printObjectInfo();
    std::string getObjectType() {return "Sphere";}
private:
    glm::vec3 center;
    float radius;
    
    const float epsilon = 0.001;
};

#endif /* Sphere_hpp */
