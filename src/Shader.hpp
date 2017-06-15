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

#include "OptionalArgs.hpp"

class SceneObject;
class LightSource;
class Ray;

struct IlluminationValues
{
    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;
    
    glm::vec3 getColor()
    {
        return ambient + diffuse + specular;
    }
};

struct Contributions
{
    float localContribution;
    float reflectionContribution;
    float refractionContribution;
};

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
    IlluminationValues findLocalColorBlinnPhong(const std::shared_ptr<Ray> &ray);
    IlluminationValues findLocalColorCookTorrance(const std::shared_ptr<Ray> &ray);
    glm::vec3 findRefractedColor(const std::shared_ptr<Ray> &ray, const int bounces,
                                 std::string &trace);
    glm::vec3 findReflectedColor(const std::shared_ptr<Ray> &ray, const int bounces,
                                 std::string &trace);
    glm::vec3 findAmbientMonteCarlo(const std::shared_ptr<Ray> &ray, const int bounces,
                                    const glm::vec3 &N, std::string &trace);
    float findShadowFactor(const std::shared_ptr<Ray> &ray, const std::shared_ptr<LightSource> &light,
                           const glm::vec3 &N);
    
    Contributions findContributions(const std::shared_ptr<SceneObject> &obj,
                                    const bool totalInternalReflection,
                                    const float fresnelReflectance);
    
    void addToPrintRays(const std::shared_ptr<SceneObject> &obj, const std::shared_ptr<Ray> &ray,
                        const Contributions contrib, const IlluminationValues &iv,
                        const glm::vec3 &reflectedColor, const glm::vec3 &refractedColor,
                        const int bounces, const glm::vec3 &N, const glm::vec3 &finalColor,
                        std::string &trace);
    
    const std::vector<std::shared_ptr<SceneObject>> objects;
    const std::vector<std::shared_ptr<LightSource>> lights;
    const OptionalArgs optArgs;
    const bool isPrintRays;
};

#endif /* Shader_hpp */
