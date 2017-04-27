//
//  SceneObject.hpp
//  ray-tracer
//
//  Created by Cody Rhoads on 4/4/17.
//
//

#ifndef SceneObject_hpp
#define SceneObject_hpp

#include <memory>
#include <vector>
#include <string>
#include <glm/gtc/type_ptr.hpp>

class Ray;
class LightSource;

class SceneObject
{
public:
    SceneObject();
    SceneObject(const glm::vec3 &color, const float ambient,
                const float diffuse, const float specular,
                const float roughness);
    
    virtual bool testIntersection(const std::shared_ptr<Ray> &ray, float &t);
    virtual glm::vec3 getColorBlinnPhong(const std::vector<std::shared_ptr<SceneObject>> &objects,
                                         const std::vector<std::shared_ptr<LightSource>> &lights,
                                         const std::shared_ptr<Ray> &ray);
    virtual glm::vec3 getColorCookTorrance(const std::vector<std::shared_ptr<SceneObject>> &objects,
                                           const std::vector<std::shared_ptr<LightSource>> &lights,
                                           const std::shared_ptr<Ray> &ray);
    
    glm::vec3 getColor() const {return color;}
    float getAmbient() const {return ambient;}
    float getDiffuse() const {return diffuse;}
    float getSpecular() const {return specular;}
    float getRoughness() const {return roughness;}
    
    virtual void printObjectInfo();
    virtual std::string getObjectType();
protected:
    glm::vec3 color;
    float ambient, diffuse, specular, roughness;
};

#endif /* SceneObject_hpp */
