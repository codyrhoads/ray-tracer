# ray-tracer

## Parsing
Parses Camera, Lights, Spheres, Planes, and Triangles correctly. Comments are ignored. Object transforms function properly, provided they are given consecutively and are the last object information provided.

## Rendering
Images render fine with Blinn-Phong and Cook-Torrance. Reflectance and Refraction are functioning properly. Schlick's Approximation and Beer's Law are implemented. Anti-aliasing is implemented.

## Diagnostic/Testing
Most test commands (sceneinfo, pixelray, firsthit, pixelcolor) are functioning as intended.

## Personal POV File
For my own POV file, I decided to play with having multiple lights to create what I find to be an interesting and visually pleasing scene. The POV file is located at resources/my_pov_file.pov and the png image is located at my_pov_file.png.
