//
//  FileParser.cpp
//  ray-tracer
//
//  Created by Cody Rhoads on 4/4/17.
//
//

#include <string>
#include <algorithm>

#include "FileParser.hpp"
#include "Camera.hpp"
#include "LightSource.hpp"
#include "Sphere.hpp"
#include "Plane.hpp"

using namespace std;
using namespace glm;

FileParser::FileParser() :
camera(make_shared<Camera>()),
lights(vector<shared_ptr<LightSource>>()),
objects(vector<shared_ptr<SceneObject>>())
{
    
}

void FileParser::parse(const string &filename)
{
    string segment;
    ifstream file;
    
    file.open(filename);
    
    // The '{' character marks the beginning of a new section, whether it be
    // major (camera, plane, sphere, etc.) or minor (pigment, finish).
    while (getline(file, segment, '{')) {
        // Remove all comments from the segment
        size_t found = segment.find("//");
        while (found != string::npos) {
            segment.erase(segment.begin()+found,
                          segment.begin()+segment.find('\n', found+1));
            found = segment.find("//");
        }
        
        /* PARSING CAMERA */
        if (segment.find("camera") != string::npos) {
            parseCamera(file);
        }
        
        /* PARSING LIGHT SOURCE */
        if (segment.find("light_source") != string::npos) {
            parseLight(file);
        }
        
        /* PARSING SPHERE OBJECT */
        if (segment.find("sphere") != string::npos) {
            parseSphere(file);
        }
        
        /* PARSING PLANE OBJECT */
        if (segment.find("plane") != string::npos) {
            parsePlane(file);
        }
    }
    
    file.close();
}

void FileParser::parseCamera(ifstream &file)
{
    string segment;
    vec3 location = vec3(0, 0, 0);
    vec3 up = vec3(0, 0, 0);
    vec3 right = vec3(0, 0, 0);
    vec3 lookAt = vec3(0, 0, 0);
    size_t found;
    
    // A '}' marks the end of a section. The camera doesn't have any subsections
    // that use curly brackets, so we can assume that the '}' is the end of the
    // camera object. Segment now contains all of the information for the camera.
    getline(file, segment, '}');
    
    // Remove all whitespace from the string.
    segment.erase(remove(segment.begin(), segment.end(), ' '), segment.end());
    // Remove all newlines from the string.
    segment.erase(remove(segment.begin(), segment.end(), '\n'), segment.end());
    
    found = segment.find("location");
    if (found != string::npos) {
        string temp;
        int end;
        
        // Go to beginning of x value for location.
        found += sizeof("location");
        end = (int)segment.find(",", found);
        temp = segment.substr(found, end-found);
        location.x = atof(temp.c_str());
        
        // Go to beginning of y value for location.
        found = end + 1;
        end = (int)segment.find(",", found);
        temp = segment.substr(found, end-found);
        location.y = atof(temp.c_str());
        
        // Go to beginning of y value for location.
        found = end + 1;
        end = (int)segment.find(">", found);
        temp = segment.substr(found, end-found);
        location.z = atof(temp.c_str());
    }
    
    found = segment.find("up");
    if (found != string::npos) {
        string temp;
        int end;
        
        // Go to beginning of x value for up.
        found += sizeof("up");
        end = (int)segment.find(",", found);
        temp = segment.substr(found, end-found);
        up.x = atof(temp.c_str());
        
        // Go to beginning of y value for up.
        found = end + 1;
        end = (int)segment.find(",", found);
        temp = segment.substr(found, end-found);
        up.y = atof(temp.c_str());
        
        // Go to beginning of y value for up.
        found = end + 1;
        end = (int)segment.find(">", found);
        temp = segment.substr(found, end-found);
        up.z = atof(temp.c_str());
    }
    
    found = segment.find("right");
    if (found != string::npos) {
        string temp;
        int end;
        
        // Go to beginning of x value for right.
        found += sizeof("right");
        end = (int)segment.find(",", found);
        temp = segment.substr(found, end-found);
        right.x = atof(temp.c_str());
        
        // Go to beginning of y value for right.
        found = end + 1;
        end = (int)segment.find(",", found);
        temp = segment.substr(found, end-found);
        right.y = atof(temp.c_str());
        
        // Go to beginning of y value for right.
        found = end + 1;
        end = (int)segment.find(">", found);
        temp = segment.substr(found, end-found);
        right.z = atof(temp.c_str());
    }
    
    found = segment.find("look_at");
    if (found != string::npos) {
        string temp;
        int end;
        
        // Go to beginning of x value for lookAt.
        found += sizeof("look_at");
        end = (int)segment.find(",", found);
        temp = segment.substr(found, end-found);
        lookAt.x = atof(temp.c_str());
        
        // Go to beginning of y value for lookAt.
        found = end + 1;
        end = (int)segment.find(",", found);
        temp = segment.substr(found, end-found);
        lookAt.y = atof(temp.c_str());
        
        // Go to beginning of y value for lookAt.
        found = end + 1;
        end = (int)segment.find(">", found);
        temp = segment.substr(found, end-found);
        lookAt.z = atof(temp.c_str());
    }
    
    camera = make_shared<Camera>(location, up, right, lookAt);
}

void FileParser::parseLight(ifstream &file)
{
    vec3 location = vec3(0, 0, 0), color = vec3(0, 0, 0);
    size_t found, end;
    string segment, temp;
    
    // A '}' marks the end of a section. The light source doesn't have any
    // subsections that use curly brackets, so we can assume that the '}' is the
    // end of the light source object. Segment now contains all of the
    // information for the current light source.
    getline(file, segment, '}');
    
    // Remove all whitespace from the string.
    segment.erase(remove(segment.begin(), segment.end(), ' '), segment.end());
    // Remove all newlines from the string.
    segment.erase(remove(segment.begin(), segment.end(), '\n'), segment.end());
    
    found = segment.find("<");
    if (found != string::npos) {
        // Go to beginning of x value for location.
        found += sizeof("<")-1;
        end = segment.find(",", found);
        temp = segment.substr(found, end-found);
        location.x = atof(temp.c_str());
        
        // Go to beginning of y value for location.
        found = end + 1;
        end = segment.find(",", found);
        temp = segment.substr(found, end-found);
        location.y = atof(temp.c_str());
        
        // Go to beginning of y value for location.
        found = end + 1;
        end = segment.find(">", found);
        temp = segment.substr(found, end-found);
        location.z = atof(temp.c_str());
    }
    
    found = segment.find("colorrgb");
    if (found != string::npos) {
        // Go to beginning of r value for color.
        found += sizeof("colorrgb");
        end = segment.find(",", found);
        temp = segment.substr(found, end-found);
        color.x = atof(temp.c_str());
        
        // Go to beginning of g value for color.
        found = end + 1;
        end = segment.find(",", found);
        temp = segment.substr(found, end-found);
        color.y = atof(temp.c_str());
        
        // Go to beginning of b value for color.
        found = end + 1;
        end = segment.find(">", found);
        temp = segment.substr(found, end-found);
        color.z = atof(temp.c_str());
    }
    
    shared_ptr<LightSource> ls = make_shared<LightSource>(location, color);
    lights.push_back(ls);
}

void FileParser::parseSphere(ifstream &file)
{
    vec3 center = vec3(0, 0, 0);
    vec3 color = vec3(0, 0, 0);
    float radius = 0, ambient = 0, diffuse = 0, specular = 0.1, roughness = 0.1;
    float metallic = 0.1, ior = 1.6;
    size_t found = 0, end = 0;
    string segment = "", temp;
    
    // Since there are subsections denoted with curly braces, we have to search
    // for the '}' without a corresponding '{'. This means that it is the end
    // of the sphere information.
    getline(file, temp, '}');
    while (count(temp.begin(), temp.end(), '{') > 0) {
        temp.append("}");
        segment.append(temp);
        getline(file, temp, '}');
    }
    segment.append(temp);
    
    // Remove all whitespace from the string.
    segment.erase(remove(segment.begin(), segment.end(), ' '), segment.end());
    // Remove all newlines from the string.
    segment.erase(remove(segment.begin(), segment.end(), '\n'), segment.end());
    
    found = segment.find("<");
    if (found != string::npos) {
        // Go to beginning of x value for center.
        found += sizeof("<")-1;
        end = segment.find(",", found);
        temp = segment.substr(found, end-found);
        center.x = atof(temp.c_str());
        
        // Go to beginning of y value for center.
        found = end + 1;
        end = segment.find(",", found);
        temp = segment.substr(found, end-found);
        center.y = atof(temp.c_str());
        
        // Go to beginning of y value for center.
        found = end + 1;
        end = segment.find(">", found);
        temp = segment.substr(found, end-found);
        center.z = atof(temp.c_str());
    }
    
    found = segment.find(",", end);
    if (found != string::npos) {
        // Go to beginning of radius.
        found += sizeof(",")-1;
        end = segment.find_first_not_of("0123456789.", found);
        temp = segment.substr(found, end-found);
        radius = atof(temp.c_str());
    }
    
    found = segment.find("colorrgb");
    if (found != string::npos) {
        // Go to beginning of r value for color.
        found += sizeof("colorrgb");
        end = segment.find(",", found);
        temp = segment.substr(found, end-found);
        color.x = atof(temp.c_str());
        
        // Go to beginning of g value for color.
        found = end + 1;
        end = segment.find(",", found);
        temp = segment.substr(found, end-found);
        color.y = atof(temp.c_str());
        
        // Go to beginning of b value for color.
        found = end + 1;
        end = segment.find(">", found);
        temp = segment.substr(found, end-found);
        color.z = atof(temp.c_str());
    }
    
    found = segment.find("ambient");
    if (found != string::npos) {
        // Go to beginning of ambient factor.
        found += sizeof("ambient")-1;
        end = segment.find_first_not_of("0123456789.", found);
        temp = segment.substr(found, end-found);
        ambient = atof(temp.c_str());
    }
    
    found = segment.find("diffuse");
    if (found != string::npos) {
        // Go to beginning of diffuse factor.
        found += sizeof("diffuse")-1;
        end = segment.find_first_not_of("0123456789.", found);
        temp = segment.substr(found, end-found);
        diffuse = atof(temp.c_str());
    }
    
    found = segment.find("specular");
    if (found != string::npos) {
        // Go to beginning of specular factor.
        found += sizeof("specular")-1;
        end = segment.find_first_not_of("0123456789.", found);
        temp = segment.substr(found, end-found);
        specular = atof(temp.c_str());
    }
    
    found = segment.find("roughness");
    if (found != string::npos) {
        // Go to beginning of roughness factor.
        found += sizeof("roughness")-1;
        end = segment.find_first_not_of("0123456789.", found);
        temp = segment.substr(found, end-found);
        roughness = atof(temp.c_str());
    }
    
    found = segment.find("metallic");
    if (found != string::npos) {
        // Go to beginning of metallic factor.
        found += sizeof("metallic")-1;
        end = segment.find_first_not_of("0123456789.", found);
        temp = segment.substr(found, end-found);
        metallic = atof(temp.c_str());
    }
    
    found = segment.find("ior");
    if (found != string::npos) {
        // Go to beginning of ior factor.
        found += sizeof("ior")-1;
        end = segment.find_first_not_of("0123456789.", found);
        temp = segment.substr(found, end-found);
        ior = atof(temp.c_str());
    }
    
    shared_ptr<Sphere> sphere = make_shared<Sphere>(center, radius, color,
                                                    ambient, diffuse, specular,
                                                    roughness, metallic, ior);
    objects.push_back(sphere);
}

void FileParser::parsePlane(ifstream &file)
{
    vec3 normal = vec3(0, 0, 0);
    vec3 color = vec3(0, 0, 0);
    int distance = 0;
    float ambient = 0, diffuse = 0;
    size_t found = 0, end = 0;
    string segment = "", temp;
    
    // Since there are subsections denoted with curly braces, we have to search
    // for the '}' without a corresponding '{'. This means that it is the end
    // of the plane information.
    getline(file, temp, '}');
    while (count(temp.begin(), temp.end(), '{') > 0) {
        temp.append("}");
        segment.append(temp);
        getline(file, temp, '}');
    }
    segment.append(temp);
    
    // Remove all whitespace from the string.
    segment.erase(remove(segment.begin(), segment.end(), ' '), segment.end());
    // Remove all newlines from the string.
    segment.erase(remove(segment.begin(), segment.end(), '\n'), segment.end());
    
    found = segment.find("<");
    if (found != string::npos) {
        // Go to beginning of x value for normal.
        found += sizeof("<")-1;
        end = segment.find(",", found);
        temp = segment.substr(found, end-found);
        normal.x = atof(temp.c_str());
        
        // Go to beginning of y value for normal.
        found = end + 1;
        end = segment.find(",", found);
        temp = segment.substr(found, end-found);
        normal.y = atof(temp.c_str());
        
        // Go to beginning of y value for normal.
        found = end + 1;
        end = segment.find(">", found);
        temp = segment.substr(found, end-found);
        normal.z = atof(temp.c_str());
    }
    
    found = segment.find(",", end);
    if (found != string::npos) {
        // Go to beginning of distance.
        found += sizeof(",")-1;
        end = segment.find_first_not_of("0123456789-", found);
        temp = segment.substr(found, end-found);
        distance = atoi(temp.c_str());
    }
    
    found = segment.find("colorrgb");
    if (found != string::npos) {
        // Go to beginning of r value for color.
        found += sizeof("colorrgb");
        end = segment.find(",", found);
        temp = segment.substr(found, end-found);
        color.x = atof(temp.c_str());
        
        // Go to beginning of g value for color.
        found = end + 1;
        end = segment.find(",", found);
        temp = segment.substr(found, end-found);
        color.y = atof(temp.c_str());
        
        // Go to beginning of b value for color.
        found = end + 1;
        end = segment.find(">", found);
        temp = segment.substr(found, end-found);
        color.z = atof(temp.c_str());
    }
    
    found = segment.find("ambient");
    if (found != string::npos) {
        // Go to beginning of ambient factor.
        found += sizeof("ambient")-1;
        end = segment.find_first_not_of("0123456789.", found);
        temp = segment.substr(found, end-found);
        ambient = atof(temp.c_str());
    }
    
    found = segment.find("diffuse");
    if (found != string::npos) {
        // Go to beginning of diffuse factor.
        found += sizeof("diffuse")-1;
        end = segment.find_first_not_of("0123456789.", found);
        temp = segment.substr(found, end-found);
        diffuse = atof(temp.c_str());
    }

    shared_ptr<Plane> plane = make_shared<Plane>(normal, distance, color,
                                                 ambient, diffuse, 0.1f,
                                                 0.1f, 0.1f, 1.6f);
    objects.push_back(plane);
}
