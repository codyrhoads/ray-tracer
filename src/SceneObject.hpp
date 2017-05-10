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
    SceneObject(const glm::vec3 &color, const float ambient, const float diffuse,
                const float specular, const float reflection, const float roughness,
                const float metallic, const float ior);
    
    virtual bool testIntersection(const std::shared_ptr<Ray> &ray, float &t);
    glm::vec3 getShadedColor(const std::vector<std::shared_ptr<SceneObject>> &objects,
                             const std::vector<std::shared_ptr<LightSource>> &lights,
                             const std::shared_ptr<Ray> &ray, const int bouncesLeft,
                             const std::string &BRDF);
    
    glm::vec3 getColor() const {return color;}
    float getAmbient() const {return ambient;}
    float getDiffuse() const {return diffuse;}
    float getSpecular() const {return specular;}
    float getRoughness() const {return roughness;}
    
    virtual void printObjectInfo();
    virtual std::string getObjectType() {return "NULL";}
protected:
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
    
    virtual glm::vec3 getNormal() const {return glm::vec3(0);}
    virtual glm::vec3 getNormalAtPoint(const glm::vec3 &point) const {return glm::vec3(0);}
    
    glm::vec3 color;
    float ambient, diffuse, specular, reflection, roughness, metallic, ior;
    
    const float sphereEpsilon = 0.001;
    const float epsilon = 0.0001;
};

#endif /* SceneObject_hpp */
