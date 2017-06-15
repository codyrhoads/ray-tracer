# ray-tracer

## Final Project Info

### Final Feature
For my final feature I chose to implement ambient occlusion. I wanted to add some feature that added more realism to shading; in particular, I felt that intersecting objects were lacking in the realism department. There wasn't any indication of a transition between the two objects. AO helped to fix that issue by darkening the area where the objects intersect.

To implement this, I refactored my shadow calculation code to allow for objects to be in partial shadow. This is represented by a "shadow factor" variable, which is a float between 0 and 1. 0 is entirely in shadow, while 1 is entirely in light. When testing the color of a point, I send feeler rays from that point to see how occluded it is. If a ray doesn't intersect any object OR if it intersects an object that is far away from the point, then I add 1/(total amount of feeler rays) to the shadow factor variable. Over the course of many feeler rays, the shadow factor begins to reflect how dark the point should be. For example, if the point is close to the intersection of two objects, then many feeler rays would have intersections close to their origins, making the shadow factor variable closer to 0. This shadow factor is then multiplied by the diffuse and specular components. Thus, an occluded point will have a smaller shadow factor, and therefore have reduced diffuse and specular shading. This results in darkened areas close to the intersections of objects to reflect the fact that these areas are occluded and don't receive as much light.

### Resources
I only used the provided notes on ambient occlusion, as well as previous knowledge about the method of implementing Monte Carlo global illumination.

### Software Design Implementation
Initially, my FileParser.cpp file was incredibly long and redundant. Parsing a variable had a few key steps, and these steps had to be rewritten for every parsed variable for every parsed object. This led to lengthy, repetitive methods that were difficult to read.

To fix this issue, I realized that variables inside pov files could only be one of three types: a single float, a vec3, or a vec4. Knowing this, I was able to simplify my code by creating three generic methods that each specialized in parsing one of these types. This greatly reduced the length of FileParser.cpp, and increased legibility as well. Separating functionality into several methods also made it easier to debug issues with my parser program.

## General Info

### Parsing
Parses Camera, Lights, Spheres, Boxes, Planes, and Triangles correctly. Comments are ignored. Object transforms manipulate the object properly, provided all transforms are given consecutively.

### Rendering
Images can render with either Blinn-Phong and Cook-Torrance. Reflectance and Refraction are functioning properly. Schlick's Approximation, Beer's Law, anti-aliasing, global illumination, and ambient occlusion are implemented.

### Spatial Data Structure
Bounding Volume Hierarchy is implemented.
