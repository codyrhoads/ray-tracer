// cs473, ao
camera {
  location  <-2, 0, 14>
  up        <0,  1,  0>
  right     <1.333333, 0,  0>
  look_at   <0, 0, 0>
}


light_source {<0, 5, 10> color rgb <1.0, 1.0, 1.0>}

// floor
plane {<0, 1, 0>, -3
  pigment {color rgb <0.2, 0.2, 0.8>}
  finish {ambient 0.1 diffuse 0.8}
  translate <0, -1, 0>
  rotate <-10, 0, 0>
}

// back wall
plane {<0, 0, 1>, 0
  pigment {color rgb <0.8, 0.8, 0.2>}
  finish {ambient 0.1 diffuse 0.8}
  rotate <0, 30, 0>
}

sphere { <6, 1, -3>, 2
  pigment { color rgb <1.0, 1.0, 1.0>}
  finish {ambient 0.1 diffuse 0.8}
  translate <-4, -1, 2>
}

sphere { <2, 1, -2>, 2
  pigment { color rgb <1.0, 1.0, 1.0>}
  finish {ambient 0.2 diffuse 0.8}
  translate <-4, -1, 2>
}

triangle { <-1, 5, -1> <-1, 10, -1> <4, 5, 4>
  pigment {color rgb <1.0, 1.0, 1.0>}
  finish {ambient 0.1 diffuse 0.8}
  translate <0, -2, 0>
}

box { <-1, -1, -1>, <1, 1, 1>
  pigment { color rgb <1.0, 1.0, 1.0>}
  finish {ambient 0.1 diffuse 0.8}
  translate <0, -1, 0>
  rotate <5, 25, 0>
}

box { <-2, -2, -2>, <1, 1, 1>
  pigment { color rgb <1.0, 1.0, 1.0>}
  finish {ambient 0.1 diffuse 0.8}
  translate <0, 3, 0>
  rotate <0, 45, 0>
}
