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
#include <string>
#include <glm/gtc/type_ptr.hpp>

class SceneObject;
class Ray;
class LightSource;

struct OptionalArgs;

class Camera
{
public:
    Camera();
    Camera(const glm::vec3 &location, const glm::vec3 &up,
           const glm::vec3 &right, const glm::vec3 &lookAt);
    
    void setImageSize(const int width, const int height);
    
    void render(const std::vector<std::shared_ptr<SceneObject>> &objects,
                const std::vector<std::shared_ptr<LightSource>> &lights,
                const OptionalArgs &optArgs);
    void pixelRay(const float pixelX, const float pixelY);
    void firstHit(const std::vector<std::shared_ptr<SceneObject>> &objects,
                  const float pixelX, const float pixelY);
    void pixelColor(const std::vector<std::shared_ptr<SceneObject>> &objects,
                    const std::vector<std::shared_ptr<LightSource>> &lights,
                    const float pixelX, const float pixelY,
                    const OptionalArgs &optArgs);
    void printRays(const std::vector<std::shared_ptr<SceneObject>> &objects,
                   const std::vector<std::shared_ptr<LightSource>> &lights,
                   const float pixelX, const float pixelY,
                   const OptionalArgs &optArgs);
    
    glm::vec3 getLocation() const {return location;}
    glm::vec3 getUp() const {return up;}
    glm::vec3 getRight() const {return right;}
    glm::vec3 getLookAt() const {return lookAt;}
    
    void printCameraInfo() const;
private:
    void setCurrRay(const int pixelX, const int pixelY);
    void setCurrRaySuperSampling(const int pixelX, const int pixelY,
                                 const int subPixelX, const int subPixelY,
                                 const int numSubPixels);
    
    std::shared_ptr<Ray> currRay;
    glm::vec3 location, up, right, lookAt;
    int imageWidth, imageHeight;
};

#endif /* Camera_hpp */
