//
//  OptionalArgs.cpp
//  raytrace
//
//  Created by Cody Rhoads on 6/7/17.
//
//

#include <stdlib.h>

#include "OptionalArgs.hpp"

OptionalArgs::OptionalArgs()
{
    fresnel = false;
    superSampleN = 0;
    useBVH = false;
    globalIllum = false;
    giSamples = 128;
    giBounces = 2;
    giRatio = 4;
    BRDF = "Blinn-Phong";
};

OptionalArgs::OptionalArgs(char **argv, const int argc, const int start) :
OptionalArgs()
{
    for (int i = start; i < argc; i++) {
        std::string arg = argv[i];
        std::string subArg = arg.substr(0, 4);
        if (arg == "-fresnel") {
            fresnel = true;
        }
        else if (subArg == "-ss=") {
            superSampleN = atoi(arg.substr(4, arg.length() - 4).c_str());
        }
        else if (arg == "-sds") {
            useBVH = true;
        }
        else if (arg == "-gi") {
            globalIllum = true;
        }
        else if (subArg == "-gi_") {
            std::string sampleCheck = arg.substr(4, 8);
            std::string bounceCheck = arg.substr(4, 8);
            std::string ratioCheck = arg.substr(4, 6);
            if (sampleCheck == "samples=") {
                giSamples = atoi(arg.substr(12, arg.length() - 12).c_str());
            }
            else if (bounceCheck == "bounces=") {
                giBounces = atoi(arg.substr(12, arg.length() - 12).c_str());
            }
            else if (ratioCheck == "ratio=") {
                giRatio = atoi(arg.substr(10, arg.length() - 10).c_str());
            }
        }
        else if (arg == "-altbrdf") {
            BRDF = "Alternate";
        }
    }
}
