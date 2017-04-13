//
//  Ray.hpp
//  ray-tracer
//
//  Created by Cody Rhoads on 4/11/17.
//
//

#ifndef Ray_hpp
#define Ray_hpp

#include <glm/gtc/type_ptr.hpp>

class Ray
{
public:
    Ray();
    Ray(glm::vec3 origin, glm::vec3 dir);
    
    void setIntersectionTime(float newT) {t = newT;}
    
    glm::vec3 getPointAtTime(float t);
    glm::vec3 getOrigin() const {return origin;}
    glm::vec3 getDirection() const {return direction;}
    float getClosestIntersectionTime() const {return t;}
    
    void printRayInfo();
private:
    glm::vec3 origin, direction;
    float t;
};

#endif /* Ray_hpp */
