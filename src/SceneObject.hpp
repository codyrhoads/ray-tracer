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

struct IntersectionResults;

class SceneObject
{
public:
    SceneObject();
    SceneObject(const int ID, const glm::vec3 &color, const float ambient,
                const float diffuse, const float specular, const float reflection,
                const float filter, const float roughness, const float metallic,
                const float ior, const glm::mat4 &inverseModelMatrix);
    
    virtual IntersectionResults findIntersection(const std::shared_ptr<Ray> &ray);
    
    int getID() const {return ID;}
    glm::vec3 getColor() const {return color;}
    float getAmbient() const {return ambient;}
    float getDiffuse() const {return diffuse;}
    float getSpecular() const {return specular;}
    float getReflection() const {return reflection;}
    float getFilter() const {return filter;}
    float getRoughness() const {return roughness;}
    float getMetallic() const {return metallic;}
    float getIOR() const {return ior;}
    glm::mat4 getInverseModelMatrix() const {return inverseModelMatrix;}
    glm::vec3 getMins() const {return BBmins;}
    glm::vec3 getMaxes() const {return BBmaxes;}
    
    virtual glm::vec3 getNormalAtPoint(const glm::vec3 &point) const {return glm::vec3(0);}
    
    virtual void printObjectInfo() const;
    virtual std::string getNormalAtPointString(const glm::vec3 &point) const;
    virtual std::string getObjectType() const {return "NULL";}
protected:
    int ID;
    glm::vec3 color;
    float ambient, diffuse, specular, reflection, filter, roughness, metallic, ior;
    glm::mat4 inverseModelMatrix;
    
    glm::vec3 BBmins, BBmaxes;
    
    const float sphereEpsilon = 0.001;
    const float epsilon = 0.0001;
};

#endif /* SceneObject_hpp */
