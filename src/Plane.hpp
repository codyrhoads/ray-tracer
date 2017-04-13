//
//  Plane.hpp
//  ray-tracer
//
//  Created by Cody Rhoads on 4/4/17.
//
//

#ifndef Plane_hpp
#define Plane_hpp

#include <fstream>
#include <glm/gtc/type_ptr.hpp>

class Plane : public SceneObject
{
public:
    Plane();
    Plane(glm::vec3 normal, float distance, glm::vec3 color, float ambient,
          float diffuse);
    
    bool testIntersection(std::shared_ptr<Ray> &ray, float &t);
    
    glm::vec3 getNormal() const {return normal;}
    float getDistance() const {return distance;}
    
    void printObjectInfo();
    void printObjectType();
private:
    glm::vec3 normal;
    float distance;
};

#endif /* Plane_hpp */
