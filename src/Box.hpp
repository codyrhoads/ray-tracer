//
//  Box.hpp
//  raytrace
//
//  Created by Cody Rhoads on 5/29/17.
//
//

#ifndef Box_hpp
#define Box_hpp

#include "SceneObject.hpp"

class Box : public SceneObject, public std::enable_shared_from_this<Box>
{
public:
    Box();
    Box(const int ID, const glm::vec3 &mins, const glm::vec3 &maxes, const glm::vec3 &color,
        const float ambient, const float diffuse, const float specular,
        const float reflection, const float filter, const float roughness,
        const float metallic, const float ior, const glm::mat4 &inverseModelMatrix);
    
    IntersectionResults findIntersection(const std::shared_ptr<Ray> &ray);
    
    glm::vec3 getNormalAtPoint(const glm::vec3 &point) const;
    
    void printObjectInfo() const;
    std::string getObjectType() const {return "Box";}
private:
    glm::vec3 objMins, objMaxes; // mins and maxes in object space
};

#endif /* Box_hpp */
