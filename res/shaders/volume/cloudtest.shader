#include "vertex.include.shader"

uniform sampler2D u_CloudNoiseTexture;

out vec4 FragColor;
void main()
{
    FragColor = texture(u_CloudNoiseTexture, TexCoords);
}