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

class SceneObject;
class LightSource;
class Ray;

class Shader
{
public:
    static glm::vec3 getShadedColor(const std::vector<std::shared_ptr<SceneObject>> &objects,
                                    const std::vector<std::shared_ptr<LightSource>> &lights,
                                    const std::shared_ptr<Ray> &ray, const int bounces,
                                    const std::string &BRDF, std::string &trace);
private:
    static glm::vec3 findLocalColorBlinnPhong(const std::vector<std::shared_ptr<SceneObject>> &objects,
                                              const std::vector<std::shared_ptr<LightSource>> &lights,
                                              const std::shared_ptr<Ray> &ray);
    static glm::vec3 findLocalColorCookTorrance(const std::vector<std::shared_ptr<SceneObject>> &objects,
                                                const std::vector<std::shared_ptr<LightSource>> &lights,
                                                const std::shared_ptr<Ray> &ray);
    static glm::vec3 findReflectedColor(const std::vector<std::shared_ptr<SceneObject>> &objects,
                                        const std::vector<std::shared_ptr<LightSource>> &lights,
                                        const std::shared_ptr<Ray> &ray, const int bounces,
                                        const std::string &BRDF, std::string &trace);
    static glm::vec3 findRefractedColor(const std::vector<std::shared_ptr<SceneObject>> &objects,
                                        const std::vector<std::shared_ptr<LightSource>> &lights,
                                        const std::shared_ptr<Ray> &ray, const int bounces,
                                        const std::string &BRDF, std::string &trace);
    static float schlicksApproximation(const float ior, const glm::vec3 &normal,
                                       const glm::vec3 &view);
    static glm::vec3 getAttenuation(const glm::vec3 color, const float distance);
};

#endif /* Shader_hpp */
