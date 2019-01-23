#include "colors.inc"
#include "textures.inc"

#declare _texMaskTarget = texture {
  pigment { color Black }
  finish { ambient 0 }
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

background { color rgb <1.0, 1.0, 1.0> }

#declare Target = box {
  -1, 1
  scale rnd(0.5, 1.5)
  translate x * rnd(-3, 3)
  translate z * rnd(-3, 3)
  #if (Mask = 0)
    pigment { color Red }
  #else
    texture {_texMaskTarget}
  #end
};

object { Target }
