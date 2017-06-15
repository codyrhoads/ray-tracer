//
//  OptionalArgs.hpp
//  raytrace
//
//  Created by Cody Rhoads on 5/21/17.
//
//

#ifndef OptionalArgs_hpp
#define OptionalArgs_hpp

#include <string>

struct OptionalArgs
{
    OptionalArgs();
    OptionalArgs(char **argv, const int argc, const int start);
    
    bool fresnel;
    int superSampleN;
    bool useBVH;
    bool globalIllum;
    int giSamples;
    int giBounces;
    int giRatio;
    bool ambientOcc;
    std::string BRDF;
};

#endif /* OptionalArgs_hpp */
