//
//  Ray.cpp
//  ray-tracer
//
//  Created by Cody Rhoads on 4/11/17.
//
//

#include <sstream>
#include <iomanip>

#include "SceneObject.hpp"
#include "Ray.hpp"
#include "IntersectionResults.h"

using namespace std;
using namespace glm;

Ray::Ray() :
origin(vec3(0)),
direction(vec3(0)),
t(numeric_limits<float>::max()),
intersectedObj(nullptr)
{
    
}

Ray::Ray(const vec3 &origin, const vec3 &dir) :
origin(origin),
direction(dir),
t(numeric_limits<float>::max()),
intersectedObj(nullptr)
{
    
}

shared_ptr<SceneObject> Ray::findClosestObject(const vector<shared_ptr<SceneObject>> &objects,
                                               const int curObjID)
{
    IntersectionResults tempIR = IntersectionResults();
    for (unsigned int i = 0; i < objects.size(); i++) {
        tempIR = objects.at(i)->findIntersection(shared_from_this());
        if (tempIR.foundIntersection) {
            if (tempIR.intersectedObj->getID() != curObjID && tempIR.t < t) {
                t = tempIR.t;
                intersectedObj = tempIR.intersectedObj;
            }
        }
    }
    
    return intersectedObj;
}

string Ray::getRayInfo() const
{
    ostringstream info;
    info << fixed << setprecision(4) << "{" << origin.x << " " << origin.y << " "
         << origin.z << "} -> {" << direction.x << " " << direction.y << " "
         << direction.z << "}";
    return info.str();
}

string Ray::getIntersectionPointString() const
{
    ostringstream info;
    vec3 intersection = getIntersectionPoint();
    info << fixed << setprecision(4) << "{" << intersection.x << " "
         << intersection.y << " " << intersection.z << "}";
    return info.str();
}
