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

class Plane : public SceneObject, public std::enable_shared_from_this<Plane>
{
public:
    Plane();
    Plane(const int ID, const glm::vec3 &normal, const float distance, const glm::vec3 &color,
          const float ambient, const float diffuse, const float specular,
          const float reflection, const float filter, const float roughness,
          const float metallic, const float ior, const glm::mat4 &inverseModelMatrix);
    
    IntersectionResults findIntersection(const std::shared_ptr<Ray> &ray);
    
    void printObjectInfo() const;
    std::string getObjectType() const {return "Plane";}
private:
    glm::vec3 getNormalAtPoint(const glm::vec3 &point) const {return normal;}
    
    glm::vec3 normal;
    float distance;
    
    const float epsilon = 0.0001;
};

#endif /* Plane_hpp */
