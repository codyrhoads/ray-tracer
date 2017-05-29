//
//  OptionalArgs.h
//  raytrace
//
//  Created by Cody Rhoads on 5/21/17.
//
//

#ifndef OptionalArgs_h
#define OptionalArgs_h

#include <string>

struct OptionalArgs
{
    bool fresnel;
    int superSampleN;
    bool useBVH;
    std::string BRDF;
    
    OptionalArgs()
    {
        fresnel = false;
        superSampleN = 0;
        useBVH = false;
        BRDF = "Blinn-Phong";
    };
    
    OptionalArgs(const bool fres, const int ssN, const bool bvh, const std::string brdf)
    {
        fresnel = fres;
        superSampleN = ssN;
        useBVH = bvh;
        BRDF = brdf;
    };
};

#endif /* OptionalArgs_h */
