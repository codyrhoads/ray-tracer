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
    
    std::shared_ptr<Camera> camera;
    std::vector<std::shared_ptr<LightSource>> lights;
    std::vector<std::shared_ptr<SceneObject>> objects;
};

#endif /* FileParser_hpp */
