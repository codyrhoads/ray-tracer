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
#include <glm/gtc/type_ptr.hpp>

class Ray;

class SceneObject
{
public:
    SceneObject();
    SceneObject(glm::vec3 color, float ambient, float diffuse);
    
    virtual bool testIntersection(std::shared_ptr<Ray> &ray, float &t);
    
    glm::vec3 getColor() const {return color;}
    float getAmbient() const {return ambient;}
    float getDiffuse() const {return diffuse;}
    
    virtual void printObjectInfo();
    virtual void printObjectType();
    void printObjectColor();
protected:
    glm::vec3 color;
    float ambient, diffuse;
    
    const float epsilon = 0.0001;
};

#endif /* SceneObject_hpp */