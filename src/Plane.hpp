//
//  Plane.hpp
//  ray-tracer
//
//  Created by Cody Rhoads on 4/4/17.
//
//

#ifndef Plane_hpp
#define Plane_hpp

#include "SceneObject.hpp"

class Plane : public SceneObject
{
public:
    Plane();
    Plane(const glm::vec3 &normal, const float distance, const glm::vec3 &color,
          const float ambient, const float diffuse, const float specular,
          const float roughness);
    
    bool testIntersection(const std::shared_ptr<Ray> &ray, float &t);
    glm::vec3 getColorBlinnPhong(const std::vector<std::shared_ptr<SceneObject>> &objects,
                                 const std::vector<std::shared_ptr<LightSource>> &lights,
                                 const std::shared_ptr<Ray> &ray);
    glm::vec3 getColorCookTorrance(const std::vector<std::shared_ptr<SceneObject>> &objects,
                                   const std::vector<std::shared_ptr<LightSource>> &lights,
                                   const std::shared_ptr<Ray> &ray);
    
    glm::vec3 getNormal() const {return normal;}
    float getDistance() const {return distance;}
    
    void printObjectInfo();
    std::string getObjectType();
private:
    glm::vec3 normal;
    float distance;
};

#endif /* Plane_hpp */
