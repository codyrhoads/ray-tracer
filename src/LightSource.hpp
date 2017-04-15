//
//  LightSource.hpp
//  ray-tracer
//
//  Created by Cody Rhoads on 4/4/17.
//
//

#ifndef LightSource_hpp
#define LightSource_hpp

#include <glm/gtc/type_ptr.hpp>

class LightSource
{
public:
    LightSource();
    LightSource(const glm::vec3 &location, const glm::vec3 &color);
    
    glm::vec3 getLocation() const {return location;}
    glm::vec3 getColor() const {return color;}
    
    void printLightInfo();
private:
    glm::vec3 location;
    glm::vec3 color;
};

#endif /* LightSource_hpp */
