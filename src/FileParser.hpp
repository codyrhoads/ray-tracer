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
    
    void parse(const std::string &filename, const bool useBVH);
    
    std::shared_ptr<Camera> getCamera() const {return camera;}
    std::vector<std::shared_ptr<LightSource>> getLights() const {return lights;}
    std::vector<std::shared_ptr<SceneObject>> getObjects() const {return objects;}
private:
    void parseCamera(std::ifstream &file);
    void parseLight(std::ifstream &file);
    void parsePlane(std::ifstream &file, const int ID);
    void parseSphere(std::ifstream &file, const int ID, const bool useBVH);
    void parseBox(std::ifstream &file, const int ID, const bool useBVH);
    void parseTriangle(std::ifstream &file, const int ID, const bool useBVH);
    
    void findAndSetSingleValueParameter(const std::string &segment, float &parameter,
                                        const std::string &indicator, const size_t start);
    size_t findAndSetVec3Parameter(const std::string &segment, glm::vec3 &parameter,
                                   const std::string &indicator, const size_t start);
    bool findAndSetVec4Parameter(const std::string &segment, glm::vec4 &parameter,
                                 const std::string &indicator, const size_t start);
    glm::mat4 calculateInverseModelMatrix(const std::string &segment);
    size_t findIndexOfFirstTransform(const std::string &segment);
    
    void removeNewlinesAndWhitespace(std::string &segment);
    void removeComments(std::string &segment);
    std::string getNextSegment(std::ifstream &file);
    
    std::shared_ptr<Camera> camera;
    std::vector<std::shared_ptr<LightSource>> lights;
    std::vector<std::shared_ptr<SceneObject>> objects, nonPlanes;
};

#endif /* FileParser_hpp */
