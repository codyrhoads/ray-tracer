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
             const float specular, const float reflection, const float roughness,
             const float metallic, const float ior);
    
    bool testIntersection(const std::shared_ptr<Ray> &ray, float &t);
    
    void printObjectInfo();
    std::string getObjectType() {return "Triangle";}
private:
    glm::vec3 findLocalColorBlinnPhong(const std::vector<std::shared_ptr<SceneObject>> &objects,
                                       const std::vector<std::shared_ptr<LightSource>> &lights,
                                       const std::shared_ptr<Ray> &ray);
    glm::vec3 findLocalColorCookTorrance(const std::vector<std::shared_ptr<SceneObject>> &objects,
                                         const std::vector<std::shared_ptr<LightSource>> &lights,
                                         const std::shared_ptr<Ray> &ray);
    glm::vec3 findReflectedColor(const std::vector<std::shared_ptr<SceneObject>> &objects,
                                 const std::vector<std::shared_ptr<LightSource>> &lights,
                                 const std::shared_ptr<Ray> &ray, const int bouncesLeft,
                                 const std::string &BRDF);
    
    glm::vec3 v0, v1, v2;
    
    const float epsilon = 0.0001;
};

#endif /* Triangle_hpp */
