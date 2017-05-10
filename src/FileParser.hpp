//
//  FileParser.hpp
//  ray-tracer
//
//  Created by Cody Rhoads on 4/4/17.
//
//

#ifndef FileParser_hpp
#define FileParser_hpp

#include <memory>
#include <string>
#include <vector>
#include <fstream>
#include <glm/gtc/type_ptr.hpp>

class SceneObject;
class Camera;
class LightSource;

class FileParser
{
public:
    FileParser();
    
    void parse(const std::string &filename);
    
    std::shared_ptr<Camera> getCamera() const {return camera;}
    std::vector<std::shared_ptr<LightSource>> getLights() const {return lights;}
    std::vector<std::shared_ptr<SceneObject>> getObjects() const {return objects;}
private:
    void parseCamera(std::ifstream &file);
    void parseLight(std::ifstream &file);
    void parseSphere(std::ifstream &file);
    void parsePlane(std::ifstream &file);
    void parseTriangle(std::ifstream &file);
    
    void findAndSetSingleValueParameter(const std::string segment, float &parameter,
                                        const std::string indicator, const size_t start);
    size_t findAndSetVectorParameter(const std::string segment, glm::vec3 &parameter,
                                     const std::string indicator, const size_t start);
    
    std::shared_ptr<Camera> camera;
    std::vector<std::shared_ptr<LightSource>> lights;
    std::vector<std::shared_ptr<SceneObject>> objects;
};

#endif /* FileParser_hpp */
