# ray-tracer

## Parsing
Parses Camera, Lights, Spheres, Boxes, Planes, and Triangles correctly. Comments are ignored. Object transforms manipulate the object properly, provided all transforms are given consecutively.

## Rendering
Images render fine with Blinn-Phong and Cook-Torrance. Reflectance and Refraction are functioning properly. Schlick's Approximation and Beer's Law are implemented. Anti-aliasing is implemented.

## Spatial Data Structure
Bounding Volume Hierarchy is implemented.

## Diagnostic/Testing
Most test commands (sceneinfo, pixelray, firsthit, pixelcolor) are functioning as intended.

## Personal POV File
For my own POV file, I decided to play with having multiple lights to create what I find to be an interesting and visually pleasing scene. The POV file is located at resources/my_pov_file.pov and the png image is located at my_pov_file.png.
