//
//  Camera.hpp
//  ray-tracer
//
//  Created by Cody Rhoads on 4/4/17.
//
//

#ifndef Camera_hpp
#define Camera_hpp

#include <memory>
#include <vector>
#include <glm/gtc/type_ptr.hpp>

class SceneObject;
class Ray;

class Camera
{
public:
    Camera();
    Camera(const glm::vec3 &location, const glm::vec3 &up,
           const glm::vec3 &right, const glm::vec3 &lookAt);
    
    void setImageSize(int width, int height);
    
    void render(const std::vector<std::shared_ptr<SceneObject>> objects);
    void pixelRay(float pixelX, float pixelY);
    void firstHit(const std::vector<std::shared_ptr<SceneObject>> objects,
                  float pixelX, float pixelY);
    
    glm::vec3 getLocation() const {return location;}
    glm::vec3 getUp() const {return up;}
    glm::vec3 getRight() const {return right;}
    glm::vec3 getLookAt() const {return lookAt;}
    
    void printCameraInfo();
private:
    int getClosestObjectIndex(const std::vector<std::shared_ptr<SceneObject>> objects,
                              float &t);
    void setCurrRay(float pixelX, float pixelY);
    
    std::shared_ptr<Ray> currRay;
    glm::vec3 location, up, right, lookAt;
    int imageWidth, imageHeight;
};

#endif /* Camera_hpp */
