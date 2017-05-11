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
                                    const std::shared_ptr<Ray> &ray, const int bouncesLeft,
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
                                        const std::shared_ptr<Ray> &ray, const int bouncesLeft,
                                        const std::string &BRDF, std::string &trace);
};

#endif /* Shader_hpp */
