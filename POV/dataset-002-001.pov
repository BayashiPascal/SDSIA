#include "colors.inc"
#include "textures.inc"

#declare _texMaskTarget = texture {
  pigment { color Black }
}

#declare _texMaskNonTarget = texture {
  pigment { color White }
}

#declare RndSeed = seed(clock);

camera {
  location <0.0,10.0,0.0>
  look_at <0.0,0.0,0.0>
  right x
}

#macro rnd(A,B)
  (A+(B-A)*rand(RndSeed))
#end

light_source {
  <rnd(-5.0, 5.0), 10.0, rnd(-5.0, 5.0)>
  color rgb 1.0
}

background { color rgb <1.0, 1.0, 1.0> }

#declare TargetA = box {
  -1, 1
  rotate <rnd(-90.0, 90.0), rnd(-90.0, 90.0), rnd(-90.0, 90.0)>
  translate x * rnd(-3, 3)
  translate z * rnd(-3, 3)
  scale rnd(0.5, 1.5)
  #if (Mask = 0)
    pigment { color Red }
  #else
    #if (Mask = 1)
      texture {_texMaskTarget}
    #else
      texture {_texMaskNonTarget}
    #end
  #end
};

#declare TargetB = cylinder { 
  -y, y, 0.5 
  scale rnd(0.5, 1.5)
  rotate <rnd(-90.0, 90.0), rnd(-90.0, 90.0), rnd(-90.0, 90.0)>
  translate x * rnd(-5, 5)
  translate z * rnd(-5, 5)
  #if (Mask = 0)
    pigment { color Blue }
  #else
    #if (Mask = 1)
      texture {_texMaskNonTarget}
    #else
      texture {_texMaskTarget}
    #end
  #end
}

object { TargetA }
object { TargetB }
