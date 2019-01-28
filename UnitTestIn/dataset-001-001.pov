#include "colors.inc"
#include "textures.inc"

// Black texture used to create the mask of the target
#declare _texMaskTarget = texture {
  pigment { color Black }
}

// White texture used to create the mask of the non-target
#declare _texMaskNonTarget = texture {
  pigment { color White }
}

// Random generator seed, based on the clock variable set by the 
// SDSIA generator
#declare RndSeed = seed(clock);

// Macro to get a random value in [A, B]
#macro rnd(A,B)
  (A+(B-A)*rand(RndSeed))
#end

// Camera
camera {
  location    <0.0,10.0,0.0>
  look_at     <0.0,0.0,0.0>
  right x
}

// Light source
light_source {
  <0.0,10.0,0.0>
}

// Background
background { color rgb <1.0, 1.0, 1.0> }

// Target definition
#declare Target = box {
  -1, 1

  // Transformation of the target to create various samples
  translate x * rnd(0, 1)

  // Apply the real texture or the mask texture according to the 
  // Mask variable set by the SDSIA generator 
  #if (Mask = 0)
    pigment { color Red }
  #else
    texture {_texMaskTarget}
  #end

}

// Non-target definition
#declare NonTarget = cylinder { 
  -y, y, 0.5
  
  // Transformation of the non-target to create various samples
  translate x * rnd(0, 1)

  // Apply the real texture or the mask texture according to the 
  // Mask variable set by the SDSIA generator 
  #if (Mask = 0)
    pigment { color Blue }
  #else
    texture {_texMaskNonTarget}
  #end

}

// Create the scene
object { Target }
object { NonTarget }
