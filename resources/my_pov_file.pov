// csc473, assignment 4 file

camera {
  location  <0, 1, 15>
  up        <0,  0.9985,  -0.0555>
  right     <1.33333, 0,  0>
  look_at   <0, 0, 0>
}

light_source {<10, 5, 0> color rgb <0, 1.5, 0>}
light_source {<-10, 5, 0> color rgb <0, 0, 1.5>}
light_source {<0, 5, 10> color rgb <1.5, 0, 0>}

// Center
sphere { <0, 0, -5>, 2.0
  pigment { color rgb <1.0, 1.0, 1.0> }
  finish {roughness 0.02 ior 1.6 reflection 1.0}
}

// Left Half Sphere
sphere { <-10, 0, 0>, 3.0
  pigment { color rgbf <1.0, 1.0, 1.0, 0.4> }
  finish {ambient 0.2 diffuse 0.6 specular 0.7 roughness 0.02 ior 1.6 reflection 0.6}
}

// Right Half Sphere
sphere { <10, 0, 0>, 3.0
  pigment { color rgbf <1.0, 1.0, 1.0, 0.4> }
  finish {ambient 0.2 diffuse 0.6 specular 0.7 roughness 0.02 ior 1.6 reflection 0.6}
}

// Left Small
sphere { <-2.75, -3, 0>, 0.8
  pigment { color rgb <0.8, 0.8, 0.8> }
  finish {ambient 0.2 diffuse 0.6 specular 0.7 roughness 0.02 ior 1.6 reflection 0.4}
}

// Right Small
sphere { <2.75, -3, 0>, 0.8
  pigment { color rgb <0.8, 0.8, 0.8> }
  finish {ambient 0.2 diffuse 0.6 specular 0.7 roughness 0.02 ior 1.6 reflection 0.4}
}

// Upper Small
sphere { <0, 3, 0>, 0.8
  pigment { color rgb <0.8, 0.8, 0.8> }
  finish {ambient 0.2 diffuse 0.6 specular 0.7 roughness 0.02 ior 1.6 reflection 0.4}
}

// Floor
plane {<0, 1, 0>, -4
  pigment {color rgb <1.0, 1.0, 1.0>}
  finish {ambient 0.4 diffuse 1.0 reflection 0.5}
}

// Background
plane {<0, 0, 1>, -100
  pigment {color rgb <1.0, 1.0, 1.0>}
  finish {ambient 0.2 diffuse 0.4}
}
