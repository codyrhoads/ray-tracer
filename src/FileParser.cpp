//
//  FileParser.cpp
//  ray-tracer
//
//  Created by Cody Rhoads on 4/4/17.
//
//

#include <algorithm>
#include <glm/gtc/matrix_transform.hpp>

#include "FileParser.hpp"
#include "Camera.hpp"
#include "LightSource.hpp"
#include "Plane.hpp"
#include "Sphere.hpp"
#include "Box.hpp"
#include "Triangle.hpp"
#include "BVHNode.hpp"

using namespace std;
using namespace glm;

FileParser::FileParser() :
camera(make_shared<Camera>()),
lights(vector<shared_ptr<LightSource>>()),
objects(vector<shared_ptr<SceneObject>>())
{
    
}

void FileParser::parse(const string &filename, const bool useBVH)
{
    string segment;
    ifstream file;
    int ID = 1;
    
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
        else if (segment.find("light_source") != string::npos) {
            parseLight(file);
        }
        /* PARSING PLANE OBJECT */
        else if (segment.find("plane") != string::npos) {
            parsePlane(file, ID++);
        }
        /* PARSING SPHERE OBJECT */
        if (segment.find("sphere") != string::npos) {
            parseSphere(file, ID++, useBVH);
        }
        /* PARSING BOX OBJECT */
        if (segment.find("box") != string::npos) {
            parseBox(file, ID++, useBVH);
        }
        /* PARSING TRIANGLE OBJECT */
        else if (segment.find("triangle") != string::npos) {
            parseTriangle(file, ID++, useBVH);
        }
    }
    
    if (useBVH) {
        shared_ptr<BVHNode> BVHroot = make_shared<BVHNode>();
        BVHroot->recursiveBuildTree(nonPlanes, 0);
        objects.push_back(BVHroot);
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
    
    removeNewlinesAndWhitespace(segment);
    
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
    
    removeNewlinesAndWhitespace(segment);
    
    findAndSetVec3Parameter(segment, location, "<", 0);
    findAndSetVec3Parameter(segment, color, "colorrgb<", 0);
    
    shared_ptr<LightSource> ls = make_shared<LightSource>(location, color);
    lights.push_back(ls);
}

void FileParser::parsePlane(ifstream &file, const int ID)
{
    vec3 normal = vec3(0);
    vec3 color = vec3(0);
    vec4 rgbf = vec4(0);
    float distance = 0;
    float ambient = 0, diffuse = 0, specular = 0, reflection = 0;
    float roughness = 0.3, metallic = 0.1, ior = 1.6;
    mat4 inverseModelMatrix = mat4(1.0f);
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
    
    removeNewlinesAndWhitespace(segment);
    
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
    inverseModelMatrix = calculateInverseModelMatrix(segment);
    
    shared_ptr<Plane> plane;
    if (hasFilter) {
        plane = make_shared<Plane>(ID, normal, distance, vec3(rgbf), ambient, diffuse,
                                   specular, reflection, rgbf.w, roughness, metallic,
                                   ior, inverseModelMatrix);
    }
    else {
        plane = make_shared<Plane>(ID, normal, distance, color, ambient, diffuse, specular,
                                   reflection, 0, roughness, metallic, ior, inverseModelMatrix);
    }
    
    objects.push_back(plane);
}

void FileParser::parseSphere(ifstream &file, const int ID, const bool useBVH)
{
    vec3 center = vec3(0);
    vec3 color = vec3(0);
    vec4 rgbf = vec4(0);
    float radius = 0, ambient = 0, diffuse = 0, specular = 0, reflection = 0;
    float roughness = 0.3, metallic = 0.1, ior = 1.6;
    mat4 inverseModelMatrix = mat4(1.0f);
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
    
    removeNewlinesAndWhitespace(segment);
    
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
    inverseModelMatrix = calculateInverseModelMatrix(segment);
    
    shared_ptr<Sphere> sphere;
    if (hasFilter) {
        sphere = make_shared<Sphere>(ID, center, radius, vec3(rgbf), ambient, diffuse,
                                     specular, reflection, rgbf.w, roughness, metallic,
                                     ior, inverseModelMatrix);
    }
    else {
        sphere = make_shared<Sphere>(ID, center, radius, color, ambient, diffuse, specular,
                                     reflection, 0, roughness, metallic, ior, inverseModelMatrix);
    }
    
    if (useBVH) {
        nonPlanes.push_back(sphere);
    }
    else {
        objects.push_back(sphere);
    }
}

void FileParser::parseBox(ifstream &file, const int ID, const bool useBVH)
{
    vec3 mins = vec3(0);
    vec3 maxes = vec3(0);
    vec3 color = vec3(0);
    vec4 rgbf = vec4(0);
    float ambient = 0, diffuse = 0, specular = 0, reflection = 0;
    float roughness = 0.3, metallic = 0.1, ior = 1.6;
    mat4 inverseModelMatrix = mat4(1.0f);
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
    
    removeNewlinesAndWhitespace(segment);
    
    newStart = findAndSetVec3Parameter(segment, mins, "<", 0);
    findAndSetVec3Parameter(segment, maxes, "<", newStart);
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
    inverseModelMatrix = calculateInverseModelMatrix(segment);
    
    shared_ptr<Box> box;
    if (hasFilter) {
        box = make_shared<Box>(ID, mins, maxes, vec3(rgbf), ambient, diffuse,
                               specular, reflection, rgbf.w, roughness, metallic,
                               ior, inverseModelMatrix);
    }
    else {
        box = make_shared<Box>(ID, mins, maxes, color, ambient, diffuse, specular,
                               reflection, 0, roughness, metallic, ior, inverseModelMatrix);
    }
    
    if (useBVH) {
        nonPlanes.push_back(box);
    }
    else {
        objects.push_back(box);
    }
}

void FileParser::parseTriangle(ifstream &file, const int ID, const bool useBVH)
{
    vec3 v0 = vec3(0);
    vec3 v1 = vec3(0);
    vec3 v2 = vec3(0);
    vec3 color = vec3(0);
    vec4 rgbf = vec4(0);
    float ambient = 0, diffuse = 0, specular = 0, reflection = 0;
    float roughness = 0.3, metallic = 0.1, ior = 1.6;
    mat4 inverseModelMatrix = mat4(1.0f);
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
    
    removeNewlinesAndWhitespace(segment);
    
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
    inverseModelMatrix = calculateInverseModelMatrix(segment);
    
    shared_ptr<Triangle> triangle;
    if (hasFilter) {
        triangle = make_shared<Triangle>(ID, v0, v1, v2, vec3(rgbf), ambient, diffuse,
                                         specular, reflection, rgbf.w, roughness,
                                         metallic, ior, inverseModelMatrix);
    }
    else {
        triangle = make_shared<Triangle>(ID, v0, v1, v2, color, ambient, diffuse,
                                         specular, reflection, 0, roughness,
                                         metallic, ior, inverseModelMatrix);
    }
    
    if (useBVH) {
        nonPlanes.push_back(triangle);
    }
    else {
        objects.push_back(triangle);
    }
}

void FileParser::findAndSetSingleValueParameter(const string &segment, float &parameter,
                                                const string &indicator, const size_t start)
{
    size_t found = -1, end = -1;
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

size_t FileParser::findAndSetVec3Parameter(const string &segment, vec3 &parameter,
                                           const string &indicator, const size_t start)
{
    size_t found = -1, end = -1;
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

bool FileParser::findAndSetVec4Parameter(const string &segment, vec4 &parameter,
                                         const string &indicator, const size_t start)
{
    size_t found = -1, end = -1;
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

mat4 FileParser::calculateInverseModelMatrix(const string &segment)
{
    size_t end = -1, startSubSegment = -1, endSubSegment = -1;
    vec3 scale, rotate, translate;
    string subSegment;
    
    mat4 modelMatrix = mat4(1.0f);
    
    startSubSegment = findIndexOfFirstTransform(segment);
    endSubSegment = segment.find(">", startSubSegment);
    while (endSubSegment != string::npos) {
        // Adding 1 to endSubSegment makes the subSegment include the '>' char,
        // which is needed to make findAndSetVec3Parameter work properly.
        endSubSegment += 1;
        subSegment = segment.substr(startSubSegment, endSubSegment - startSubSegment);
        
        end = findAndSetVec3Parameter(subSegment, scale, "scale<", 0);
        if (end != -1) {
            modelMatrix = glm::scale(mat4(1.0f), scale) * modelMatrix;
        }
        else {
            end = findAndSetVec3Parameter(subSegment, rotate, "rotate<", 0);
            if (end != -1) {
                mat4 rotation;
                rotation = glm::rotate(mat4(1.0f), radians(rotate.z), vec3(0, 0, 1)) * rotation;
                rotation = glm::rotate(mat4(1.0f), radians(rotate.y), vec3(0, 1, 0)) * rotation;
                rotation = glm::rotate(mat4(1.0f), radians(rotate.x), vec3(1, 0, 0)) * rotation;
                modelMatrix = rotation * modelMatrix;
            }
            else {
                end = findAndSetVec3Parameter(subSegment, translate, "translate<", 0);
                if (end != -1) {
                    modelMatrix = glm::translate(mat4(1.0f), translate) * modelMatrix;
                }
                else {
                    // Found end of tranforms: break out of loop.
                    break;
                }
            }
        }
        
        startSubSegment = endSubSegment;
        endSubSegment = segment.find(">", startSubSegment);
    }
    
    return glm::inverse(modelMatrix);
}

size_t FileParser::findIndexOfFirstTransform(const string &segment)
{
    size_t rotate = -1, scale = -1, translate = -1, result = numeric_limits<size_t>::max();
    
    rotate = segment.find("rotate", 0);
    scale = segment.find("scale", 0);
    translate = segment.find("translate", 0);
    
    result = std::min(result, rotate);
    result = std::min(result, scale);
    result = std::min(result, translate);
    
    return result;
}

void FileParser::removeNewlinesAndWhitespace(string &segment)
{
    // Remove all newlines from the string.
    segment.erase(remove(segment.begin(), segment.end(), '\n'), segment.end());
    // Remove all whitespace from the string.
    segment.erase(remove(segment.begin(), segment.end(), ' '), segment.end());
}
