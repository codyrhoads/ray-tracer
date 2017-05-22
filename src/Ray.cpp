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

using namespace std;
using namespace glm;

Ray::Ray() :
origin(vec3(0)),
direction(vec3(0)),
t(0),
indexOfIntersected(-1)
{
    
}

Ray::Ray(const vec3 &origin, const vec3 &dir) :
origin(origin),
direction(dir),
t(0),
indexOfIntersected(-1)
{
    
}

int Ray::findClosestObjectIndex(const vector<shared_ptr<SceneObject>> &objects)
{
    int index = -1;
    
    for (unsigned int i = 0; i < objects.size(); i++) {
        float currT;
        if (objects.at(i)->testIntersection(shared_from_this(), currT)) {
            if (index == -1 || t > currT) {
                t = currT;
                index = i;
            }
        }
    }
    
    indexOfIntersected = index;
    return index;
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
