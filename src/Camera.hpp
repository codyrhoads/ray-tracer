//
//  Camera.hpp
//  ray-tracer
//
//  Created by Cody Rhoads on 4/4/17.
//
//

#ifndef Camera_hpp
#define Camera_hpp

#include <fstream>
#include <glm/gtc/type_ptr.hpp>

class Camera
{
public:
    Camera();
    Camera(glm::vec3 location, glm::vec3 up, glm::vec3 right, glm::vec3 lookAt);
    
    void print();
    
    glm::vec3 getLocation() const {return location;}
    glm::vec3 getUp() const {return up;}
    glm::vec3 getRight() const {return right;}
    glm::vec3 getLookAt() const {return lookAt;}
private:
    glm::vec3 location;
    glm::vec3 up;
    glm::vec3 right;
    glm::vec3 lookAt;
};

#endif /* Camera_hpp */
