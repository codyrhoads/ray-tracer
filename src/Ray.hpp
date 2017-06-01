//
//  Ray.hpp
//  ray-tracer
//
//  Created by Cody Rhoads on 4/11/17.
//
//

#ifndef Ray_hpp
#define Ray_hpp

#include <memory>
#include <vector>
#include <glm/gtc/type_ptr.hpp>

class SceneObject;

class Ray : public std::enable_shared_from_this<Ray>
{
public:
    Ray();
    Ray(const glm::vec3 &origin, const glm::vec3 &dir);
    
    std::shared_ptr<SceneObject> findClosestObject(const std::vector<std::shared_ptr<SceneObject>> &objects, const int curObjID);
    
    float getIntersectionTime() const {return t;}
    std::shared_ptr<SceneObject> getIntersectedObject() const {return intersectedObj;}
    glm::vec3 getIntersectionPoint() const {return origin + t * direction;}
    glm::vec3 getOrigin() const {return origin;}
    glm::vec3 getDirection() const {return direction;}
    
    std::string getRayInfo() const;
    std::string getIntersectionPointString() const;
private:
    glm::vec3 origin, direction;
    float t;
    std::shared_ptr<SceneObject> intersectedObj;
};

#endif /* Ray_hpp */
