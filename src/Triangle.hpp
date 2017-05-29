//
//  Triangle.hpp
//  raytrace
//
//  Created by Cody Rhoads on 5/9/17.
//
//

#ifndef Triangle_hpp
#define Triangle_hpp

#include "SceneObject.hpp"

class Triangle : public SceneObject, public std::enable_shared_from_this<Triangle>
{
public:
    Triangle();
    Triangle(const int ID, const glm::vec3 &v0, const glm::vec3 &v1, const glm::vec3 &v2,
             const glm::vec3 &color, const float ambient, const float diffuse,
             const float specular, const float reflection, const float filter,
             const float roughness, const float metallic, const float ior,
             const glm::mat4 &inverseModelMatrix);
    
    IntersectionResults findIntersection(const std::shared_ptr<Ray> &ray);
    
    glm::vec3 getNormalAtPoint(const glm::vec3 &point) const;
    
    void printObjectInfo() const;
    std::string getObjectType() const {return "Triangle";}
private:
    glm::vec3 v0, v1, v2;
};

#endif /* Triangle_hpp */
