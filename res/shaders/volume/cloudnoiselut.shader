#include "vertex.include.shader"
#include "common.shader"
/**
  Buffer A generates Perlin-Worley and Worley fbm noises used for modeling clouds
  in buffer C. This buffer only writes to texture at the beginning or whenever the
  viewport resolution is changed.
*/



out vec4 FragColor;

void main()
{
   
    FragColor = vec4(1);
}