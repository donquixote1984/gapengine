in vec2 TexCoords;
in vec3 Normal;
in vec3 FragPos;
in vec3 FragViewPos;
in vec3 ScreenSpaceDir;
out vec4 FragColor;
const float PI = 3.14159265359;
const float MAX_REFLECTION_LOD = 4.0;
const vec3 earth_center = vec3(0, -6360, 0);
{{__TBN_FRAG_IN__}}
{{__INSTANCING_UV_FRAG__}}