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

#include "SceneObject.hpp"

class Plane : public SceneObject
{
public:
    Plane();
    Plane(glm::vec3 normal, int distance, glm::vec3 color, float ambient, float diffuse);
    
    void print();
    
    glm::vec3 getNormal() const {return normal;}
    int getDistance() const {return distance;}
private:
    glm::vec3 normal;
    int distance;
};

#endif /* Plane_hpp */
