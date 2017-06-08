//
//  Math.hpp
//  raytrace
//
//  Created by Cody Rhoads on 6/6/17.
//
//

#ifndef Math_hpp
#define Math_hpp

#include <vector>
#include <glm/gtc/type_ptr.hpp>

class Math
{
public:
    static float schlicksApproximation(const float ior, const glm::vec3 &normal,
                                       const glm::vec3 &view);
    static glm::vec3 getAttenuation(const glm::vec3 &color, const float distance);
    static glm::vec3 generateCosineWeightedPoint(const float u, const float v);
    static glm::vec3 alignSampleVector(const glm::vec3 &sample, const glm::vec3 &up,
                                       const glm::vec3 &normal);
    static float randFloat(const float low, const float high);
};

#endif /* Math_hpp */
