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

class Triangle : public SceneObject
{
public:
    Triangle();
    Triangle(const glm::vec3 &v0, const glm::vec3 &v1, const glm::vec3 &v2,
             const glm::vec3 &color, const float ambient, const float diffuse,
             const float specular, const float reflection, const float filter,
             const float roughness, const float metallic, const float ior);
    
    bool testIntersection(const std::shared_ptr<Ray> &ray, float &t);
    
    void printObjectInfo();
    std::string getObjectType() {return "Triangle";}
private:
    glm::vec3 getNormal() const {return normalize(cross(v1 - v0, v2 - v0));}
    
    glm::vec3 v0, v1, v2;
};

#endif /* Triangle_hpp */
