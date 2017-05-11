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
    
    glm::vec3 getColor() const {return color;}
    float getAmbient() const {return ambient;}
    float getDiffuse() const {return diffuse;}
    float getSpecular() const {return specular;}
    float getReflection() const {return reflection;}
    float getRoughness() const {return roughness;}
    float getMetallic() const {return metallic;}
    float getIOR() const {return ior;}
    
    virtual glm::vec3 getNormal() const {return glm::vec3(0);}
    virtual glm::vec3 getNormalAtPoint(const glm::vec3 &point) const {return glm::vec3(0);}
    
    virtual void printObjectInfo();
    virtual std::string getNormalAtPointString(const glm::vec3 &point);
    std::string getAmbientString() const;
    std::string getDiffuseString() const;
    std::string getSpecularString() const;
    virtual std::string getObjectType() {return "NULL";}
protected:
    glm::vec3 color;
    float ambient, diffuse, specular, reflection, roughness, metallic, ior;
    
    const float sphereEpsilon = 0.001;
    const float epsilon = 0.0001;
};

#endif /* SceneObject_hpp */
