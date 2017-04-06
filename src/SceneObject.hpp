//
//  SceneObject.hpp
//  ray-tracer
//
//  Created by Cody Rhoads on 4/4/17.
//
//

#ifndef SceneObject_hpp
#define SceneObject_hpp

#include <glm/gtc/type_ptr.hpp>

class SceneObject
{
public:
    SceneObject();
    SceneObject(glm::vec3 color, float ambient, float diffuse);
    
    virtual void print();
    
    glm::vec3 getColor() const {return color;}
    float getAmbient() const {return ambient;}
    float getDiffuse() const {return diffuse;}
protected:
    glm::vec3 color;
    float ambient, diffuse;
};

#endif /* SceneObject_hpp */
