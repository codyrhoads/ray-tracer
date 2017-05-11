//
//  FileParser.cpp
//  ray-tracer
//
//  Created by Cody Rhoads on 4/4/17.
//
//

#include <algorithm>

#include "FileParser.hpp"
#include "Camera.hpp"
#include "LightSource.hpp"
#include "Sphere.hpp"
#include "Plane.hpp"
#include "Triangle.hpp"

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
    
    // The '{' character marks the beginning of a new section (camera, light,
    // object).
    while (getline(file, segment, '{')) {
        // Remove all comments from the segment
        size_t found = segment.find("//");
        while (found != string::npos) {
            segment.erase(segment.begin() + found,
                          segment.begin() + segment.find('\n', found + 1));
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
        
        /* PARSING TRIANGLE OBJECT */
        if (segment.find("triangle") != string::npos) {
            parseTriangle(file);
        }
    }
    
    file.close();
}

void FileParser::parseCamera(ifstream &file)
{
    string segment;
    vec3 location = vec3(0);
    vec3 up = vec3(0);
    vec3 right = vec3(0);
    vec3 lookAt = vec3(0);
    
    // A '}' marks the end of a section. The camera doesn't have any subsections
    // that use curly brackets, so we can assume that the '}' is the end of the
    // camera object. Segment now contains all of the information for the camera.
    getline(file, segment, '}');
    
    // Remove all whitespace from the string.
    segment.erase(remove(segment.begin(), segment.end(), ' '), segment.end());
    // Remove all newlines from the string.
    segment.erase(remove(segment.begin(), segment.end(), '\n'), segment.end());
    
    findAndSetVec3Parameter(segment, location, "location<", 0);
    findAndSetVec3Parameter(segment, up, "up<", 0);
    findAndSetVec3Parameter(segment, right, "right<", 0);
    findAndSetVec3Parameter(segment, lookAt, "look_at<", 0);
    
    camera = make_shared<Camera>(location, up, right, lookAt);
}

void FileParser::parseLight(ifstream &file)
{
    vec3 location = vec3(0), color = vec3(0);
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
    
    findAndSetVec3Parameter(segment, location, "<", 0);
    findAndSetVec3Parameter(segment, color, "colorrgb<", 0);
    
    shared_ptr<LightSource> ls = make_shared<LightSource>(location, color);
    lights.push_back(ls);
}

void FileParser::parseSphere(ifstream &file)
{
    vec3 center = vec3(0);
    vec3 color = vec3(0);
    vec4 rgbf = vec4(0);
    float radius = 0, ambient = 0, diffuse = 0, specular = 0, reflection = 0;
    float roughness = 0.3, metallic = 0.1, ior = 1.6;
    size_t newStart = 0;
    string segment = "", temp;
    bool hasFilter = false;
    
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
    
    newStart = findAndSetVec3Parameter(segment, center, "<", 0);
    findAndSetSingleValueParameter(segment, radius, ",", newStart);
    hasFilter = findAndSetVec4Parameter(segment, rgbf, "colorrgbf<", 0);
    if (!hasFilter) {
        findAndSetVec3Parameter(segment, color, "colorrgb<", 0);
    }
    findAndSetSingleValueParameter(segment, ambient, "ambient", 0);
    findAndSetSingleValueParameter(segment, diffuse, "diffuse", 0);
    findAndSetSingleValueParameter(segment, specular, "specular", 0);
    findAndSetSingleValueParameter(segment, reflection, "reflection", 0);
    findAndSetSingleValueParameter(segment, roughness, "roughness", 0);
    findAndSetSingleValueParameter(segment, metallic, "metallic", 0);
    findAndSetSingleValueParameter(segment, ior, "ior", 0);
    
    shared_ptr<Sphere> sphere;
    if (hasFilter) {
        sphere = make_shared<Sphere>(center, radius, vec3(rgbf), ambient, diffuse,
                                     specular, reflection, rgbf.w, roughness, metallic,
                                     ior);
    }
    else {
        sphere = make_shared<Sphere>(center, radius, color, ambient, diffuse, specular,
                                     reflection, 0, roughness, metallic, ior);
    }
    
    objects.push_back(sphere);
}

void FileParser::parsePlane(ifstream &file)
{
    vec3 normal = vec3(0);
    vec3 color = vec3(0);
    vec4 rgbf = vec4(0);
    float distance = 0;
    float ambient = 0, diffuse = 0, specular = 0, reflection = 0;
    float roughness = 0.3, metallic = 0.1, ior = 1.6;
    size_t newStart = 0;
    string segment = "", temp;
    bool hasFilter = false;
    
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
    
    newStart = findAndSetVec3Parameter(segment, normal, "<", 0);
    findAndSetSingleValueParameter(segment, distance, ",", newStart);
    hasFilter = findAndSetVec4Parameter(segment, rgbf, "colorrgbf<", 0);
    if (!hasFilter) {
        findAndSetVec3Parameter(segment, color, "colorrgb<", 0);
    }
    findAndSetSingleValueParameter(segment, ambient, "ambient", 0);
    findAndSetSingleValueParameter(segment, diffuse, "diffuse", 0);
    findAndSetSingleValueParameter(segment, specular, "specular", 0);
    findAndSetSingleValueParameter(segment, reflection, "reflection", 0);
    findAndSetSingleValueParameter(segment, roughness, "roughness", 0);
    findAndSetSingleValueParameter(segment, metallic, "metallic", 0);
    findAndSetSingleValueParameter(segment, ior, "ior", 0);

    shared_ptr<Plane> plane;
    if (hasFilter) {
        plane = make_shared<Plane>(normal, distance, vec3(rgbf), ambient, diffuse,
                                   specular, reflection, rgbf.w, roughness, metallic,
                                   ior);
    }
    else {
        plane = make_shared<Plane>(normal, distance, color, ambient, diffuse, specular,
                                   reflection, 0, roughness, metallic, ior);
    }
    objects.push_back(plane);
}

void FileParser::parseTriangle(ifstream &file)
{
    vec3 v0 = vec3(0);
    vec3 v1 = vec3(0);
    vec3 v2 = vec3(0);
    vec3 color = vec3(0);
    vec4 rgbf = vec4(0);
    float ambient = 0, diffuse = 0, specular = 0, reflection = 0;
    float roughness = 0.3, metallic = 0.1, ior = 1.6;
    size_t newStart = 0;
    string segment = "", temp;
    bool hasFilter = false;
    
    // Since there are subsections denoted with curly braces, we have to search
    // for the '}' without a corresponding '{'. This means that it is the end
    // of the triangle information.
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
    
    newStart = findAndSetVec3Parameter(segment, v0, "<", 0);
    newStart = findAndSetVec3Parameter(segment, v1, "<", newStart);
    findAndSetVec3Parameter(segment, v2, "<", newStart);
    hasFilter = findAndSetVec4Parameter(segment, rgbf, "colorrgbf<", 0);
    if (!hasFilter) {
        findAndSetVec3Parameter(segment, color, "colorrgb<", 0);
    }
    findAndSetSingleValueParameter(segment, ambient, "ambient", 0);
    findAndSetSingleValueParameter(segment, diffuse, "diffuse", 0);
    findAndSetSingleValueParameter(segment, specular, "specular", 0);
    findAndSetSingleValueParameter(segment, reflection, "reflection", 0);
    findAndSetSingleValueParameter(segment, roughness, "roughness", 0);
    findAndSetSingleValueParameter(segment, metallic, "metallic", 0);
    findAndSetSingleValueParameter(segment, ior, "ior", 0);
    
    shared_ptr<Triangle> triangle;
    if (hasFilter) {
        triangle = make_shared<Triangle>(v0, v1, v2, vec3(rgbf), ambient, diffuse,
                                         specular, reflection, rgbf.w, roughness,
                                         metallic, ior);
    }
    else {
        triangle = make_shared<Triangle>(v0, v1, v2, color, ambient, diffuse,
                                         specular, reflection, 0, roughness,
                                         metallic, ior);
    }
    objects.push_back(triangle);
}

void FileParser::findAndSetSingleValueParameter(const string segment, float &parameter,
                                                const string indicator, const size_t start)
{
    size_t found = 0, end = 0;
    string temp;
    
    found = segment.find(indicator, start);
    if (found != string::npos) {
        // Go to beginning of the value to be recorded.
        found += indicator.length();
        end = segment.find_first_not_of("0123456789.-", found);
        temp = segment.substr(found, end - found);
        parameter = atof(temp.c_str());
    }
}

size_t FileParser::findAndSetVec3Parameter(const string segment, vec3 &parameter,
                                             const string indicator, const size_t start)
{
    size_t found = 0, end = 0;
    string temp;
    
    found = segment.find(indicator, start);
    if (found != string::npos) {
        // Go to beginning of x value for parameter.
        found += indicator.length();
        end = segment.find(",", found);
        temp = segment.substr(found, end - found);
        parameter.x = atof(temp.c_str());
        
        // Go to beginning of y value for parameter.
        found = end + 1;
        end = segment.find(",", found);
        temp = segment.substr(found, end - found);
        parameter.y = atof(temp.c_str());
        
        // Go to beginning of z value for parameter.
        found = end + 1;
        end = segment.find(">", found);
        temp = segment.substr(found, end - found);
        parameter.z = atof(temp.c_str());
    }
    
    return end;
}

bool FileParser::findAndSetVec4Parameter(const string segment, vec4 &parameter,
                                         const string indicator, const size_t start)
{
    size_t found = 0, end = 0;
    bool hasFilter = false;
    string temp;
    
    found = segment.find(indicator, start);
    hasFilter = (found != string::npos);
    if (hasFilter) {
        // Go to beginning of x value for parameter.
        found += indicator.length();
        end = segment.find(",", found);
        temp = segment.substr(found, end - found);
        parameter.x = atof(temp.c_str());
        
        // Go to beginning of y value for parameter.
        found = end + 1;
        end = segment.find(",", found);
        temp = segment.substr(found, end - found);
        parameter.y = atof(temp.c_str());
        
        // Go to beginning of z value for parameter.
        found = end + 1;
        end = segment.find(",", found);
        temp = segment.substr(found, end - found);
        parameter.z = atof(temp.c_str());
        
        // Go to beginning of w value for parameter.
        found = end + 1;
        end = segment.find(">", found);
        temp = segment.substr(found, end - found);
        parameter.w = atof(temp.c_str());
    }
    
    return hasFilter;
}
