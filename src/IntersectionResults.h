//
//  IntersectionResults.h
//  raytrace
//
//  Created by Cody Rhoads on 5/28/17.
//
//

#ifndef IntersectionResults_h
#define IntersectionResults_h

#include <memory>

class SceneObject;

struct IntersectionResults
{
    bool foundIntersection;
    float t;
    std::shared_ptr<SceneObject> intersectedObj;
    
    IntersectionResults()
    {
        foundIntersection = false;
        t = -1;
        intersectedObj = nullptr;
    };
};

#endif /* IntersectionResults_h */
