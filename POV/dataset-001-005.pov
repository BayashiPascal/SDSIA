#include "colors.inc"
#include "textures.inc"

#declare _texMaskTarget = texture {
  pigment { color Black }
}

#declare RndSeed = seed(clock);
#declare _posCamera = <0.0,10.0,0.0>;
#declare _lookAt = <0.0,0.0,0.0>;

camera {
  location    <0.0,10.0,0.0>
  look_at     <0.0,0.0,0.0>
  right x
}

#macro rnd(A,B)
  (A+(B-A)*rand(RndSeed))
#end

light_source {
  <rnd(-5.0, 5.0), 10.0, rnd(-5.0, 5.0)>
  color rgb 1.0
}

#if (Mask = 0)
  background { color rgb <1.0, 0.0, 0.0> }
#else
  background { color rgb <1.0, 1.0, 1.0> }
#end

#declare Target = box {
  -1, 1
  #if (Mask = 0)
    pigment {
      bozo 
      scale 0.5
      color_map { 
        [0.0 color rgb <1.0, 0.0, 1.0>]
        [0.0 color rgb <0.0, 1.0, 0.0>]
        [1.0 color rgb <0.5, 0.5, 1.0>]
      }
    }
  #else
    texture {_texMaskTarget}
  #end
  scale rnd(0.5, 1.5)
  rotate <rnd(-90.0, 90.0), rnd(-90.0, 90.0), rnd(-90.0, 90.0)>
  translate x * rnd(-3, 3)
  translate z * rnd(-3, 3)
};

object { Target }
