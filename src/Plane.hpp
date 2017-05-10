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
          const float reflection, const float roughness, const float metallic,
          const float ior);
    
    bool testIntersection(const std::shared_ptr<Ray> &ray, float &t);
    
    glm::vec3 getNormal() const {return normal;}
    float getDistance() const {return distance;}
    
    void printObjectInfo();
    std::string getObjectType() {return "Plane";}
private:
    glm::vec3 findReflectedColor(const std::vector<std::shared_ptr<SceneObject>> &objects,
                                 const std::vector<std::shared_ptr<LightSource>> &lights,
                                 const std::shared_ptr<Ray> &ray, const int bouncesLeft,
                                 const std::string &BRDF);
    
    glm::vec3 normal;
    float distance;
    
    const float epsilon = 0.0001;
};

#endif /* Plane_hpp */
