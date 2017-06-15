// cs473, ambient occlusion 2
camera {
  location  <0, 0, 14>
  up        <0,  1,  0>
  right     <1.5, 0,  0>
  look_at   <0, 0, 0>
}


light_source {<-5, 3, 0> color rgb <1.0, 1.0, 1.0>}

// floor
plane {<0, 1, 0>, -4
  pigment {color rgb <1, 1, 1>}
  finish {ambient 0.1 diffuse 0.8}
  translate <0, -1, 0>
}

// left wall
plane {<1, 0, 0>, -8
  pigment {color rgb <1, 0, 0>}
  finish {ambient 0.1 diffuse 0.8}
}

// back wall
plane {<0, 0, -1>, 10
  pigment {color rgb <0, 1, 0>}
  finish {ambient 0.1 diffuse 0.8}
  rotate <0, 10, 0>
}

// right wall on the
//plane {<0, 0, -1>, 8
//  pigment {color rgb <0.0, 0.2, 0.2>}
//  finish {ambient 0.4 diffuse 0.8}
//  rotate <0, -20, 0>
//}

// and a few assorted objects
sphere { <0, 0, 0>, 2
  pigment {color rgb <1.0, 1.0, 1.0>}
  finish {ambient 0.2 diffuse 0.8}
  translate <5.5, -3, -1>
}

box { <-2, -5, -5>, <2, 5, 5>
  pigment { color rgb <1.0, 0.2, 1.0>}
  finish {ambient 0.2 diffuse 0.8}
  rotate <0, -45, 0>
  translate <3, 0, -5>
}

