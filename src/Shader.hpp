//
//  Shader.hpp
//  raytrace
//
//  Created by Cody Rhoads on 5/10/17.
//
//

#ifndef Shader_hpp
#define Shader_hpp

#include <memory>
#include <vector>
#include <string>
#include <glm/gtc/type_ptr.hpp>

#include "OptionalArgs.h"

class SceneObject;
class LightSource;
class Ray;

class Shader
{
public:
    Shader();
    Shader(const std::vector<std::shared_ptr<SceneObject>> &objects,
           const std::vector<std::shared_ptr<LightSource>> &lights,
           const OptionalArgs &optArgs, const bool isPrintRays);
    
    glm::vec3 getShadedColor(const std::shared_ptr<Ray> &ray, const int bounces,
                             std::string &trace);
private:
    glm::vec3 findLocalColorBlinnPhong(const std::shared_ptr<Ray> &ray);
    glm::vec3 findLocalColorCookTorrance(const std::shared_ptr<Ray> &ray);
    glm::vec3 findRefractedColor(const std::shared_ptr<Ray> &ray, const int bounces,
                                 std::string &trace);
    glm::vec3 findReflectedColor(const std::shared_ptr<Ray> &ray, const int bounces,
                                 std::string &trace);
    static float schlicksApproximation(const float ior, const glm::vec3 &normal,
                                       const glm::vec3 &view);
    static glm::vec3 getAttenuation(const glm::vec3 &color, const float distance);
    
    const std::vector<std::shared_ptr<SceneObject>> objects;
    const std::vector<std::shared_ptr<LightSource>> lights;
    const OptionalArgs optArgs;
    const bool isPrintRays;
};

#endif /* Shader_hpp */
